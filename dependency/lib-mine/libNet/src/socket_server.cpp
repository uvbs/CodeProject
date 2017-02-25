#include "socket_server.h"
#include "assert.h"
#include "string_util.h"

#if defined(_LINUX64)
#include <string.h>          // memset
#endif

namespace tcp_net {

SocketServer::SocketServer() 
{
	memset(_hostIP, 0, MAX_IP_SIZE);
	_hostPort = 0;
}

SocketServer::~SocketServer() 
{
	memset(_hostIP, 0, MAX_IP_SIZE);
	_hostPort = 0;
}

bool SocketServer::create( const char* host , int port)
{
	__ENTER_FUNCTION
	bool bRet = Socket::create();
	if (!bRet) { return false; }
	str_util::strncpy(_hostIP, host, MAX_IP_SIZE - 1);
	_hostPort = port;
	bRet = reuseAddr();
	if (!bRet) { return false; }
	return true;
	__LEAVE_FUNCTION
	return false ;
}

//关闭连接
void SocketServer::close()
{
	__ENTER_FUNCTION
	Socket::close();
	memset(_hostIP, 0, MAX_IP_SIZE);
	_hostPort = 0;
	__LEAVE_FUNCTION
}

//绑定
bool SocketServer::bind()
{
	__ENTER_FUNCTION
	_sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	_sockAddr.sin_port = htons(_hostPort);
	bool result = SocketAPI::socket_bind(_sockfd, (const struct sockaddr*)&_sockAddr, sizeof(_sockAddr));
	return result;
	__LEAVE_FUNCTION
	return false;
}

//监听
bool SocketServer::listen(int backlog)
{
	__ENTER_FUNCTION
	return SocketAPI::socket_listen(_sockfd, backlog);
	__LEAVE_FUNCTION
	return false;	
}

//重用地址
bool SocketServer::reuseAddr(bool on)
{
	__ENTER_FUNCTION 
	int opt = on == true ? 1 : 0;
	return SocketAPI::socket_setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	__LEAVE_FUNCTION
	return false;
}

//接受连接
SOCK SocketServer::accept(struct sockaddr* addr, int* addrlen)
{
	__ENTER_FUNCTION
	return SocketAPI::socket_accept(_sockfd, addr, addrlen);
	__LEAVE_FUNCTION
	return INVALID_SOCKET ;
}

bool SocketServer::accept(Socket* socket)
{
	__ENTER_FUNCTION
	int addrlen = sizeof(SOCKADDR_IN);
	socket->close();
	SOCK s = accept((struct sockaddr *)(socket->getSocketAddr()), &addrlen);
	if (s == INVALID_SOCKET) { return false ; }
	socket->setFd(s);
	return true;
	__LEAVE_FUNCTION
	return false;
}

} //namespace tcpnet
