#include "player_mgr.h"
#include "game_player.h"
#include "player_pool.h"
#include "time_system.h"
#include "output_stream.h"
#include "socket_api.h"
#include "log.h"

using namespace tcp_net;

PlayerMgr::PlayerMgr()
{
	clearSocket();
}

//����
void PlayerMgr::cleanUp()
{
	//���obj
	ObjectMgr<GamePlayer>::cleanUp() ;
	clearSocket();
}

void PlayerMgr::clearSocket()
{
	//���fd
	for (int i = EM_SELECT_BAK; i <= EM_SELECT_USE; ++i)
	{
		_timeout[i].tv_sec = 0;
		_timeout[i].tv_usec = 0;
		FD_ZERO(&_readFds[i]);
		FD_ZERO(&_writeFds[i]);
		FD_ZERO(&_exceptFds[i]) ;
	}
	_minFd = INVALID_SOCKET ;
	_maxFd = INVALID_SOCKET ;
}

//�������
bool PlayerMgr::select()
{
	__ENTER_FUNCTION

	bool ret = false ;
	if ( _maxFd == INVALID_SOCKET && _minFd == INVALID_SOCKET)
	{
		return true ;
	}
	//�ӱ���ȡ��ʹ��fd
	_timeout[EM_SELECT_USE].tv_sec = _timeout[EM_SELECT_BAK].tv_sec;
	_timeout[EM_SELECT_USE].tv_usec = _timeout[EM_SELECT_BAK].tv_usec;
	_readFds[EM_SELECT_USE] = _readFds[EM_SELECT_BAK];
	_writeFds[EM_SELECT_USE] = _writeFds[EM_SELECT_BAK];
	_exceptFds[EM_SELECT_USE] = _exceptFds[EM_SELECT_BAK];

	__MYTRY
	{
		int err = tcp_net::SocketAPI::socket_select((int)_maxFd+1 , 
			&_readFds[EM_SELECT_USE] , 
			&_writeFds[EM_SELECT_USE] , 
			&_exceptFds[EM_SELECT_USE] , 
			&_timeout[EM_SELECT_USE] ) ;
		if (err == SOCKET_ERROR)
		{
			LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, "ScenePlayerManager select error: error code = %d, error description = %s", 
				getSocketErrorCode(), getSocketErrorDescription()) ;
			return false;
		}
	} 
	__MYCATCH
	{}

	return true ;
	__LEAVE_FUNCTION
	return false;
}

//���ݽ���
bool PlayerMgr::processInputs()
{
	__ENTER_FUNCTION

	bool ret = false ;
	//no player
	if (_minFd == INVALID_SOCKET && _maxFd == INVALID_SOCKET)
	{
		return true ;
	}
	//���ݶ�ȡ
	int playerCount = getObjNum() ;
	for (int i = playerCount - 1; i >= 0; --i)
	{
		GamePlayer* pPlayer = getObjByIndex(i) ;
		if (pPlayer == NULL)
		{
			continue;
		}
		if (!pPlayer->isValid())
		{
			removeSocket(pPlayer);
			removeObj(pPlayer);
			continue;
		}
		Socket* pSocket = pPlayer->getSocket();
		if (pSocket == NULL)
		{
			continue;
		}
		SOCK s = pSocket->getFd();
		MyAssert(s != INVALID_SOCKET) ;
		if (FD_ISSET(s, &_readFds[EM_SELECT_USE]))
		{
			//���ӳ��ִ���
			if (pSocket->isSockError())
			{
				ret = kickPlayer(pPlayer) ;
				MyAssert(ret);
			}
			else
			{
				__MYTRY
				{
					ret = pPlayer->processInput( ) ;
					if (!ret)
					{
						throw(1);
					}
				}
				__MYCATCH
				{
					ret = kickPlayer( pPlayer ) ;
					MyAssert(ret);
				}
			}
		}
	}
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

//���ݷ���
bool PlayerMgr::processOutputs( )
{
	__ENTER_FUNCTION

	bool ret = false ;
	//no player
	if (_minFd == INVALID_SOCKET && _maxFd == INVALID_SOCKET)
	{ 
		return true ;
	}
	//���ݷ���
	int playerCount = getObjNum() ;
	for (int i = playerCount - 1; i >= 0; --i)
	{
		GamePlayer* pPlayer = getObjByIndex(i) ;
		if (pPlayer == NULL)
		{
			continue;
		}
		if (!pPlayer->isValid())
		{
			removeSocket(pPlayer);
			removeObj(pPlayer);
			continue;
		}
		Socket* pSocket = pPlayer->getSocket();
		if (NULL == pSocket)
		{
			continue;
		}
		SOCK s = pSocket->getFd();
		MyAssert(s != INVALID_SOCKET) ;
		if (FD_ISSET(s, &_writeFds[EM_SELECT_USE]))
		{
			//���ӳ��ִ���
			if (pSocket->isSockError())
			{
				ret = kickPlayer( pPlayer ) ;
				MyAssert(ret);	//���ش���,��������ִ��,�������Ҳ����������
			}
			else
			{
				__MYTRY
				{
					//int beforeLengh = pPlayer->getSocketStream()->getOutstream()->getLength();
					ret = pPlayer->processOutput();
					//int afterLength = pPlayer->getSocketStream()->getOutstream()->getLength();
					//int sendLength = beforeLengh - afterLength;
					if (!ret)
					{
						throw(1);
					}
				}
				__MYCATCH
				{
					ret = kickPlayer( pPlayer ) ;
					MyAssert(ret);
				}
			}
		}
	}

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

//�쳣���Ӵ���
bool PlayerMgr::processExceptions( )
{
	__ENTER_FUNCTION

	bool ret = false ;
	//no player
	if (_minFd == INVALID_SOCKET && _maxFd == INVALID_SOCKET)
	{ 
		return true ;
	}
	int playerCount = getObjNum() ;
	for (int i = playerCount - 1; i >= 0; --i)
	{
		GamePlayer* pPlayer = getObjByIndex(i);
		if (NULL == pPlayer)
		{
			continue;
		}
		if (!pPlayer->isValid())
		{
			removeSocket(pPlayer);
			removeObj(pPlayer);
			continue;
		}
		Socket* pSocket = pPlayer->getSocket();
		if (NULL == pSocket)
		{
			continue;
		}
		SOCK s = pSocket->getFd();
		MyAssert(s != INVALID_SOCKET) ;
		if (FD_ISSET(s, &_exceptFds[EM_SELECT_USE]))
		{
			ret = kickPlayer( pPlayer ) ;
			MyAssert(ret);
		}
	}
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

//��Ϣִ��
bool PlayerMgr::processCmds( )
{
	__ENTER_FUNCTION

	bool bRet = false ;
	if (_minFd == INVALID_SOCKET && _maxFd == INVALID_SOCKET)
	{
		return true ;
	}

	int playerCount = getObjNum() ;
	for (int i = playerCount - 1; i >= 0; --i)
	{
		GamePlayer* pPlayer = getObjByIndex(i);
		if (NULL == pPlayer)
		{
			continue;
		}
		if (!pPlayer->isValid())
		{
			removeSocket(pPlayer);
			removeObj(pPlayer);
			continue;
		}
		Socket* pSocket = pPlayer->getSocket();
		if (NULL == pSocket)
		{
			continue;
		}
		SOCK s = pSocket->getFd();
		MyAssert(s != INVALID_SOCKET) ;

		//���ӳ��ִ���
		if (pSocket->isSockError())
		{
			bRet = kickPlayer( pPlayer ) ;
			MyAssert(bRet);
		}
		else
		{
			__MYTRY
			{
				bRet = pPlayer->processCmd( ) ;
				if (!bRet)
				{
					throw(1);
				}
			}
			__MYCATCH
			{
				bRet = kickPlayer( pPlayer ) ;
				MyAssert(bRet);
			}
		}
	}

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

//�߼�
bool PlayerMgr::heartBeat( uint uTime )
{
	__ENTER_FUNCTION

	//���´����ʱ��
	TimeSystem::getSinglePtr()->tick();
	uTime = TimeSystem::getSinglePtr()->getRunTime() ;
	for( int i=0;i<getObjNum();++i)
	{
		GamePlayer* pPlayer = getObjByIndex(i);
		if (NULL == pPlayer)
		{
			continue;
		}
		__MYTRY
		{
			bool bRet = pPlayer->heartBeat(uTime,1 ) ;
			if (!bRet)
			{
			  kickPlayer( pPlayer );
			}
		}
		__MYCATCH
		{}
	}

	return true ;
	__LEAVE_FUNCTION
	return false ;
}
//��Player���ݼ���ϵͳ��
bool PlayerMgr::addPlayer(GamePlayer* pPlayer)
{
	__ENTER_FUNCTION
	//�Ѿ������ܹ����������������
	if (getObjNum() >= getCapacity())
	{
		return false;
	}
	Socket* pSocket = pPlayer->getSocket();
	if (pSocket == NULL)
	{
		return false;
	}
	SOCK fd = pSocket->getFd();
	MyAssert(fd != INVALID_SOCKET) ;
	if (FD_ISSET(fd, &_readFds[EM_SELECT_BAK]))
	{
		return false ;
	}
	bool ret = addObj(pPlayer) ;
	if( !ret )
	{
		return false ;
	}
	_minFd = Min(fd , _minFd);
	_maxFd = Max(fd , _maxFd);
	FD_SET(fd , &_readFds[EM_SELECT_BAK]);
	FD_SET(fd , &_writeFds[EM_SELECT_BAK]);
	FD_SET(fd , &_exceptFds[EM_SELECT_BAK]);

    return true ;
    __LEAVE_FUNCTION
	return false ;
}

//ɾ��һ����ң������������п���ת�Ƶ���ĳ�����
bool PlayerMgr::removePlayer(int playerId)
{
	GamePlayer* pGamePlayer = PlayerPool<GamePlayer>::getSinglePtr()->getPlayer(playerId) ;
	if (NULL == pGamePlayer)
	{
		return false;
	}
	removeSocket(pGamePlayer);
	removeObj(pGamePlayer);
	return true ;
}

bool PlayerMgr::removeSocket(GamePlayer* pPlayer)
{
	__ENTER_FUNCTION

	if (NULL== pPlayer)
	{
		return false;
	}
	Socket* pSocket = pPlayer->getSocket();
	if (NULL == pSocket)
	{
		return true;
	}
	SOCK fd = pSocket->getFd();
	if (INVALID_SOCKET== fd)
	{
		return true;
	}
	if (fd == _minFd || fd == _maxFd)
	{
		getMaxMinFD(fd, _maxFd, _minFd);
	}
	for (int i = EM_SELECT_BAK; i <= EM_SELECT_USE; ++i)
	{
		FD_CLR(fd , &_readFds[i]);
		FD_CLR(fd , &_writeFds[i]);
		FD_CLR(fd , &_exceptFds[i]);
	}
	return true;
	__LEAVE_FUNCTION
	return false;
}

//������player��Ϣ����,���Ͽ�����
void PlayerMgr::kickAllPlayers( )
{
	__ENTER_FUNCTION

	//AutoLock autolock(m_Lock);
	int playerCount = getObjNum() ;
	for (int i = playerCount - 1; i >= 0 ; --i)
	{
		GamePlayer* pPlayer = getObjByIndex(i);
		if (pPlayer == NULL)
		{
			continue;
		}
		kickPlayer(pPlayer) ;
	}
	cleanUp();

	__LEAVE_FUNCTION
}

bool PlayerMgr::broadCast(Packet* pPacket)
{
	__ENTER_FUNCTION

	for (int i = 0; i< getObjNum(); ++i)
	{
		GamePlayer* pPlayer = getObjByIndex(i);
		if (!pPlayer 
			|| pPlayer->getStatus() != EM_PLAYER_STATUS_NORMAL)
		{
			continue;
		}
		pPlayer->sendPacket(pPacket);
	}

	return true;
	__LEAVE_FUNCTION
	return false;
}
