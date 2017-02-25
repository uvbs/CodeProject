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
/*** 认证 ***/
/**************************************************/
int CSAuthorizePacketHandler::execute(CSAuthorizePacket* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION
	Assert(pPacket);

	if (pPlayer)
	{
		//检查线程执行资源是否正确
		Assert(getCurrTid() == GamePlayerMgr::getSinglePtr()->_tid);
		GamePlayer* pGamePlayer = static_cast<GamePlayer*>(pPlayer);
		const char* user = pPacket->getUserId();
		const char* key = pPacket->getKey();
		//检查GamePlayer状态是否正常, 新连接
		if (pGamePlayer->getStatus() != EM_PLAYER_STATUS_CONNECT)
		{
			LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
				"CSAuthorizePacket::getStatus! UserID=%s, Status=%d", user, pGamePlayer->getStatus());
			return PACKET_EXE_ERROR ;
		}
		//USER, KEY校验
		if (str_util::strlen(user) <= 0 || str_util::strlen(key) <= 0)
		{
			return PACKET_EXE_ERROR;
		}
		// 保存连接的userid
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
/*** 进入场景 ***/
/**************************************************/
int	 CSEnterScenePacketHandler::execute(CSEnterScenePacket* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION
	GamePlayer* pGamePlayer = static_cast<GamePlayer*>(pPlayer);
	Assert(pGamePlayer);
	if (pGamePlayer->getStatus() == EM_PLAYER_STATUS_AUTHORIZED)
	{
		//当前玩家是刚接入的
		//检查线程执行资源是否正确
		Assert(getCurrTid() == GamePlayerMgr::getSinglePtr()->_tid);
		//得到负载最小的线程
		const LogicThread* pThread = LogicThreadMgr::getSinglePtr()->getMinloadThread();
		Assert(pThread && pThread->getScene());
		//删除接入模块中数据
		Player_ID id = pGamePlayer->getPlayerID();
		bool ret = GamePlayerMgr::getSinglePtr()->removePlayer(id);
		if (!ret)
		{
			LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, "CSEnterScenePacket remove player error.") ;
			return PACKET_EXE_ERROR;
		}
		//向目的场景(线程)发送转移消息
		CSEnterScenePacket* pEnterPacket = static_cast<CSEnterScenePacket*>(
			PacketFactoryMgr::getSinglePtr()->createPacket(PACKET_CS_ENTER_SCENE)) ;
		pEnterPacket->setSceneId(pThread->getScene()->getSceneId());
		pThread->getScene()->sendPacket(pEnterPacket, id);
		//更新状态
		pGamePlayer->setStatus(EM_PLAYER_STATUS_ENTER);

		//**注意**
		//必须返回PACKET_EXE_BREAK
		return PACKET_EXE_BREAK;
	}
	else if (pGamePlayer->getStatus() == EM_PLAYER_STATUS_ENTER)
	{
		//场景收到消息，处理添加
		//当前代码由pScene线程(逻辑线程)来执行
		//收到原场景发送过来的消息，由目的场景执行
		//功能:
		//1.添加此玩家到本场景中
		//2.玩家发送一个消息
		int sceneId = pPacket->getSceneId();
		const LogicThread* pThread = LogicThreadMgr::getSinglePtr()->getThread(sceneId);
		Assert(pThread);
		Scene* pScene = pThread->getScene();
		if (NULL == pScene)
		{
			return PACKET_EXE_ERROR ;
		}
		//检查线程执行资源是否正确
		Assert(getCurrTid() == pScene->getTid()) ;
		
		//将客户端连接加入目的场景玩家管理器
		ScenePlayerMgr* pScenePlayerMgr = pScene->getScenePlayerMgr();
		Assert(pScenePlayerMgr);
		bool ret = pScenePlayerMgr->addPlayer(static_cast<GamePlayer*>(pPlayer));
		if (!ret)
		{
			//如果加入场景失败，由于当前玩家已经从原先的场景里脱离，
			//所以只能断开此玩家的网络连接
			LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, "CSEnterScenePacket add player error.");
			return PACKET_EXE_ERROR ;
		}
		//成功进入场景
		pGamePlayer->setStatus(EM_PLAYER_STATUS_NORMAL);

		//向客户端发送进入场景成功的消息
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
/*** 心跳 ***/
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
