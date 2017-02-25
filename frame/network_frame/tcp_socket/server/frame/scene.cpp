#include "scene.h"
#include "log.h"
#include "packet_factory_mgr.h"
#include "player_pool.h"
#include "game_player.h"
#include "time_system.h"
#include <string.h>

Scene::Scene(int nSceneId)
{
	_nTid = -1;
	_nSceneId = nSceneId;

	_pPacketCache = new PacketCache[kPacketCacheGrow];
	Assert(_pPacketCache) ;
	_nPacketCacheCurrSize = kPacketCacheGrow ;
	_nHead = _nTail = 0 ;

	_pScenePlayerMgr = new ScenePlayerMgr();
	Assert(_pScenePlayerMgr);
	_pScenePlayerMgr->setScene(this);
}

Scene::~Scene()
{
	SAFE_DELETE_ARRAY(_pPacketCache);
	SAFE_DELETE(_pScenePlayerMgr);
}

bool Scene::init(int nPlayerCount)
{
	Assert(_pScenePlayerMgr);
	bool ret =  _pScenePlayerMgr->init(nPlayerCount);
	Assert(ret);

	return true;
}

void Scene::cleanUp()
{
	__ENTER_FUNCTION
	_pScenePlayerMgr->cleanUp();
	__LEAVE_FUNCTION
}

bool Scene::tick(uint uTime)
{
	__ENTER_FUNCTION

	bool ret = false;
	__MYTRY 
	{ 
		ret = _pScenePlayerMgr->processExceptions(); Assert(ret); 
		ret = _pScenePlayerMgr->select(); Assert(ret);
		ret = _pScenePlayerMgr->processExceptions(); Assert(ret);
		ret = _pScenePlayerMgr->processInputs(); Assert(ret);
		ret = _pScenePlayerMgr->processOutputs(); Assert(ret);
		ret = _pScenePlayerMgr->processCmds(); Assert(ret);
		processCacheCmds();
		ret = heartBeat(uTime); Assert(ret);
	}
	__MYCATCH
	{
		MyExceptionLog();
	}

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool Scene::recvPacket(Packet*& pPacket, Player_ID& PlayerID, bool& bRemove)
{
	__ENTER_FUNCTION

	AutoMLock lock(_lockQ);
	//没有消息
	if (_pPacketCache[_nHead].pPacket == NULL) { return false; }
	pPacket  = _pPacketCache[_nHead].pPacket ;
	PlayerID = _pPacketCache[_nHead].nPlayerId ;
	bRemove  = _pPacketCache[_nHead].bRemove ;
	_pPacketCache[_nHead].pPacket	= NULL;
	_pPacketCache[_nHead].nPlayerId = -1;
	_pPacketCache[_nHead].bRemove	= false;
	++_nHead;
	if (_nHead >= _nPacketCacheCurrSize)  { _nHead = 0; }

	return true ;
	__LEAVE_FUNCTION
	return false;
}

bool Scene::removePacket(int nPlayerId)
{
	__ENTER_FUNCTION

	AutoMLock lock(_lockQ);
	int pos = _nHead ;
	for (int i = 0; i < _nPacketCacheCurrSize; i++)
	{
		if (_pPacketCache[pos].pPacket == NULL) break;
		if (_pPacketCache[pos].nPlayerId == nPlayerId) { _pPacketCache[pos].bRemove = true; }
		pos++;
		if (pos >= _nPacketCacheCurrSize) { pos = 0; }
	}

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool Scene::sendPacket(Packet* pPacket, Player_ID nPlayerId, bool bRemove)
{
	__ENTER_FUNCTION

	AutoMLock lock(_lockQ);
	//缓冲区满
	if (_pPacketCache[_nTail].pPacket != NULL)
	{
		bool ret = resizeCache(); 
		Assert(ret);
	}
	_pPacketCache[_nTail].pPacket = pPacket;
	_pPacketCache[_nTail].nPlayerId = nPlayerId;
	_pPacketCache[_nTail].bRemove = bRemove;
	++_nTail;
	if (_nTail >= _nPacketCacheCurrSize) { _nTail = 0; }

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool Scene::resizeCache( )
{
	__ENTER_FUNCTION

	//一定有数据
	Assert(_pPacketCache[_nTail].pPacket != NULL);
	PacketCache* pNew = new PacketCache[_nPacketCacheCurrSize + kPacketCacheGrow];
	if (pNew == NULL) { return false; }
	if (_nHead < _nTail)
	{
		memcpy(pNew, &(_pPacketCache[_nHead]), sizeof(PacketCache) * (_nTail - _nHead) );
	}
	else if (_nHead >= _nTail)
	{
		memcpy(pNew, &(_pPacketCache[_nHead]), sizeof(PacketCache) * (_nPacketCacheCurrSize - _nHead));
		memcpy(&pNew[_nPacketCacheCurrSize - _nHead], _pPacketCache, sizeof(PacketCache) * _nTail );
	}
	memset( _pPacketCache, 0, sizeof(PacketCache) * _nPacketCacheCurrSize ); //只是为了不让m_PacketQue中的pPacket被delete掉
	SAFE_DELETE_ARRAY(_pPacketCache) ;
	_pPacketCache = pNew;
	_nHead = 0;
	_nTail = _nPacketCacheCurrSize;
	_nPacketCacheCurrSize = _nPacketCacheCurrSize + kPacketCacheGrow ;

	return true;
	__LEAVE_FUNCTION
	return false ;
}

bool Scene::processCacheCmds()
{
	__ENTER_FUNCTION
	Packet* pPacket = NULL ;
	Player_ID nPlayerId = -1;
	bool bRemove = false;
	for (int i = 0; i < _nPacketCacheCurrSize; i++)
	{
		bool ret = recvPacket(pPacket, nPlayerId, bRemove);
		if (!ret) { return true; }
		Assert(pPacket);
		if (bRemove)
		{
			ret = PacketFactoryMgr::getSinglePtr()->removePacket(pPacket) ;
			Assert(ret);
			continue;
		}
		bool bNeedRemove = true;
		if (nPlayerId == -1)
		{
			__MYTRY
			{
				pPacket->execute(NULL);
			}
			__MYCATCH
			{
				MyExceptionLog();
			}
		}
		else
		{
			__MYTRY
			{
				GamePlayer* pPlayer = PlayerPool<GamePlayer>::getSinglePtr()->getPlayer(nPlayerId);
				if (pPlayer)
				{
					int nRet = PACKET_EXE_ERROR ;
					__MYTRY
					{
						nRet = pPacket->execute(pPlayer) ;
					}
					__MYCATCH
					{
						MyExceptionLog();
						nRet = PACKET_EXE_ERROR;
					}
					if (nRet == PACKET_EXE_ERROR)
					{
						_pScenePlayerMgr->kickPlayer(pPlayer);
					}
					else if (nRet == PACKET_EXE_BREAK);
					else if (nRet == PACKET_EXE_CONTINUE);
					else if (nRet == PACKET_EXE_NOTREMOVE)
					{
						bNeedRemove = false;
					}
				}
			}
			__MYCATCH
			{
				MyExceptionLog();
			}
		}
		//回收消息
		if (bNeedRemove)
		{
			PacketFactoryMgr::getSinglePtr()->removePacket(pPacket);
		}
	}
	return true;
	__LEAVE_FUNCTION
	return false;
}

bool	Scene::heartBeat(uint uTime)
{
	__ENTER_FUNCTION

	__MYTRY
	{
		TimeSystem::getSinglePtr()->getTimeTM(); //设定此桢的时间
		uTime = TimeSystem::getSinglePtr()->getRunTime(); 
		bool ret = _pScenePlayerMgr->heartBeat(uTime);
		Assert(ret);
	}
	__MYCATCH
	{
		MyExceptionLog();
	}

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool Scene::broadCast(Packet* pPacket)
{
	__ENTER_FUNCTION

	int nPlayerCount = getPlayerCount();
	for (int i = 0; i < nPlayerCount; ++i)
	{
		GamePlayer* pPlayer = _pScenePlayerMgr->getObjByIndex(i);
		if (pPlayer) { 
			pPlayer->sendPacket(pPacket); 
		}
	}
	return true;
	__LEAVE_FUNCTION
	return false;
}

//WX_IMPLEMENT_SINGLEON(SceneMgr);
//SceneMgr::SceneMgr()
//{
//	for (int i = 0; i < kMaxSceneCount; ++i)
//	{
//		_pScenes[i] = NULL;
//	}
//	_nSceneCount = 0;
//}
//
//SceneMgr::~SceneMgr()
//{
//	__ENTER_FUNCTION
//	for (int  i = 0; i < _nSceneCount; ++i)
//	{
//		SAFE_DELETE(_pScenes[i]);
//	}
//	_nSceneCount = 0;
//	__LEAVE_FUNCTION
//}
//
//bool	 SceneMgr::init()
//{
//	int count = kMaxSceneCount; //read from config
//	for (int i = 0; i < count; ++i)
//	{
//		Scene* pScene = new Scene(i); Assert(pScene);
//		bool ret = pScene->init(i); Assert(ret);
//		ret = addScene(pScene); Assert(ret);
//	}
//	return true;
//}
//
//Scene* SceneMgr::getSceneByIndex(int nIndex) const
//{
//	if (nIndex < 0 || nIndex >= _nSceneCount) { return NULL; }
//	return _pScenes[nIndex];
//}
//
//Scene* SceneMgr::getScene(int nSceneId) const
//{
//	for (int i = 0; i < _nSceneCount; ++i)
//	{
//		if (_pScenes[i] && _pScenes[i]->getSceneId() == nSceneId)
//			return _pScenes[i];
//	}
//	return NULL;
//}
//
//bool SceneMgr::addScene(Scene* pScene)
//{
//	Assert(pScene);
//	if (_nSceneCount < kMaxSceneCount)
//	{
//		_pScenes[_nSceneCount++] = pScene;
//		return true;
//	}
//	return false;
//}
//
//int  SceneMgr::getMinloadScene() const
//{
//	__ENTER_FUNCTION
//
//	int minIndex = 0;
//	for (int i = 1; i < _nSceneCount; ++i)
//	{
//		if (_pScenes[i] && _pScenes[i]->getPlayerCount() < _pScenes[minIndex]->getPlayerCount())
//		{
//			minIndex = i;
//		}
//	}
//
//	return _pScenes[minIndex]->getSceneId();
//
//	__LEAVE_FUNCTION
//	return -1;
//}
//
