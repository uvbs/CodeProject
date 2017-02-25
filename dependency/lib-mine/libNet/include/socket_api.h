////////////////////////////////////////////////////////////////////////////////////
// 
// @file: socket_api.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:11
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __SOCKET_API_H_
#define __SOCKET_API_H_

#include "type.h"
#include "socketdef.h"

namespace tcp_net {

	////////////////////////////////////////////////////////////////////////////////
	//	socket API
	////////////////////////////////////////////////////////////////////////////////
	namespace SocketAPI {

		//创建网络通信套接字
		SOCK socket_create(int domain, int type, int protocol);
		//绑定端口
		bool socket_bind(SOCK s, const struct sockaddr* name, int namelen);
		//建立连接
		bool socket_connect(SOCK s , const struct sockaddr * addr , int addrlen);
		//监听
		bool socket_listen(SOCK s, int backlog);
		//接受连接
		SOCK socket_accept(SOCK s, struct sockaddr* addr, int* addrlen);
		//发送数据
		int	 socket_send(SOCK s, const void* buf, int length, int flags);
		int	 socket_sendto(SOCK s, const void* buf, int length, int flags, const struct sockaddr* to, int tolen);
		//接收数据
		int	 socket_recv(SOCK s, void* buf, int length, int flags);
		int	 socket_recvfrom(SOCK s, void* buf, int length, int flags, struct sockaddr* from, int* fromlen);
		//关闭套接字
		bool socket_close(SOCK s);
		//获得套接字信息
		bool socket_getsockopt(SOCK s, int level, int optname, void* optval, int* optlen);
		//设置套接字信息
		bool socket_setsockopt(SOCK s, int level, int optname, const void* optval, int optlen);
		//控制套接字选项
		bool socket_ioctlsocket(SOCK s, long cmd, unsigned long* argp);
		//设置阻塞及非阻塞模式
		bool setsocketnonblocking(SOCK s, bool on);
		//获得缓冲现有数据
		uint socket_available( SOCK s );
		//查询状态
		int	 socket_select(int maxfdp1, fd_set* readset, fd_set* writeset, fd_set* exceptset, struct timeval* timeout) ;
	} //namespace SocketAPI

} //namespace tcpnet
using namespace tcp_net;

#endif
