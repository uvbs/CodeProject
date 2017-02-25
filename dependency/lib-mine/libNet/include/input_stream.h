////////////////////////////////////////////////////////////////////////////////////
// 
// @file: input_stream.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:14
// @brief: 输入缓冲区
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __INPUT_STREAM_H__
#define __INPUT_STREAM_H__

#include "type.h"
#include "socketdef.h"

namespace tcp_net {

class Packet;
class Socket;
class SocketStream;
////////////////////////////////////////////////////////////////////////////////
//	输入流
////////////////////////////////////////////////////////////////////////////////
class InputStream
{
public :
	InputStream();
	~InputStream();

public :
	//初始化
	bool		init(int bufferSize = DEFAULT_SOCKETINPUT_BUFFERSIZE, 
		int maxBufferSize = MAX_SOCKETINPUT_BUFFERSIZE);
	//清理
	void		cleanUp() ;
	//读数据
	int			read(char* buf, int length) ;
	bool		readPacket(SocketStream& socketstream, Packet* pPacket) ;
	//窥探数据
	bool		peek(char* buf, int length) ;
	//跳过指定长度缓冲
	bool		skip(int length) ;
	//接收socket数据到缓冲区
	int			reveive(Socket* pSocket);
	//重新初始化
	void		initsize() ;
	//重置缓冲区大小
	bool		resize(int size) ;
	//获得容量
	int			capacity() const { return _length; }
	//获得buf长度
	int			getLength() const ;
	//获得缓冲区
	char*	getBuffer() const {return _pBuffer; }
	//是否buf为空
	bool		isEmpty() const { return _head == _tail; }
	//获得头尾
	int			getHead() const { return _head; }
	int			getTail() const { return _tail; }

private :
	char*	_pBuffer;	//缓冲区
	int		_length;		//缓冲区长度
	int		_maxLength;	//缓冲区最大长度
	int		_head;	//头标记
	int		_tail;		//尾标记
};

} //namespace tcpnet
using namespace tcp_net;

#endif
