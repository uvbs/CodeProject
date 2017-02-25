////////////////////////////////////////////////////////////////////////////////////
// 
// @file: socket_server.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:10
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __SOCKET_SERVER_H_
#define __SOCKET_SERVER_H_

#include "socket.h"
#include "socket_api.h"
#include "socketdef.h"

namespace tcp_net {

////////////////////////////////////////////////////////////////////////////////
//	class SocketServer
////////////////////////////////////////////////////////////////////////////////
class SocketServer: public Socket
{
public :
	SocketServer();
	virtual ~SocketServer();

public :
	bool create(const char* host, int port); //����socket
	void close();
	uint getPort() const { return _hostPort; }
	bool reuseAddr(bool on = true); //���õ�ַ
	bool bind();
	bool listen(int backlog);
	SOCK accept(struct sockaddr* addr, int* addrlen);
	bool accept(Socket* socket) ;

public :
	char _hostIP[MAX_IP_SIZE]; //����IP
	uint _hostPort; //�����˿�
};

} //namespace tcpnet
using namespace tcp_net;

#endif
