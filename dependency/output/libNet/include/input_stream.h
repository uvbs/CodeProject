////////////////////////////////////////////////////////////////////////////////////
// 
// @file: input_stream.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:14
// @brief: ���뻺����
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
//	������
////////////////////////////////////////////////////////////////////////////////
class InputStream
{
public :
	InputStream();
	~InputStream();

public :
	//��ʼ��
	bool		init(int bufferSize = DEFAULT_SOCKETINPUT_BUFFERSIZE, 
		int maxBufferSize = MAX_SOCKETINPUT_BUFFERSIZE);
	//����
	void		cleanUp() ;
	//������
	int			read(char* buf, int length) ;
	bool		readPacket(SocketStream& socketstream, Packet* pPacket) ;
	//��̽����
	bool		peek(char* buf, int length) ;
	//����ָ�����Ȼ���
	bool		skip(int length) ;
	//����socket���ݵ�������
	int			reveive(Socket* pSocket);
	//���³�ʼ��
	void		initsize() ;
	//���û�������С
	bool		resize(int size) ;
	//�������
	int			capacity() const { return _length; }
	//���buf����
	int			getLength() const ;
	//��û�����
	char*	getBuffer() const {return _pBuffer; }
	//�Ƿ�bufΪ��
	bool		isEmpty() const { return _head == _tail; }
	//���ͷβ
	int			getHead() const { return _head; }
	int			getTail() const { return _tail; }

private :
	char*	_pBuffer;	//������
	int		_length;		//����������
	int		_maxLength;	//��������󳤶�
	int		_head;	//ͷ���
	int		_tail;		//β���
};

} //namespace tcpnet
using namespace tcp_net;

#endif
