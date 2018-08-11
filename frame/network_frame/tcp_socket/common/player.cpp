#include "player.h"
#include "socket.h"
#include "socket_stream.h"
#include "socket_client.h"
#include "socket_server.h"
#include "packet.h"
#include "packet_factory_mgr.h"
#include "input_stream.h"
#include "output_stream.h"
#include "myassert.h"
#include "macrodef.h"
#include "player_pool.h"
#include "log.h"

#if defined(_LINUX64)
#include <string.h>          // memset
#endif

WX_IMPLEMENT_SINGLEON(PlayerPool<Player>);

namespace tcp_net {

Player::Player(bool isClient)
{
	_mgrID = -1;
	_poolID = -1;
	if (isClient) {
		_pSocket = new SocketClient();
	} else {
		_pSocket = new SocketServer();
	}
	_pSocketStream = new SocketStream();
	MyAssert(_pSocketStream);
	MyAssert(_pSocket);
	bool ret = _pSocketStream->init(_pSocket);
	MyAssert(ret);
}

Player::~Player()
{
	__ENTER_FUNCTION
	SAFE_DELETE(_pSocket);
	SAFE_DELETE(_pSocketStream);
	__LEAVE_FUNCTION
}

void Player::cleanUp()
{
	if (_pSocket){
		_pSocket->close();
	}
	if (_pSocketStream){
		_pSocketStream->cleanUp();
	}
}

void Player::disconnect()
{
	cleanUp();
}

bool Player::processInput( )
{
	__ENTER_FUNCTION

	MyAssert(_pSocketStream);
	__MYTRY 
	{
		int err = _pSocketStream->reveive() ;
		if (err <= SOCKET_ERROR)
		{
			LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
				"socket Reveive error: error code = %d, error description = %s", 
				getSocketErrorCode(), getSocketErrorDescription()) ;
			return false;
		}
	} 
	__MYCATCH
	{}

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool Player::processCmd( )
{
	__ENTER_FUNCTION

	MyAssert(_pSocketStream);
	InputStream* pInStream = _pSocketStream->getInstream();
	MyAssert(pInStream);
	PacketFactoryMgr* pPacketFactryMgr = PacketFactoryMgr::getSinglePtr();
	MyAssert(pPacketFactryMgr);

	Packet* pPacket = NULL;

	__MYTRY
	{
		////TGW 改造
		//if (IsGamePlayer() && GetFlag())
		//{
		//	//检查TGW包头
		//	char packetTGW[PACKET_TGW_SIZE] = {0};
		//	if( !pInputStream->Peek(&packetTGW[0], PACKET_TGW_SIZE) )
		//	{
		//		return true; //下一帧再处理
		//	}
		//	pInputStream->Skip(PACKET_TGW_SIZE);
		//	SetFlag(false);
		//}
		for (int i = 0; i < PACKET_COUNT_PER_TICK; i++)
		{
			char packetHead[PACKET_HEADER_SIZE] = {0};
			//检查消息头
			if (!pInStream->peek(&packetHead[0], PACKET_HEADER_SIZE))
			{
				break ;	//消息头不能填充
			}
///协议包头解密
#ifdef _ENCRYPT_
			{
				decryptHead_cs(packetHead);
			}
#endif
			Packet_ID packetID = PACKET_INVALID;
			int packetSize = 0;
			char packetEncrypt = 0;
			memcpy(&packetID, &packetHead[0], sizeof(Packet_ID)) ;	
			memcpy(&packetEncrypt, &packetHead[sizeof(Packet_ID)], sizeof(char) );
			memcpy(&packetSize, &packetHead[sizeof(Packet_ID) + sizeof(char)], sizeof(int) );
///协议解密--Begin
#ifdef _ENCRYPT_
			{
				int size = packetSize + PACKET_HEADER_SIZE;
				int head = pInStream->getHead();
				int tail = pInStream->getTail();
				int maxBufferLen = pInStream->capacity();
				char* buffer = pInStream->getBuffer();
				if (head < tail) 
				{
					decrypt_cs(&buffer[head], size);
				}
				else 
				{
					int rightLen = maxBufferLen - head;
					if( size <= rightLen ) 
					{
						decrypt_cs(&buffer[head], size);
					} 
					else
					{
						decrypt_cs(&buffer[head], rightLen);
						decrypt_cs(buffer, size-rightLen);
					}
				}
			}
#endif
///协议解密--End
			//packetID无效
			if (!pPacketFactryMgr->checkPacketID(packetID))
			{
				LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
					"PacketID is Invalid or PacketID is not Reg in factory, packetID = %d", packetID) ;
				bool ret = pInStream->skip(PACKET_HEADER_SIZE + packetSize);
				if (!ret)
				{}
				return false ;
			}
			int maxPacketSize = pPacketFactryMgr->getPacketMaxSize(packetID);
			if (maxPacketSize < 0)
			{
				return false ;
			}
			if (packetSize > maxPacketSize|| packetSize < 0)
			{
				LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
					"PacketSize error, packetID=%d, packetSize is %d, maxPacketSize is %d.", 
					packetID, packetSize, maxPacketSize) ;
				return false ;
			}
			//消息没有接收全
			if (pInStream->getLength() < PACKET_HEADER_SIZE + packetSize)
			{
				break;
			}
			pPacket = PacketFactoryMgr::getSinglePtr()->createPacket(packetID);
			if (pPacket == NULL)
			{
				return false;
			}
			__MYTRY
			{
				//读取数据
				bool ret = _pSocketStream->readPacket(pPacket) ;
				if (!ret || pPacket->getSize() != packetSize)
				{
					LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
						"PacketSize error, packetID=%d, packetSize is %d, recvPacketSize is %d, maxPacketSize is %d.", 
						packetID, packetSize, pPacket->getSize(), maxPacketSize) ;
					PacketFactoryMgr::getSinglePtr()->removePacket(pPacket);
					return false ;
				}
				bool isNeedRemove = true ;
				int err = PACKET_EXE_ERROR;
				resetKickTime( );
				__MYTRY
				{
					err = pPacket->execute(this) ;
				}
				__MYCATCH
				{
					err = PACKET_EXE_ERROR ;
				}
				//出现异常错误，断开此玩家连接
				if (err == PACKET_EXE_ERROR)
				{
					if (pPacket) 
					{
						PacketFactoryMgr::getSinglePtr()->removePacket(pPacket) ;
					}
					return false ;
				}
				//当前消息的解析执行将停止直到下个循环时才继续对缓存中的数据进行消息格式化和执行。
				//当需要将客户端的执行从一个场景转移到另外一个场景时：需要在发送转移消息后将执行在本线程中停止。
				else if (err == PACKET_EXE_BREAK)
				{
					if (pPacket) 
					{
						PacketFactoryMgr::getSinglePtr()->removePacket(pPacket) ;
					}
					break ;
				}
				//继续解析剩下的消息
				else if (err == PACKET_EXE_CONTINUE)
				{
				}
				//继续解析剩下的消息，并且不回收当前消息
				else if (err == PACKET_EXE_NOTREMOVE)
				{
					isNeedRemove = false ;
				}
				if (pPacket && isNeedRemove) 
				{
					PacketFactoryMgr::getSinglePtr()->removePacket(pPacket) ;
				}
			}
			__MYCATCH
			{
				return false ;
			}
		}
	}
	__MYCATCH
	{
		return false ;
	}

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool Player::processOutput()
{
	__ENTER_FUNCTION

	MyAssert(_pSocketStream);

	__MYTRY
	{
		int size = _pSocketStream->getOutstream()->getLength() ;
		if (size <= 0)
		{
			return true ;
		}
		int err = _pSocketStream->send();
		if (err <= SOCKET_ERROR)
		{
			LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
				"Socket send error,error code = %d, error description = %s", 
				getSocketErrorCode(), getSocketErrorDescription()) ;
			return false ;
		}
	} 
	__MYCATCH
	{
		return false ;
	}
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool Player::sendPacket(Packet* pPacket)
{
	__ENTER_FUNCTION

	MyAssert(_pSocketStream);
	MyAssert(pPacket);

	int beforeSize = _pSocketStream->getOutstream()->getLength();
	int afterSize = 0;
	int writeSize = 0;
	bool ret = false;
	__MYTRY
	{
		//查询当前包尾位置。记录写包前位置
		int tail_begin = _pSocketStream->getOutstream()->getTail();
		ret = _pSocketStream->writePacket(pPacket);
		MyAssert(ret);
		//查询当前包尾位置。记录写包后位置
		int tail_end = _pSocketStream->getOutstream()->getTail();
		afterSize = _pSocketStream->getOutstream()->getLength();
		writeSize = afterSize - beforeSize;
		
///协议加密--Begin
#ifdef __ENCRYPT__
		{
			int head = _pSocketStream->getOutstream()->getHead();
			int tail = _pSocketStream->getOutstream()->getTail();
			int maxBufferLen = _pSocketStream->getOutstream()->capacity();
			char* buffer = _pSocketStream->getOutstream()->getBuffer();
			if (head < tail) 
			{
				//int nSize = nTail_End - nTail_Begin;
				encrypt_cs(&(buffer[tail_begin]), writeSize);
			} 
			else
			{
				if (tail_end < tail_begin)
				{
					int rightLen = maxBufferLen - tail_begin;
					encrypt_cs(&(buffer[tail_begin]), rightLen);
					encrypt_cs(buffer, writeSize-rightLen);
				}
				else
				{
					encrypt_cs(&(buffer[tail_begin]), writeSize);
				}
			}
		}
#endif
///协议加密--End
	}
	__MYCATCH
	{
		return false ;
	}
	if (pPacket->getSize() != writeSize - PACKET_HEADER_SIZE)
	{
		LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, 
			"Packet Size Error! ID=%d, SIZE=%d, should=%d", 
			pPacket->getPacketID(), pPacket->getSize(), writeSize - PACKET_HEADER_SIZE);
	}

	return ret ;
	__LEAVE_FUNCTION
	return false ;
}

} //namespace tcp_net
