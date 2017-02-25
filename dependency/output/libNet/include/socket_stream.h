////////////////////////////////////////////////////////////////////////////////////
// 
// @file: socket_stream.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:08
// @brief: socket������
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __SOCKET_STREAM_H_
#define __SOCKET_STREAM_H_

#include "type.h"
#include "socket.h"
#include "socketdef.h"

namespace tcp_net {

class Packet;
class InputStream;
class OutputStream;
////////////////////////////////////////////////////////////////////////////////
//	class SocketStream
////////////////////////////////////////////////////////////////////////////////
class SocketStream
{
public :
	SocketStream();
	~SocketStream();

public :
	//��ʼ��
	bool		init(Socket* pSocket,
		int inBufSize = DEFAULT_SOCKETINPUT_BUFFERSIZE, 
		int maxInBufSize = MAX_SOCKETINPUT_BUFFERSIZE,
		int outBufSize = DEFAULT_SOCKETOUTPUT_BUFFERSIZE, 
		int maxOutBufSize = MAX_SOCKETOUTPUT_BUFFERSIZE);
	//����
	void		cleanUp();
	//�������
	int			fill(char* buf, int length, EM_TYPE_MSG_MODE mode ,int& nSize);
	//��
	bool		readPacket(Packet* pPacket);
	//д
	bool		writePacket(Packet* pPacket);
	//��������
	int			reveive();
	//����
	int			send();
	//�������Stream
	InputStream* getInstream() { return _pInstream;}
	//������Stream
	OutputStream* getOutstream() { return _pOutstream;}

private :
	Socket* _pSocket ;			//socket
	InputStream* _pInstream;		//���뻺��
	OutputStream* _pOutstream;	//�������
};

} //namespace tcpnet
using namespace tcp_net;

#endif

