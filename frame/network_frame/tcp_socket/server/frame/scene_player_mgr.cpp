#include "scene_player_mgr.h"
#include "player_pool.h"
#include <stdlib.h>
#include "time_system.h"
#include "packet_factory_mgr.h"
#include "log.h"

//初始化
bool	ScenePlayerMgr::init(int count)
{
	return ObjectMgr<GamePlayer>::init(count);
}

void	ScenePlayerMgr::getMaxMinFD(SOCK removedFD,SOCK& maxFD,SOCK& minFD)
{
    maxFD = minFD = INVALID_SOCKET;
    for (int i = 0; i < getObjNum(); i++)
    {
        GamePlayer* pPlayer = getObjByIndex(i);
        if (NULL == pPlayer)
        {
            continue;
        }
        Socket* pSocket = pPlayer->getSocket();
        if (NULL == pSocket)
        {
            continue;
        }
        SOCK fd = pSocket->getFd();
        if (removedFD == fd)
        {
            continue;
        }
        if (maxFD < fd)
        {
            maxFD = fd ;
        }
        if (minFD > fd)
        {
            minFD = fd;
        }		
    }
}

bool ScenePlayerMgr::kickPlayer(GamePlayer* pPlayer)
{
	__ENTER_FUNCTION
	MyAssert(pPlayer);
	MyAssert(_pScene);
    bool bRet = false;

	//清除Socket信息
	//int sceneId = _pScene->getSceneId();
	int playerId = pPlayer->getPlayerID();
	SOCK fd = pPlayer->getSocket()->getFd();
	char* host = pPlayer->getSocket()->getHost();
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, 
		"ScenePlayerMgr KickPlayer(SOCKET=%d, IP=%s), PlayerID=%d", fd, host, playerId) ;

	//_pScene->removePacket(playerId) ;
	//char* strIP = pPlayer->GetSocket()->GetIp();
	bRet = removePlayer(playerId) ;
	MyAssert(bRet);

	//清除在池中的信息
	bRet = pPlayer->free();
	MyAssert(bRet) ;

	return true ;
	__LEAVE_FUNCTION
	return false ;
}
