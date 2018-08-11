#include "robot.h"
#include "myassert.h"
#include "macrodef.h"
#include "string_util.h"
#include "socket_client.h"
#include "socket_api.h"
#include "log.h"
#include "CSAuthorizePacket.h"
#include "CSHeartBeatPacket.h"

const char* const g_Host = "127.0.0.1";
const int g_Port = 8888;

Robot::Robot(int userid) : Player(true)
{
	__ENTER_FUNCTION
	
	cleanUp();
	str_util::int32ToStr(userid, _userId, 10);
	_status = EM_PLAYER_STATUS_INVALID;

	__LEAVE_FUNCTION
}

Robot::~Robot()
{
	cleanUp();
	_bConnected = false;
}

bool Robot::init()
{
	__ENTER_FUNCTION
	SocketClient* pSocket = static_cast<SocketClient*>(getSocket());
	MyAssert(pSocket);
	bool bRet = pSocket->create(g_Host, g_Port); //read from config
	MyAssert(bRet);
	if (!isValid()) { 
		MyAssert(0);
	}
	_status = EM_PLAYER_STATUS_IDLE;
	return true;
	__LEAVE_FUNCTION
	return false;
}

void  Robot::cleanUp()
{
	for (int i = EM_SELECT_BAK; i< EM_SELECT_MAX; ++i)
	{
		FD_ZERO(&_readFds[i]);
		FD_ZERO(&_writeFds[i]);
		FD_ZERO(&_exceptFds[i]);
		_timeout[i].tv_sec = 0;
		_timeout[i].tv_usec = 0;
	}
	_minFd = _maxFd = INVALID_SOCKET;
	_fdSize = 0;
	_bConnected = false;
	Player::cleanUp();
}

bool Robot::select()
{
	__ENTER_FUNCTION
	//no player
	if (_minFd == INVALID_SOCKET && _maxFd == INVALID_SOCKET)
	{ 
		return true;
	}
	if (!isValid())
	{
		return false;
	}
	//从备份取出使用fd
	_timeout[EM_SELECT_USE].tv_sec  = _timeout[EM_SELECT_BAK].tv_sec;
	_timeout[EM_SELECT_USE].tv_usec = _timeout[EM_SELECT_BAK].tv_usec;
	_readFds[EM_SELECT_USE]   = _readFds[EM_SELECT_BAK];
	_writeFds[EM_SELECT_USE]  = _writeFds[EM_SELECT_BAK];
	_exceptFds[EM_SELECT_USE] = _exceptFds[EM_SELECT_BAK];
	__MYTRY 
	{
		int iRet = tcp_net::SocketAPI::socket_select(	(int)_maxFd + 1 , 
			&_readFds[EM_SELECT_USE], &_writeFds[EM_SELECT_USE], &_exceptFds[EM_SELECT_USE],
			&_timeout[EM_SELECT_USE]);
		if (iRet == SOCKET_ERROR)
		{
			LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
				"Robot select error: error code = %d, error description = %s", 
				getSocketErrorCode(), getSocketErrorDescription()) ;
			return false;
		}
	} 
	__MYCATCH
	{
		MyExceptionLog();
	}
	return true;
	__LEAVE_FUNCTION
	return false;
}

bool Robot::processInputs()
{
	__ENTER_FUNCTION
	bool ret = false;
	//no player
	if (_minFd == INVALID_SOCKET && _maxFd == INVALID_SOCKET)
	{ 
		return true;
	}
	Socket* pSocket = getSocket();
	if (!socket)
	{
		return false;
	}
	if (!isValid())
	{
		return false;
	}
	SOCK s = pSocket->getFd();
	if (FD_ISSET(s, &_readFds[EM_SELECT_USE]))
	{
		if (pSocket->isSockError())
		{
			ret = removeSocket();
			MyAssert(ret);
		}
		else
		{
			__MYTRY
			{
				ret = processInput(); //处理服务器发送过来的消息
				if (!ret)
				{
					sendCloseMsg();
					ret = removeSocket();
					MyAssert(ret);
					LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
						"RemoveSocket In ProcessInput() ...");
				}
			}
			__MYCATCH
			{
				MyExceptionLog();
				ret = removeSocket();
				MyAssert(ret);
			}
		}
	}

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool Robot::processOutputs( )
{
	__ENTER_FUNCTION
	bool ret = false;
	//no player
	if (_minFd == INVALID_SOCKET && _maxFd == INVALID_SOCKET)
	{ 
		return true;
	}
	Socket* pSocket = getSocket();
	if (!pSocket)
	{
		return false;
	}
	if (!isValid())
	{
		return false;
	}
	SOCK s = pSocket->getFd();
	if (FD_ISSET(s, &_writeFds[EM_SELECT_USE]))
	{
		if (pSocket->isSockError())
		{
			ret = removeSocket();
			MyAssert(ret);
		}
		else
		{
			__MYTRY
			{
				ret = processOutput();
				if (!ret)
				{
					ret = removeSocket();
					MyAssert(ret);
					LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
						"RemoveSocket In ProcessOutput() ...") ;
				}
			}
			__MYCATCH
			{
				MyExceptionLog();
				removeSocket();
			}
		}
	}

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool Robot::processExceptions( )
{
	__ENTER_FUNCTION
	//no player
	if (_minFd == INVALID_SOCKET && _maxFd == INVALID_SOCKET)
	{ 
		return true;
	}
	Socket* pSocket = getSocket();
	if (!pSocket)
	{
		return false;
	}
	if (!isValid())
	{
		return false;
	}
	SOCK s = pSocket->getFd();
	if (FD_ISSET(s, &_exceptFds[EM_SELECT_USE]))
	{
		bool ret = removeSocket();
		MyAssert(ret);
	}

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool Robot::processCmds( )
{
	__ENTER_FUNCTION
	bool bRet = false;
	//no player
	if (_minFd == INVALID_SOCKET && _maxFd == INVALID_SOCKET)
	{ 
		return true;
	}
	if (!isValid())
	{
		return false;
	}
	if (getSocket()->isSockError())
	{
		bRet = removeSocket();
		MyAssert(bRet);
	}
	else
	{
		__MYTRY
		{
			//这里出错也不处理
			bRet = processCmd();
		}
		__MYCATCH
		{
			MyExceptionLog();
		}
	}
	return true;
	__LEAVE_FUNCTION
	return false;
}

bool	Robot::connectServer()
{
	__ENTER_FUNCTION
	bool bRet = false;
	SocketClient* pSocket = static_cast<SocketClient*>(getSocket());
	MyAssert(pSocket);
	__MYTRY
	{
		if (!isValid())
		{
			bRet = pSocket->create(g_Host, g_Port);							
			if (!bRet)
			{
				goto EXCEPTION;
			}
		}
		bRet = pSocket->connect();							
		if (!bRet)
		{
			LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
				"Can' Connect Server: error code = %d, error description = %s", 
				getSocketErrorCode(), getSocketErrorDescription()) ;
			return false;
		}
		bRet = pSocket->setNonBlocking();
		if (!bRet)
		{
			goto EXCEPTION;
		}
		bRet = pSocket->setSendBufferSize(kSendBufferSize);
		if (!bRet)
		{	
			goto EXCEPTION;
		}
		bRet = pSocket->setReceiveBufferSize(kRecvBufferSize);
		if (!bRet)
		{
			goto EXCEPTION;
		}
		bRet = pSocket->setLinger(0);
		if (!bRet)
		{
			goto EXCEPTION;
		}
	}
	__MYCATCH
	{
		MyExceptionLog();
		goto EXCEPTION;
	}
	bRet = useSocket();
	if (!bRet)
	{
		goto EXCEPTION;
	}
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, 
		"ConnectServer IP = %s Port = %d ...OK ",  pSocket->getHost(), pSocket->getPort()) ;
	return true;

EXCEPTION:
	LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
		"ConnectServer IP = %s Port = %d ...Fail ",pSocket->getHost(), pSocket->getPort()) ;
	cleanUp();

	return false;
	__LEAVE_FUNCTION
	return false;
}

bool	Robot::useSocket()
{
	__ENTER_FUNCTION
	SocketClient* pSocket = static_cast<SocketClient*>(getSocket());
	MyAssert(pSocket);
	SOCK fd = pSocket->getFd();
	MyAssert(fd != INVALID_SOCKET);

	if (_fdSize >= FD_SETSIZE)
	{
		MyAssert(0);
	}
	FD_SET(fd , &_readFds[EM_SELECT_BAK]);
	FD_SET(fd , &_writeFds[EM_SELECT_BAK]);
	FD_SET(fd , &_exceptFds[EM_SELECT_BAK]);
	_fdSize++;
	_minFd = ((_minFd == INVALID_SOCKET) ? fd : Min(fd , _minFd));
	_maxFd = ((_maxFd == INVALID_SOCKET) ? fd : Max(fd,  _maxFd));

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool	Robot::removeSocket()
{
	__ENTER_FUNCTION
	SocketClient* pSocket = static_cast<SocketClient*>(getSocket());
	MyAssert(pSocket);
	SOCK fd = pSocket->getFd();
	MyAssert(fd != INVALID_SOCKET);
	MyAssert(_minFd != INVALID_SOCKET);
	MyAssert(_maxFd != INVALID_SOCKET);

	//_minFd = _maxFd = INVALID_SOCKET ;
	//FD_CLR(fd , &_readFds[EM_SELECT_BAK]);
	//FD_CLR(fd , &_readFds[EM_SELECT_USE]);
	//FD_CLR(fd , &_writeFds[EM_SELECT_BAK]);
	//FD_CLR(fd , &_writeFds[EM_SELECT_USE]);
	//FD_CLR(fd , &_exceptFds[EM_SELECT_BAK]);
	//FD_CLR(fd , &_exceptFds[EM_SELECT_USE]);
	//_fdSize--;
	//MyAssert(_fdSize>=0) ;

	cleanUp();

	_status = EM_PLAYER_STATUS_IDLE;

	LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, "RemoveSocket.") ;

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool	Robot::sendCloseMsg()
{
	//CMClosePacket packet;
	//SendPacket(&packet);
	return true;
}

bool	Robot::heartBeat(uint uTime)
{
	__ENTER_FUNCTION
	if (!_bConnected)
	{
		if (!_connectTimer.isSet()) {
			_connectTimer.beginTimer(2000, uTime);
		}
		else if (_connectTimer.isReach(uTime)) 
		{
			if (!connectServer())  { return true; }
			_bConnected = true;
			_connectTimer.endTimer();

			CSAuthorizePacket packet;
			packet.setUserId(getUserId());
			packet.setKey("robot");
			sendPacket(&packet); 
			_status = EM_PLAYER_STATUS_CONNECT;
		}
		return true;
	}
	testLogic(uTime);
	return true;
	__LEAVE_FUNCTION
	return false;
}

void Robot::testLogic(uint uTime)
{
	if (!_bConnected) { return; }
	//测试逻辑
	testTick(uTime);
	if (EM_PLAYER_STATUS_NORMAL == _status) {
		testChat(uTime);
	}
}

void	Robot::testTick(uint uTime)
{
	__ENTER_FUNCTION
	if (!_tickTimer.isSet())
	{
		_tickTimer.beginTimer(2 * 1000, uTime);
	}
	else if (_tickTimer.isReach(uTime))
	{
		CSHeartBeatPacket packet;
		sendPacket(&packet);
		_tickTimer.endTimer();
	}
	__LEAVE_FUNCTION
}

void	Robot::testChat(uint uTime)
{
	__ENTER_FUNCTION
	if (!_chatTimer.isSet())
	{
		_chatTimer.beginTimer(60 * 1000, uTime);
	}
	else if (_chatTimer.isReach(uTime))
	{
		//CGChatPacket packet;
		//packet.SetContent("Hello, Welcome to Dota World!");
		//SendPacket(&packet);
		_chatTimer.endTimer();
	}
	__LEAVE_FUNCTION
}

void Robot::testClose(uint uTime)
{
	__ENTER_FUNCTION
	if (! _closeTimer.isSet())
	{
		_closeTimer.beginTimer(180 * 1000, uTime);
	}
	else if (_closeTimer.isReach(uTime))
	{
		//CGAskClosePacket closepacket;
		//SendPacket(&closepacket);
		_closeTimer.endTimer();
	}
	__LEAVE_FUNCTION
}
