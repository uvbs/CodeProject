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

		//��������ͨ���׽���
		SOCK socket_create(int domain, int type, int protocol);
		//�󶨶˿�
		bool socket_bind(SOCK s, const struct sockaddr* name, int namelen);
		//��������
		bool socket_connect(SOCK s , const struct sockaddr * addr , int addrlen);
		//����
		bool socket_listen(SOCK s, int backlog);
		//��������
		SOCK socket_accept(SOCK s, struct sockaddr* addr, int* addrlen);
		//��������
		int	 socket_send(SOCK s, const void* buf, int length, int flags);
		int	 socket_sendto(SOCK s, const void* buf, int length, int flags, const struct sockaddr* to, int tolen);
		//��������
		int	 socket_recv(SOCK s, void* buf, int length, int flags);
		int	 socket_recvfrom(SOCK s, void* buf, int length, int flags, struct sockaddr* from, int* fromlen);
		//�ر��׽���
		bool socket_close(SOCK s);
		//����׽�����Ϣ
		bool socket_getsockopt(SOCK s, int level, int optname, void* optval, int* optlen);
		//�����׽�����Ϣ
		bool socket_setsockopt(SOCK s, int level, int optname, const void* optval, int optlen);
		//�����׽���ѡ��
		bool socket_ioctlsocket(SOCK s, long cmd, unsigned long* argp);
		//����������������ģʽ
		bool setsocketnonblocking(SOCK s, bool on);
		//��û�����������
		uint socket_available( SOCK s );
		//��ѯ״̬
		int	 socket_select(int maxfdp1, fd_set* readset, fd_set* writeset, fd_set* exceptset, struct timeval* timeout) ;
	} //namespace SocketAPI

} //namespace tcpnet
using namespace tcp_net;

#endif
