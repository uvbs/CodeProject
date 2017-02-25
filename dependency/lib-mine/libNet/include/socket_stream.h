////////////////////////////////////////////////////////////////////////////////////
// 
// @file: socket_stream.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:08
// @brief: socket缓冲区
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
	//初始化
	bool		init(Socket* pSocket,
		int inBufSize = DEFAULT_SOCKETINPUT_BUFFERSIZE, 
		int maxInBufSize = MAX_SOCKETINPUT_BUFFERSIZE,
		int outBufSize = DEFAULT_SOCKETOUTPUT_BUFFERSIZE, 
		int maxOutBufSize = MAX_SOCKETOUTPUT_BUFFERSIZE);
	//清理
	void		cleanUp();
	//填充数据
	int			fill(char* buf, int length, EM_TYPE_MSG_MODE mode ,int& nSize);
	//读
	bool		readPacket(Packet* pPacket);
	//写
	bool		writePacket(Packet* pPacket);
	//接收数据
	int			reveive();
	//发送
	int			send();
	//获得输入Stream
	InputStream* getInstream() { return _pInstream;}
	//获得输出Stream
	OutputStream* getOutstream() { return _pOutstream;}

private :
	Socket* _pSocket ;			//socket
	InputStream* _pInstream;		//输入缓冲
	OutputStream* _pOutstream;	//输出缓冲
};

} //namespace tcpnet
using namespace tcp_net;

#endif

