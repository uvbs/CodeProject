#include "output_stream.h"
#include "socket_stream.h"
#include "assert.h"
#include "macrodef.h"
#include "socketdef.h"
#include "packet.h"

#if defined(_LINUX64)
#include <string.h>          // memset
#endif

namespace tcp_net {

extern int  g_socketErrorCode;

OutputStream::OutputStream() 
{
	_length = _maxLength = 0;
	_head = _tail = 0;
	_pBuffer = NULL;
}

OutputStream::~OutputStream() 
{
	__ENTER_FUNCTION
	delete[] _pBuffer;
	_pBuffer = NULL;
	_length = _maxLength = 0;
	_head = _tail = 0;
	__LEAVE_FUNCTION
}

//初始化
bool	OutputStream::init(int bufferSize, int maxBufferSize)
{
	__ENTER_FUNCTION
	if (bufferSize > 0 && maxBufferSize >= bufferSize)
	{
		_length = bufferSize;
		_maxLength = maxBufferSize;
		_head = _tail = 0;
		_pBuffer = new char[_length];
		memset(_pBuffer, 0, _length * sizeof(char)) ;
		return true ;
	}
	__LEAVE_FUNCTION
	return false;
}

int OutputStream::getLength() const
{
	if (_head < _tail) { return _tail - _head; }
	else if (_head > _tail) { return _length - _head + _tail; }
	return 0 ;
}

int OutputStream::write( const char* buf, int length ) 
{
	__ENTER_FUNCTION
	if (buf == NULL || length <= 0) { return 0; }
	int nFree = ((_head <= _tail) ? (_length - _tail + _head - 1) : (_head - _tail - 1)) ;
	if (length >= nFree) {
		if (!resize(length - nFree + 1)) { return 0;}
	}
	if (_head <= _tail) 
	{
		if (_head == 0) {
			nFree = _length - _tail - 1;
			memcpy(&_pBuffer[_tail], buf, length);
		} 
		else  
		{
			nFree = _length - _tail;
			if (length <= nFree) {
				memcpy(&_pBuffer[_tail], buf, length);
			}
			else {
				memcpy(&_pBuffer[_tail], buf, nFree);
				memcpy(_pBuffer, &buf[nFree], length - nFree);
			}
		}
	} 
	else  {
		memcpy(&_pBuffer[_tail], buf, length) ;
	}
	_tail = (_tail + length) % _length;
	return length;
	__LEAVE_FUNCTION
	return 0 ;
}

bool OutputStream::writePacket(SocketStream& socketstream, Packet* pPacket )
{
	__ENTER_FUNCTION
	if (pPacket == NULL) { return false; }
	Packet_ID packetID = pPacket->getPacketID();
	int w = write((char*)&packetID, sizeof(Packet_ID));
	if (w <= 0) { return false; }
	char packetEncryptFlag = pPacket->getEncryptFlag();
	w = write((char*)&packetEncryptFlag , sizeof(char));
	if (w <= 0) { return false; }
	char* pBuf = &_pBuffer[_tail];
	int packetINT = 0;
	char* pPacketINT = (char*)&packetINT;
	w = write((char*)&packetINT, sizeof(int)) ;
	if (w <= 0) { return false; }
	pPacket->setSize(0);
	bool bRet = pPacket->fill(socketstream, EM_TYPE_MSG_MODE_WRITE) ;
	if (bRet)
	{
		int packetSize = pPacket->getSize();
		packetINT = packetSize;
		if (_head < _tail) {
			int nbegin = _tail - packetSize - sizeof(int);
			char* pBuf = &_pBuffer[nbegin];
			memcpy(pBuf, pPacketINT, sizeof(int)) ;
		}
		else
		{
			if (_tail >= (packetSize + sizeof(int))) {
				char* pBuf = &_pBuffer[_tail - packetSize - sizeof(int)];
				memcpy(pBuf, pPacketINT, sizeof(int));
			}
			else if (_tail > packetSize) {
				int nbegin = _length - (sizeof(int) - (_tail - packetSize));
				char* pBuf = &_pBuffer[nbegin];
				memcpy(pBuf, pPacketINT, sizeof(int) - (_tail - packetSize)) ;
				memcpy(_pBuffer, &pPacketINT[sizeof(int) - (_tail - packetSize)], _tail - packetSize);
			}
			else {
				int nbegin = _length - (packetSize - _tail) - sizeof(int);
				char* pBuf = &_pBuffer[nbegin];
				memcpy(pBuf, pPacketINT, sizeof(int)) ;
			}
		}
	}
	return bRet;
	__LEAVE_FUNCTION
	return false ;
}

void OutputStream::initsize()
{
	__ENTER_FUNCTION
	delete[] _pBuffer;
	_head = _tail = 0;
	_length = 0;
	_pBuffer = new char[DEFAULT_SOCKETOUTPUT_BUFFERSIZE];
	if (_pBuffer) {
		_length = DEFAULT_SOCKETOUTPUT_BUFFERSIZE;
		memset(_pBuffer, 0,  _length);
	}
	__LEAVE_FUNCTION
}

int OutputStream::flush(Socket* pSocket) 
{
	__ENTER_FUNCTION
	if (pSocket == NULL) { throw 1; }
	int nFlushed = 0;
	int nSent = 0;
	int nLeft;
	//如果单个客户端的缓存太大，则重新设置缓存，并将此客户端断开连接
	if (_length > _maxLength) {
		initsize();
		return EM_SOCKET_ERROR_STEP0;
	}
#if defined(_WIN32)
	uint flag = MSG_DONTROUTE;
#elif defined(_LINUX64)
	int flag = MSG_NOSIGNAL;
#endif
	__MYTRY 
	{
		if (_head < _tail) 
		{
			nLeft = _tail - _head;
			while (nLeft > 0)  
			{
				nSent = pSocket->send(&_pBuffer[_head], nLeft, (int)flag);
				if (nSent == SOCKET_ERROR_WOULD_BLOCK) return 0; 
				if (nSent == SOCKET_ERROR) return EM_SOCKET_ERROR_STEP1;
				if (nSent == 0) return 0;
				nFlushed += nSent;
				nLeft -= nSent;
				_head += nSent;
			}
		} 
		else if (_head > _tail) 
		{
			nLeft = _length - _head;
			while (nLeft > 0) 
			{
				nSent = pSocket->send(&_pBuffer[_head], nLeft, (int)flag);
				if (nSent == SOCKET_ERROR_WOULD_BLOCK) return 0; 
				if (nSent == SOCKET_ERROR) return EM_SOCKET_ERROR_STEP2;
				if (nSent == 0) return 0;
				nFlushed += nSent;
				nLeft -= nSent;
				_head += nSent;
			}
			_head = 0;
			nLeft = _tail;
			while (nLeft > 0) 
			{
				nSent = pSocket->send(&_pBuffer[_head], nLeft, (int)flag);
				if (nSent == SOCKET_ERROR_WOULD_BLOCK) return 0; 
				if (nSent == SOCKET_ERROR) return EM_SOCKET_ERROR_STEP3;
				if (nSent == 0) return 0;
				nFlushed += nSent;
				nLeft -= nSent;
				_head += nSent;
			}
		}
		if (_head != _tail) 
		{
			g_socketErrorCode = 0;
			return EM_SOCKET_ERROR_STEP4;
		}
	}
	__MYCATCH
	{
		if (nSent > 0) { _head += nSent; }
	} 
	_head = _tail = 0;
	return nFlushed;
	__LEAVE_FUNCTION
	return 0 ;
}

bool OutputStream::resize(int size)
{
	__ENTER_FUNCTION
	size = Max(size, (int)(_length >> 1));
	int newBufferLen = _length + size;
	int length = getLength();
	if (size < 0) 
	{
		if (newBufferLen < 0 || newBufferLen < length) { 
			return false; 
		}
	}
	char * newBuffer = new char[newBufferLen];
	if (newBuffer == NULL) { return false; }
	if (_head < _tail) {
		memcpy(newBuffer, &_pBuffer[_head], _tail - _head);
	}
	else if (_head > _tail) {
		memcpy(newBuffer, &_pBuffer[_head] , _length - _head);
		memcpy(&newBuffer[_length-_head], _pBuffer, _tail);
	}
	delete[] _pBuffer;
	_pBuffer = newBuffer;
	_length = newBufferLen;
	_head = 0;
	_tail = length;
	return true;
	__LEAVE_FUNCTION
	return false;
}

void OutputStream::cleanUp()
{
	_head = _tail = 0 ;
}

} //namespace tcpnet 
