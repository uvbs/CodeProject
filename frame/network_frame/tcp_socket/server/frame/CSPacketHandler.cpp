/***************************************************
** @file: CSPacketHandler.cpp
** @author: Mr.Chen
** @date: 2016/7/5
** @brief: 
***************************************************/
#include "socketdef.h"
#include "common.h"
#include "CSAuthorizePacket.h"
#include "SCRetAuthorizePacket.h"
#include "CSEnterScenePacket.h"
#include "SCRetEnterScenePacket.h"
#include "CSHeartBeatPacket.h"
#include "packet_factory_mgr.h"
#include "game_player_mgr.h"
#include "scene_player_mgr.h"
#include "game_player.h"
#include "helper.h"
#include "time_system.h"
#include "log.h"
#include "assert.h"
#include "logic.h"
#include "scene.h"

/**************************************************/
/*** ��֤ ***/
/**************************************************/
int CSAuthorizePacketHandler::execute(CSAuthorizePacket* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION
	Assert(pPacket);

	if (pPlayer)
	{
		//����߳�ִ����Դ�Ƿ���ȷ
		Assert(getCurrTid() == GamePlayerMgr::getSinglePtr()->_tid);
		GamePlayer* pGamePlayer = static_cast<GamePlayer*>(pPlayer);
		const char* user = pPacket->getUserId();
		const char* key = pPacket->getKey();
		//���GamePlayer״̬�Ƿ�����, ������
		if (pGamePlayer->getStatus() != EM_PLAYER_STATUS_CONNECT)
		{
			LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
				"CSAuthorizePacket::getStatus! UserID=%s, Status=%d", user, pGamePlayer->getStatus());
			return PACKET_EXE_ERROR ;
		}
		//USER, KEYУ��
		if (str_util::strlen(user) <= 0 || str_util::strlen(key) <= 0)
		{
			return PACKET_EXE_ERROR;
		}
		// �������ӵ�userid
		pGamePlayer->setUserId(user);
		pGamePlayer->setStatus(EM_PLAYER_STATUS_AUTHORIZED);
		
		SCRetAuthorizePacket packet;
		pGamePlayer->sendPacket(&packet);
		LogSystem::getSinglePtr()->cacheLog( LOG_FILE_DEBUG, 
			"CSAuthorizePacket execute OK Incomingthread, user = %s", user) ;
	}
	
	return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
	return PACKET_EXE_ERROR;
}

/**************************************************/
/*** ���볡�� ***/
/**************************************************/
int	 CSEnterScenePacketHandler::execute(CSEnterScenePacket* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION
	GamePlayer* pGamePlayer = static_cast<GamePlayer*>(pPlayer);
	Assert(pGamePlayer);
	if (pGamePlayer->getStatus() == EM_PLAYER_STATUS_AUTHORIZED)
	{
		//��ǰ����Ǹս����
		//����߳�ִ����Դ�Ƿ���ȷ
		Assert(getCurrTid() == GamePlayerMgr::getSinglePtr()->_tid);
		//�õ�������С���߳�
		const LogicThread* pThread = LogicThreadMgr::getSinglePtr()->getMinloadThread();
		Assert(pThread && pThread->getScene());
		//ɾ������ģ��������
		Player_ID id = pGamePlayer->getPlayerID();
		bool ret = GamePlayerMgr::getSinglePtr()->removePlayer(id);
		if (!ret)
		{
			LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, "CSEnterScenePacket remove player error.") ;
			return PACKET_EXE_ERROR;
		}
		//��Ŀ�ĳ���(�߳�)����ת����Ϣ
		CSEnterScenePacket* pEnterPacket = static_cast<CSEnterScenePacket*>(
			PacketFactoryMgr::getSinglePtr()->createPacket(PACKET_CS_ENTER_SCENE)) ;
		pEnterPacket->setSceneId(pThread->getScene()->getSceneId());
		pThread->getScene()->sendPacket(pEnterPacket, id);
		//����״̬
		pGamePlayer->setStatus(EM_PLAYER_STATUS_ENTER);

		//**ע��**
		//���뷵��PACKET_EXE_BREAK
		return PACKET_EXE_BREAK;
	}
	else if (pGamePlayer->getStatus() == EM_PLAYER_STATUS_ENTER)
	{
		//�����յ���Ϣ���������
		//��ǰ������pScene�߳�(�߼��߳�)��ִ��
		//�յ�ԭ�������͹�������Ϣ����Ŀ�ĳ���ִ��
		//����:
		//1.��Ӵ���ҵ���������
		//2.��ҷ���һ����Ϣ
		int sceneId = pPacket->getSceneId();
		const LogicThread* pThread = LogicThreadMgr::getSinglePtr()->getThread(sceneId);
		Assert(pThread);
		Scene* pScene = pThread->getScene();
		if (NULL == pScene)
		{
			return PACKET_EXE_ERROR ;
		}
		//����߳�ִ����Դ�Ƿ���ȷ
		Assert(getCurrTid() == pScene->getTid()) ;
		
		//���ͻ������Ӽ���Ŀ�ĳ�����ҹ�����
		ScenePlayerMgr* pScenePlayerMgr = pScene->getScenePlayerMgr();
		Assert(pScenePlayerMgr);
		bool ret = pScenePlayerMgr->addPlayer(static_cast<GamePlayer*>(pPlayer));
		if (!ret)
		{
			//������볡��ʧ�ܣ����ڵ�ǰ����Ѿ���ԭ�ȵĳ��������룬
			//����ֻ�ܶϿ�����ҵ���������
			LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, "CSEnterScenePacket add player error.");
			return PACKET_EXE_ERROR ;
		}
		//�ɹ����볡��
		pGamePlayer->setStatus(EM_PLAYER_STATUS_NORMAL);

		//��ͻ��˷��ͽ��볡���ɹ�����Ϣ
		SCRetEnterScenePacket retpacket;
		retpacket.setResult(SCRetEnterScenePacket::EM_ENTER_SUCCESS) ;
		pGamePlayer->sendPacket(&retpacket) ;
		
		return PACKET_EXE_CONTINUE ;
	}
	else
	{
		Assert(0) ;
	}

	return PACKET_EXE_CONTINUE ;
	__LEAVE_FUNCTION
	return PACKET_EXE_ERROR;
}

/**************************************************/
/*** ���� ***/
/**************************************************/
int		CSHeartBeatPacketHandler::execute(CSHeartBeatPacket* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION
	Assert(pPacket);
	Assert(pPlayer);

	GamePlayer* pGamePlayer = static_cast<GamePlayer*>(pPlayer);
	LogSystem::getSinglePtr()->cacheLog(LOG_FILE_INFO,
		"CSHeartBeatPacket OK, user = %s", pGamePlayer->getUserId());
	
	return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
	return PACKET_EXE_ERROR;
}
