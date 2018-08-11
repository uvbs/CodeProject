#include "game_player_mgr.h"
#include "game_player.h"
#include "player_pool.h"
#include "maths.h"
#include "packet_factory_mgr.h"
#include <stdlib.h>
#include "time_system.h"
#include "socketdef.h"
#include "log.h"
#include <string.h>

const int DEFAULT_CLIENT_SEND_BUFFER_SIZE = 64 * 1024;
const int DEFAULT_CLIENT_RECV_BUFFER_SIZE = 64 * 1024;

WX_IMPLEMENT_SINGLEON(GamePlayerMgr);

GamePlayerMgr::GamePlayerMgr()
{
	_pPacketQ = new PacketCache[MAX_PACKET_CACHE_SIZE];
	MyAssert(_pPacketQ) ;
	_packetQSize = MAX_PACKET_CACHE_SIZE ;
	_head = 0 ;
	_tail = 0 ;
}

GamePlayerMgr::~GamePlayerMgr()
{
	SAFE_DELETE_ARRAY(_pPacketQ);
	SAFE_DELETE(_pSrvSocket) ;
}

bool GamePlayerMgr::init(int count, const char* host, int port)
{
	__ENTER_FUNCTION

	bool ret = ObjectMgr<GamePlayer>::init(count);
	MyAssert(ret);
	_pSrvSocket = new SocketServer(); 
	MyAssert(_pSrvSocket) ;
	ret =  _pSrvSocket->create(host, port);
	MyAssert(ret) ;
	ret = _pSrvSocket->bind() ;
	MyAssert(ret) ;
	ret = _pSrvSocket->listen(5) ;
	MyAssert(ret) ;
	ret = _pSrvSocket->setNonBlocking() ;
	MyAssert(ret) ;
	
	_srvSocketFd = _pSrvSocket->getFd();
	FD_SET(_srvSocketFd , &_readFds[EM_SELECT_BAK]);
	FD_SET(_srvSocketFd , &_writeFds[EM_SELECT_BAK]);
	FD_SET(_srvSocketFd , &_exceptFds[EM_SELECT_BAK]);
	_timeout[EM_SELECT_BAK].tv_sec = 0;
	_timeout[EM_SELECT_BAK].tv_usec = 0;
	_minFd = _maxFd = _srvSocketFd;

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool GamePlayerMgr::acceptNewConnection()
{
	__ENTER_FUNCTION

	bool ret = false;
	//从玩家池中找出一个空闲的玩家数据集
	GamePlayer* pClient = PlayerPool<GamePlayer>::getSinglePtr()->allocPlayer() ;
	if (pClient == NULL)
	{
		return false ;
	}
	Socket* pSocket = pClient->getSocket();
	MyAssert(pSocket);
	SOCK fd = INVALID_SOCKET;

	__MYTRY
	{
		//接受客户端接入Socket句柄
		ret = _pSrvSocket->accept(pSocket) ;
		if (!ret)
		{
			LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
				"GamePlayerManager AcceptNewConnection Accept error: error code = %d, error description = %s", 
				getSocketErrorCode(), getSocketErrorDescription()) ;
			goto EXCEPTION ;
		}
		fd = pSocket->getFd();
		if (fd == INVALID_SOCKET)
		{
			LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
				"GamePlayerManager AcceptNewConnection socket invalid") ;
			goto EXCEPTION ;
		}
		ret = pSocket->setNonBlocking() ;
		if (!ret)
		{
			goto EXCEPTION ;
		}
		ret = pSocket->setSendBufferSize(DEFAULT_CLIENT_SEND_BUFFER_SIZE);
		if (!ret)
		{
			goto EXCEPTION ;
		}
		ret = pSocket->setReceiveBufferSize(DEFAULT_CLIENT_RECV_BUFFER_SIZE);
		if (!ret)
		{
			goto EXCEPTION ;
		}
		ret = pSocket->setNonDelay() ;
		if (!ret)
		{	
			goto EXCEPTION ;
		}
		if (pSocket->isSockError())
		{
			goto EXCEPTION ;
		}
		ret = pSocket->setLinger(0) ;
		if (!ret)
		{
			goto EXCEPTION ;
		}
		//初始化基本玩家信息
		ret = pClient->init() ;
		if (!ret)
		{
			LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
				"GamePlayerManager AcceptNewConnection client init error.") ;
			goto EXCEPTION ;
		}
		//设置当前客户端连接的状态
		pClient->setStatus(EM_PLAYER_STATUS_CONNECT) ;
		ret = addPlayer(pClient) ;
		MyAssert(ret);
	}
	__MYCATCH
	{
		goto EXCEPTION ;
	}

	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, 
		"GamePlayerManager AcceptNewConnection(SOCKET=%d, IP=%s) PlayerID=%d ...OK", 
		fd, pSocket->getHost(), pClient->getPlayerID()) ;
	return true ;

EXCEPTION:
	ret = PlayerPool<GamePlayer>::getSinglePtr()->recyclePlayer(pClient->getPlayerID()) ;
	return false ;
	__LEAVE_FUNCTION
	return false ;
}

void	GamePlayerMgr::getMaxMinFD(SOCK removedFD,SOCK& maxFD,SOCK& minFD)
{
	__ENTER_FUNCTION

	maxFD = minFD = _srvSocketFd;
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
	__LEAVE_FUNCTION
}

bool GamePlayerMgr::processInputs( )
{
	__ENTER_FUNCTION

	if (_minFd == INVALID_SOCKET && _maxFd == INVALID_SOCKET)
	{
		return true ;
	}
	__MYTRY
	{
		//新连接接入
		if (FD_ISSET(_srvSocketFd, &_readFds[EM_SELECT_USE]))
		{
			for (int i = 0; i < 32; i++)
			{
				if (!acceptNewConnection())
				{
					break;
				}
			}
		}
	}
	__MYCATCH
	{}

	return PlayerMgr::processInputs();

	__LEAVE_FUNCTION
	return false ;
}

bool GamePlayerMgr::processCmds()
{
	__ENTER_FUNCTION

	bool ret = false ;
	if (_minFd == INVALID_SOCKET && _maxFd == INVALID_SOCKET)
	{ 
		return true ;
	}
	int playerCount = getObjNum();
	for (int i = playerCount - 1; i >= 0 ; --i)
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
		if (s == _srvSocketFd)
		{
			continue;
		}
		if (pSocket->isSockError())
		{
			kickPlayer(pPlayer);
		}
		else
		{
			__MYTRY
			{
				ret = pPlayer->processCmd();
				if (!ret)
				{
					kickPlayer(pPlayer);
				}
			}
			__MYCATCH
			{
				kickPlayer(pPlayer);
			}
		}
	}
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool GamePlayerMgr::receivePacket(Packet*& pPacket, int& playerId, bool& isRemove)
{
	AutoMLock autolock(_lock);
	//没有消息
	if (_pPacketQ[_head].pPacket == NULL)
	{
		return false ;
	}
	pPacket = _pPacketQ[_head].pPacket;
	playerId = _pPacketQ[_head].nPlayerId;
	isRemove = _pPacketQ[_head].bRemove;

	_pPacketQ[_head].pPacket = NULL ;
	_pPacketQ[_head].nPlayerId = -1;
	_pPacketQ[_head].bRemove	= false ;

	++_head;
	if (_head >= _packetQSize) 
	{
		_head = 0 ;
	}
	return true ;
}

bool GamePlayerMgr::removePacket(int playerId)
{
	AutoMLock autolock(_lock);
	int j = _head ;
	for (int i = 0; i < _packetQSize; i++)
	{
		if (_pPacketQ[j].pPacket == NULL)
		{
			break ;
		}
		if (_pPacketQ[j].nPlayerId == playerId)
		{
			_pPacketQ[j].bRemove = true ;
		}
		j++ ;
		if (j >= _packetQSize) 
		{
			j = 0 ;
		}
	}
	return true ;
}

bool GamePlayerMgr::sendPacket(Packet* pPacket, int playerId, bool isRemove)
{
	AutoMLock autolock(_lock);
	//缓冲区满
	if (_pPacketQ[_tail].pPacket != NULL)
	{
		bool ret = resizePacketCache( ) ;
		MyAssert(ret);
	}
	_pPacketQ[_tail].pPacket = pPacket ;
	_pPacketQ[_tail].nPlayerId = playerId ;
	_pPacketQ[_tail].bRemove = isRemove ;
	++_tail;
	if (_tail >= _packetQSize)
	{
		_tail = 0;
	}
	return true ;
}

bool GamePlayerMgr::resizePacketCache( )
{
	__ENTER_FUNCTION

	//一定有数据
	if (_pPacketQ[_tail].pPacket == NULL)
	{
		return false;
	}
	int newCacheSize = _packetQSize + MAX_PACKET_CACHE_SIZE;
	PacketCache* pNewCache = new PacketCache[newCacheSize];
	if (pNewCache == NULL)
	{
		return false ;
	}
	//环状队列
	if (_head < _tail)
	{
		memcpy(pNewCache, &(_pPacketQ[_head]), sizeof(PacketCache)*(_tail - _head));
	}
	else if (_head >= _tail)
	{
		memcpy(pNewCache, &(_pPacketQ[_head]), sizeof(PacketCache)*(_packetQSize - _head));
		memcpy(&pNewCache[_packetQSize - _head], _pPacketQ, sizeof(PacketCache)*_tail);
	}
	//memset( _pPacketQ, 0, sizeof(PacketCache)*_packetQSize );
	SAFE_DELETE_ARRAY(_pPacketQ) ;
	_pPacketQ = pNewCache ;

	_head = 0 ;
	_tail = _packetQSize ;
	_packetQSize = newCacheSize;

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool GamePlayerMgr::processCacheCmds()
{
	__ENTER_FUNCTION

	Packet* pPacket = NULL ;
	int playerId = -1;
	bool isRemove = false;
	for (int i = 0; i < _packetQSize; i++)
	{
		bool ret = receivePacket(pPacket, playerId, isRemove) ;
		if (!ret)
		{
			return true;
		}
		MyAssert(pPacket) ;
		if (isRemove)
		{
			ret = PacketFactoryMgr::getSinglePtr()->removePacket(pPacket) ;
			continue;
		}
		bool isNeedRemove = true ;
		if (playerId == -1)
		{
			__MYTRY
			{
				pPacket->execute(NULL) ;
			}
			__MYCATCH
			{}
		}
		else
		{
			__MYTRY
			{
				GamePlayer* pPlayer = PlayerPool<GamePlayer>::getSinglePtr()->getPlayer(playerId) ;
				//这种情况存在
				if (pPlayer != NULL)
				{
					int err = PACKET_EXE_ERROR ;
					__MYTRY
					{
						err = pPacket->execute(pPlayer) ;
					}
					__MYCATCH
					{
						err = PACKET_EXE_ERROR ;
					}

					if (err == PACKET_EXE_ERROR)
					{
						kickPlayer(pPlayer) ;
					}
					else if (err == PACKET_EXE_BREAK)
					{
					}
					else if (err == PACKET_EXE_CONTINUE)
					{
					}
					else if (err == PACKET_EXE_NOTREMOVE)
					{
						isNeedRemove = false ;
					}
				}
			}
			__MYCATCH
			{}
		}

		//回收消息
		if (isNeedRemove)
		{
			PacketFactoryMgr::getSinglePtr()->removePacket(pPacket) ;
		}
	}
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool GamePlayerMgr::kickPlayer(GamePlayer* pPlayer)
{
	if (pPlayer == NULL)
	{
		return false;
	}
	int playerId = pPlayer->getPlayerID();
	SOCK fd = pPlayer->getSocket()->getFd();
	char* host = pPlayer->getSocket()->getHost();
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, 
		"GamePlayerMgr KickPlayer(SOCKET=%d, IP=%s), PlayerID=%d", fd, host, playerId) ;

	removePacket(playerId);
	removePlayer(playerId);

	//清除在池中的信息
	bool bRet = pPlayer->free();
	MyAssert(bRet) ;
	return true ;
}

void GamePlayerMgr::relive()
{
   __ENTER_FUNCTION

   MyAssert(_pSrvSocket);
   MyAssert(_srvSocketFd >= 0);

   kickAllPlayers();

   FD_SET(_srvSocketFd , &_readFds[EM_SELECT_BAK]);
   FD_SET(_srvSocketFd , &_writeFds[EM_SELECT_BAK]);
   FD_SET(_srvSocketFd , &_exceptFds[EM_SELECT_BAK]);
   _minFd = _maxFd = _srvSocketFd;
   _head = _tail = 0 ;

   LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "GamePlayerManager Relive.");
   
   __LEAVE_FUNCTION
}

