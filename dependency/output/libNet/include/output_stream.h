////////////////////////////////////////////////////////////////////////////////////
// 
// @file: output_stream.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:13
// @brief: ���������
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __OUTPUT_STREAM_H_
#define __OUTPUT_STREAM_H_

#include "type.h"
#include "socketdef.h"

namespace tcp_net {

class Packet;
class Socket;
class SocketStream;
////////////////////////////////////////////////////////////////////////////////
//	class OutputStream
////////////////////////////////////////////////////////////////////////////////
class OutputStream
{
public :
	OutputStream();
	~OutputStream();

public :
	//��ʼ��
	bool		init(int bufferSize = DEFAULT_SOCKETOUTPUT_BUFFERSIZE, 
		int maxBufferSize = MAX_SOCKETOUTPUT_BUFFERSIZE);
	//����
	void		cleanUp( ) ;
	//д������
	int			write(const char* buf, int len) ;
	bool		writePacket(SocketStream& socketstream, Packet* pPacket ) ;
	//�޸�����
	bool		modifyValue(const void*);
	//����
	int			flush(Socket* pSocket) ;
	//���³�ʼ��
	void		initsize();
	bool		resize(int size);
	//�������
	int			capacity() const { return _length ; }
	//��û��峤��
	int			getLength() const ;
	//��û�����ָ��
	char*	getBuffer() const { return _pBuffer ; }
	//�Ƿ񻺳���Ϊ��
	bool		isEmpty() const { return _head==_tail ; }
	//��ȡͷβ
	int			getHead() const { return _head; }
	int			getTail() const { return _tail; }

private :
	char*	_pBuffer;		//������
	int		_length;			//����������
	int		_maxLength;	//��������󳤶�
	int		_head;			//ͷ���
	int		_tail;			//β���
};

} //namespace tcpnet
using namespace tcp_net;

#endif
