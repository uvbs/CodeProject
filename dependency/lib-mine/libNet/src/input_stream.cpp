#include "input_stream.h"
#include "socket_stream.h"
#include "assert.h"
#include "macrodef.h"
#include "packet.h"

#if defined(_LINUX64)
#include <string.h>          // memset
#endif

namespace tcp_net {

extern int g_socketErrorCode;

InputStream::InputStream() 
{
	_length = _maxLength = 0;
	_head = _tail = 0;
	_pBuffer = NULL;
}

InputStream::~InputStream( ) 
{
	__ENTER_FUNCTION
	delete[] _pBuffer; 
	_pBuffer = NULL;
	_length =_maxLength = 0;
	_head = _tail = 0;
	_pBuffer = NULL;
	__LEAVE_FUNCTION
}

//³õÊ¼»¯
bool	InputStream::init(int bufferSize, int maxBufferSize)
{
	__ENTER_FUNCTION
	if (bufferSize > 0 && maxBufferSize >= bufferSize)
	{
		_length = bufferSize ;
		_maxLength = maxBufferSize;
		_head = _tail = 0;
		_pBuffer = new char[_length];
		Assert(_pBuffer);
		memset(_pBuffer, 0,  _length * sizeof(char)) ;
		return true ;
	}
	__LEAVE_FUNCTION
	return false;
}

int InputStream::getLength() const
{
	if (_head < _tail) { return _tail - _head; }
	else if (_head > _tail) { return _length - _head + _tail; }
	return 0;
}

int InputStream::read(char* buf, int length) 
{
	__ENTER_FUNCTION
	if (buf == NULL|| length <= 0) { return 0; }
	if (length > getLength()) { return 0; }
	if ( _head < _tail ) {
		memcpy(buf, &_pBuffer[_head], length);
	} 
	else 
	{
		int rightLen = _length - _head;
		if (length <= rightLen) {
			memcpy(buf, &_pBuffer[_head], length);
		} 
		else  {
			memcpy(buf, &_pBuffer[_head], rightLen);
			memcpy(&buf[rightLen], _pBuffer, length - rightLen);
		}
	}
	_head = (_head + length) % _length ;
	return length;
	__LEAVE_FUNCTION
	return 0 ;
}

bool	InputStream::readPacket(SocketStream& socketstream, Packet* pPacket)
{
	__ENTER_FUNCTION
	if (pPacket == NULL) { return false; }
	bool ret = skip(PACKET_HEADER_SIZE);
	if (!ret) { return false; }
	return pPacket->fill(socketstream, EM_TYPE_MSG_MODE_READ);
	__LEAVE_FUNCTION
	return false;
}

bool InputStream::peek(char* buf, int length) 
{
	__ENTER_FUNCTION
	if (buf == NULL || length <= 0) { return false; }
	if (length > getLength()) { return false; }
	if (_head < _tail) {
		memcpy(buf, &_pBuffer[_head], length);
	} 
	else 
	{
		int rightLen = _length - _head;
		if (length <= rightLen) {
			memcpy(&buf[0], &_pBuffer[_head], length);
		} 
		else {
			memcpy(&buf[0], &_pBuffer[_head], rightLen);
			memcpy(&buf[rightLen], &_pBuffer[0], length - rightLen);
		}
	}
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool InputStream::skip(int length) 
{
	if (length <= 0) { return false; }
	if (length>getLength()) { return false; }
	_head = (_head + length) % _length;
	return true ;
}

void InputStream::initsize()
{
	__ENTER_FUNCTION
	_head = _tail = 0 ;
	if (_pBuffer) delete[] _pBuffer;
	_pBuffer = new char[DEFAULT_SOCKETINPUT_BUFFERSIZE];
	Assert(_pBuffer);
	memset(_pBuffer, 0, _length);
	_length = DEFAULT_SOCKETINPUT_BUFFERSIZE;
	__LEAVE_FUNCTION
}

int InputStream::reveive( Socket* pSocket ) 
{
	__ENTER_FUNCTION
	if (pSocket == NULL) { throw 1; }
	int nFilled = 0;
	int nReceived = 0;
	int nFree = 0;
	if (_head <= _tail) 
	{
		if (_head == 0) 
		{
			nReceived = 0;
			nFree = _length - _tail - 1;
			if (nFree != 0)
			{
				nReceived = pSocket->receive(&_pBuffer[_tail] , nFree);
				if (nReceived == SOCKET_ERROR_WOULD_BLOCK) {
					return 0; 
				}
				else if (nReceived == 0) {
					return EM_SOCKET_ERROR_STEP0;
				}
				else if (nReceived < 0) {
					return EM_SOCKET_ERROR_STEP1;
				}
				_tail += nReceived;
				nFilled += nReceived;
			}
			if (nReceived == nFree) 
			{
				int available = static_cast<int>(pSocket->available());
				if (available > 0) 
				{
					if ((_length + available + 1) > _maxLength) {
						initsize();
						g_socketErrorCode = 0;
						return EM_SOCKET_ERROR_STEP2;
					}
					if (!resize(available + 1)) { return 0; }
					nReceived = pSocket->receive(&_pBuffer[_tail], available);
					if (nReceived == SOCKET_ERROR_WOULD_BLOCK) {
						return 0; 
					}
					else if (nReceived == 0) {
						return EM_SOCKET_ERROR_STEP3;
					}
					else if (nReceived < 0) {
						return EM_SOCKET_ERROR_STEP4;
					}
					_tail += nReceived;
					nFilled += nReceived;
				}
			}
		} 
		else 
		{
			nFree = _length - _tail;
			nReceived = pSocket->receive(&_pBuffer[_tail], nFree);
			if (nReceived==SOCKET_ERROR_WOULD_BLOCK) {
				return 0; 
			}
			else if (nReceived == 0) {
				return EM_SOCKET_ERROR_STEP5;
			}
			else if (nReceived < 0) {
				return EM_SOCKET_ERROR_STEP6;
			}
			_tail = (_tail + nReceived) % _length;
			nFilled += nReceived;
			if (nReceived == nFree) 
			{
				nReceived = 0;
				nFree = _head - 1;
				if (nFree != 0)
				{
					nReceived = pSocket->receive(&_pBuffer[0] , nFree);
					if (nReceived == SOCKET_ERROR_WOULD_BLOCK) {
						return 0; 
					}
					else if (nReceived == 0) {
						return EM_SOCKET_ERROR_STEP7;
					}
					else if (nReceived < 0) {
						return EM_SOCKET_ERROR_STEP8;
					}
					_tail += nReceived;
					nFilled += nReceived;
				}
				if (nReceived == nFree) 
				{
					int available = static_cast<int>(pSocket->available());
					if (available > 0) 
					{
						if((_length + available + 1) > _maxLength) 
						{
							initsize();
							g_socketErrorCode = 0;
							return EM_SOCKET_ERROR_STEP9;
						}
						if (!resize(available + 1)) {	return 0;}
						nReceived = pSocket->receive(&_pBuffer[_tail], available);
						if (nReceived == SOCKET_ERROR_WOULD_BLOCK) {
							return 0; 
						}
						else if (nReceived == 0) {
							return EM_SOCKET_ERROR_STEP10;
						}
						else if (nReceived < 0) {
							return EM_SOCKET_ERROR_STEP11;
						}
						_tail += nReceived;
						nFilled += nReceived;
					}
				}
			}
		}
	} 
	else 
	{
		nReceived = 0;
		nFree = _head - _tail - 1 ;
		if (nFree != 0)
		{
			nReceived = pSocket->receive(&_pBuffer[_tail], nFree) ;
			if (nReceived == SOCKET_ERROR_WOULD_BLOCK) {
				return 0; 
			}
			else if (nReceived == 0) {
				return EM_SOCKET_ERROR_STEP12;
			}
			else if (nReceived < 0) {
				return EM_SOCKET_ERROR_STEP13;
			}
			_tail += nReceived;
			nFilled += nReceived;
		}
		if (nReceived == nFree) 
		{
			int available = static_cast<int>(pSocket->available());
			if (available > 0) 
			{
				if((_length + available + 1) > _maxLength)
				{
					initsize();
					g_socketErrorCode = 0;
					return EM_SOCKET_ERROR_STEP14;
				}
				if (!resize(available + 1)) { return 0; }
				nReceived = pSocket->receive( &_pBuffer[_tail], available) ;
				if (nReceived == SOCKET_ERROR_WOULD_BLOCK) {
					return 0 ; 
				}
				else if (nReceived == 0) {
					return EM_SOCKET_ERROR_STEP15;
				}
				else if (nReceived < 0) {
					return EM_SOCKET_ERROR_STEP16;
				}
				_tail += nReceived;
				nFilled += nReceived;
			}
		}
	}
	return nFilled ;
	__LEAVE_FUNCTION
	return 0 ;
}

bool InputStream::resize(int size)
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
	if (_head < _tail) {
		memcpy(newBuffer, &_pBuffer[_head], _tail - _head);
	} 
	else if (_head > _tail) {
		memcpy(newBuffer, &_pBuffer[_head], _length - _head);
		memcpy(&newBuffer[_length - _head], _pBuffer, _tail);
	}
	delete [] _pBuffer;
	_pBuffer = NULL;
	_pBuffer = newBuffer;
	_length = newBufferLen;
	_head = 0 ;
	_tail = length;
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

void InputStream::cleanUp()
{
	_head = _tail = 0;
}

} //namespace tcpnet 
