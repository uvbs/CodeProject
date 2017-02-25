#include "socket_client.h"
#include "assert.h"
#include "string_util.h"
#include "socket_api.h"

#if defined(_LINUX64)
#include <string.h>          // memset
#include <arpa/inet.h>   //inet_addr,inet_aton
#endif

namespace tcp_net {

SocketClient::SocketClient() 
{
	memset(_hostIP, 0, MAX_IP_SIZE) ;
	_hostPort = 0;
}

SocketClient::~SocketClient() 
{
	memset(_hostIP, 0, MAX_IP_SIZE);
	_hostPort = 0;
}

bool SocketClient::create(const char* host , uint port)
{
	__ENTER_FUNCTION
	bool bRet = Socket::create();
	if (!bRet) { return false; }
	str_util::strncpy(_hostIP, host, MAX_IP_SIZE - 1);
	_hostPort = port;
	memset(&_sockAddr, 0, sizeof(_sockAddr));
	_sockAddr.sin_family = AF_INET;
	return true;
	__LEAVE_FUNCTION
	return false;
}

//关闭连接
void SocketClient::close()
{
	__ENTER_FUNCTION
	Socket::close();
	memset(_hostIP, 0, MAX_IP_SIZE);
	_hostPort = 0;
	__LEAVE_FUNCTION
}

//连接远程主机
bool SocketClient::connect()
{
	__ENTER_FUNCTION
	_sockAddr.sin_addr.s_addr = inet_addr(_hostIP);
	_sockAddr.sin_port = htons(_hostPort);
	bool bResult = SocketAPI::socket_connect(_sockfd, (const struct sockaddr *)&_sockAddr, sizeof(_sockAddr));
	return bResult;
	__LEAVE_FUNCTION
	return false;
}


} //namespace tcpnet
