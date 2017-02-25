#include "socket_stream.h"
#include "input_stream.h"
#include "output_stream.h"
#include "assert.h"
#include "socketdef.h"

namespace tcp_net {

SocketStream::SocketStream() 
{
	_pSocket = NULL;
	_pInstream = NULL;
	_pOutstream = NULL;
}

SocketStream::~SocketStream( ) 
{
	__ENTER_FUNCTION
	delete[] _pInstream;
	delete[] _pOutstream;
	_pSocket = NULL;
	_pInstream = NULL;
	_pOutstream = NULL;
	__LEAVE_FUNCTION
}

//≥ı ºªØ
bool	SocketStream::init(Socket* pSocket, int inBufSize,  int maxInBufSize,
		int outBufSize, int maxOutBufSize)
{
	__ENTER_FUNCTION
	if (pSocket == NULL) { return false;}
	if (inBufSize <= 0 || maxInBufSize <= 0) { return false; }
	if (outBufSize <= 0 || maxOutBufSize <= 0) { return false; }
	if (inBufSize > maxInBufSize || outBufSize > maxOutBufSize) { return false; }
	_pSocket = pSocket;
	_pInstream = new InputStream();
	if (_pInstream == NULL) { return false; }
	_pOutstream = new OutputStream();
	if (_pInstream == NULL) { return false; }
	bool bRet = _pInstream->init(inBufSize, maxInBufSize);
	if (!bRet) { return false; }
	bRet = _pOutstream->init(outBufSize, maxOutBufSize);
	if (!bRet) { return false; }
	return true;
	__LEAVE_FUNCTION
	return false;
}

int SocketStream::fill(char* buf, int length, EM_TYPE_MSG_MODE mode, int& nSize) 
{
	__ENTER_FUNCTION
	if (_pInstream == NULL || _pOutstream == NULL) { return 0;}
	if ( mode != EM_TYPE_MSG_MODE_READ && mode != EM_TYPE_MSG_MODE_WRITE) {
		return 0;
	}
	if (buf == NULL) { return 0; }
	if (length <= 0) { return 0; }
	if (mode == EM_TYPE_MSG_MODE_READ) {
		length = _pInstream->read(buf, length);
		nSize = nSize + length;
	}
	else if (mode == EM_TYPE_MSG_MODE_WRITE) {
		length =_pOutstream->write(buf, length);
		nSize = nSize + length;
	}
	return length;
	__LEAVE_FUNCTION
	return 0 ;
}

bool SocketStream::readPacket(Packet* pPacket) 
{
	__ENTER_FUNCTION
	if (pPacket == NULL || _pInstream == NULL) { return false; }
	return _pInstream->readPacket(*this, pPacket);
	__LEAVE_FUNCTION
	return false;
}

bool SocketStream::writePacket(Packet* pPacket)
{
	__ENTER_FUNCTION
	if (pPacket == NULL || _pOutstream == NULL) { return false; }
	return _pOutstream->writePacket(*this, pPacket);
	__LEAVE_FUNCTION
	return false;
}

int SocketStream::reveive()
{
	__ENTER_FUNCTION
	if (_pSocket == NULL || _pInstream == NULL) { return SOCKET_ERROR; }
	return _pInstream->reveive(_pSocket);
	__LEAVE_FUNCTION
	return false;
}

int SocketStream::send()
{
	__ENTER_FUNCTION
	if (_pSocket == NULL || _pOutstream == NULL) { return SOCKET_ERROR; }
	return _pOutstream->flush(_pSocket);
	__LEAVE_FUNCTION
	return false ;
}

void SocketStream::cleanUp()
{
	__ENTER_FUNCTION
	if (_pInstream) {
		_pInstream->cleanUp();
	}
	if (_pOutstream) {
		_pOutstream->cleanUp();
	}
	__LEAVE_FUNCTION
}

} //namespace tcpnet
