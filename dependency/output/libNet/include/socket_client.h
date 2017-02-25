////////////////////////////////////////////////////////////////////////////////////
// 
// @file: socket_client.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:12
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __SOCKET_CLIENT_H_
#define __SOCKET_CLIENT_H_

#include "socket.h"
#include "socketdef.h"

namespace tcp_net {

////////////////////////////////////////////////////////////////////////////////
//	class SocketClient
////////////////////////////////////////////////////////////////////////////////
class SocketClient : public Socket
{
public:
	SocketClient();
	virtual ~SocketClient();

public :
	bool create(const char* host, uint port); //创建socket
	void close();
	bool connect();
	uint getPort() const { return _hostPort; }

public :
	char _hostIP[MAX_IP_SIZE]; //主机IP
	uint _hostPort; //主机端口
};

} //namespace tcpnet
using namespace tcp_net;

#endif
