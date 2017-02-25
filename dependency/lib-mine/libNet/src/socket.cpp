#include "socket.h"
#include "socket_api.h"
#include "socketdef.h"
#include "assert.h"

#if defined(_LINUX64)
#include <string.h>          // memset
#include <arpa/inet.h>
#include <netinet/tcp.h>
#endif

namespace tcp_net {

int getSocketErrorCode()
{
	extern int g_socketErrorCode;
	return g_socketErrorCode;
}

char* getSocketErrorDescription()
{
	extern char g_socketError[SOCKET_ERROR_SIZE];
	return g_socketError;
}

Socket::Socket()
{
	_sockfd = INVALID_SOCKET;
	memset(&_sockAddr, 0, sizeof(SOCKADDR_IN)) ;
}

Socket::~Socket() 
{ 
	if (_sockfd != INVALID_SOCKET) { 
		close();
	}
}

bool Socket::create()
{
	__ENTER_FUNCTION
	_sockfd = SocketAPI::socket_create(AF_INET, SOCK_STREAM, 0);
	if (_sockfd == INVALID_SOCKET) { return false; }
	memset(&_sockAddr, 0, sizeof(_sockAddr));
	_sockAddr.sin_family = AF_INET;
	return true;
	__LEAVE_FUNCTION
	return false;
}

//关闭连接
void Socket::close()
{
	__ENTER_FUNCTION
	if (_sockfd != INVALID_SOCKET && !isSockError()) 
	{
		SocketAPI::socket_close(_sockfd);
		_sockfd = INVALID_SOCKET;
		memset(&_sockAddr, 0, sizeof(_sockAddr));
	}
	__LEAVE_FUNCTION
}

//设置关闭延时时间
bool Socket::setLinger (uint lingertime)
{
	__ENTER_FUNCTION
	struct linger ling;
	ling.l_onoff = lingertime > 0 ? 1 : 0;
	ling.l_linger = lingertime;
	return SocketAPI::socket_setsockopt(_sockfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
	__LEAVE_FUNCTION
	return false;	
}

//是否socket有错误
bool Socket::isSockError() const
{
	__ENTER_FUNCTION
	int error = 0;
	int length = (int)sizeof(error);
	bool bResult = SocketAPI::socket_getsockopt(_sockfd, SOL_SOCKET, SO_ERROR, &error, &length);
	return !bResult;
	__LEAVE_FUNCTION
	return true ;
}

//获得错误码
int Socket::getSockError() const
{
	int error = SOCKET_ERROR_UNKNOWN;
	__ENTER_FUNCTION
	int length = (int)sizeof(error);
	bool bResult = SocketAPI::socket_getsockopt(_sockfd, SOL_SOCKET, SO_ERROR, &error, &length);
	if (!bResult) {
		return SOCKET_ERROR_UNKNOWN;
	} else {			  
		return error;
	}
	__LEAVE_FUNCTION
	return error;
}

//设置阻塞或非阻塞模式
bool Socket::setNonBlocking(bool on)
{
	__ENTER_FUNCTION
	return SocketAPI::setsocketnonblocking(_sockfd, on);
	__LEAVE_FUNCTION
	return false ;
}

bool Socket::setNonDelay(bool on)
{
	__ENTER_FUNCTION
	int intOn = on;
	return SocketAPI::socket_setsockopt(_sockfd, IPPROTO_TCP, TCP_NODELAY, (const void*)&intOn, sizeof(intOn));
	__LEAVE_FUNCTION
	return false;
}

//获得接收缓冲区大小
int Socket::getReceiveBufferSize() const 
{ 
	__ENTER_FUNCTION
	int ReceiveBufferSize = 0;
	int size = (int)sizeof(ReceiveBufferSize);
	SocketAPI::socket_getsockopt(_sockfd, SOL_SOCKET, SO_RCVBUF, &ReceiveBufferSize, &size );
	return ReceiveBufferSize;
	__LEAVE_FUNCTION
	return 0 ;
}

//设置接收缓冲区大小
bool Socket::setReceiveBufferSize(int size)
{ 
	__ENTER_FUNCTION
	return (bool)(SocketAPI::socket_setsockopt(_sockfd, SOL_SOCKET, SO_RCVBUF , &size, sizeof(int))) ;
	__LEAVE_FUNCTION
	return false ;
}

//获得发送缓冲区大小
int Socket::getSendBufferSize() const 
{ 
	__ENTER_FUNCTION
	int SendBufferSize;
	int size = (int)sizeof(SendBufferSize);
	SocketAPI::socket_getsockopt(_sockfd, SOL_SOCKET, SO_SNDBUF, &SendBufferSize, &size);
	return SendBufferSize;
	__LEAVE_FUNCTION
	return 0 ;
}

//设置发送缓冲区大小
bool Socket::setSendBufferSize(int size)
{ 
	__ENTER_FUNCTION
	return (bool)(SocketAPI::socket_setsockopt(_sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(int)));
	__LEAVE_FUNCTION
	return false;
}

//发送数据
int Socket::send(const void* buf, int length, int flags)
{
	__ENTER_FUNCTION
	return SocketAPI::socket_send(_sockfd, buf, length, flags);
	__LEAVE_FUNCTION
	return 0;
}

//接收数据
int Socket::receive(void* buf, int length, int flags)
{
	__ENTER_FUNCTION
	return SocketAPI::socket_recv(_sockfd, buf, length, flags);
	__LEAVE_FUNCTION
	return 0 ;
}

//现有数据
uint Socket::available() const
{
	__ENTER_FUNCTION
	return SocketAPI::socket_available(_sockfd);
	__LEAVE_FUNCTION
	return 0 ;
}

} //namespace tcpnet
