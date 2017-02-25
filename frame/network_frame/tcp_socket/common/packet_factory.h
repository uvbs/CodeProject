////////////////////////////////////////////////////////////////////////////////////
// 
// @file: packet_factory.h
// @author: by Mr.Chen
// @date: 2014/5/11	01:51
// @brief: ��Ϣ������
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __PACKET_FACTORY_H_
#define __PACKET_FACTORY_H_

#include "type.h"
#include "packet.h"
#include "lock.h"
#include "assert.h"

using tcp_net::Packet;
namespace tcp_net {

////////////////////////////////////////////////////////////////////////////////
//	class Packet
////////////////////////////////////////////////////////////////////////////////
class PacketFactory
{
public:
	PacketFactory()
	{
		_pCache = NULL;
		_createCount = 0;
		_useCount = 0;
		_removeCount = 0;
	}
	virtual ~PacketFactory()
	{
		while (_pCache != NULL)
		{
			Packet* pPacket = _pCache;
			_pCache = _pCache->_pNext;
			delete pPacket;
			pPacket = NULL;
		}
	}
public:
	// ����
	Packet* createPacket()
	{
		AutoMLock autolock(_lock);
		Packet* pRet = NULL;
		if (_pCache != NULL)
		{
			pRet = _pCache;
			_pCache = _pCache->_pNext;
		}
		else
		{
			__MYTRY
			{
				pRet = __create();
				++_createCount;
			}
			__MYCATCH
			{
				pRet = NULL;
			}
		}
		if (pRet)
		{
			pRet->cleanUp();
		}
		++_useCount;
		return pRet;
	}
	// ����
	bool removePacket(Packet* pPacket)
	{
		AutoMLock autolock(_lock);
		if (pPacket == NULL)
		{
			return false;	
		}
		pPacket->_pNext = _pCache;
		_pCache = pPacket;
		++_removeCount;
		return true;
	}

public:
	// ��ȡ�����ͱ��
	virtual	Packet_ID getPacketID() const = 0;
	// ��ȡ���������ܳ��� �����ڼ�飩
	virtual int getPacketMaxSize() const = 0;

private:
	// ��������
	virtual Packet* __create() = 0;

private:
	//����Ĳ�Ʒ���������
	Packet* _pCache;	
	MLock _lock;	//��
	int	_createCount;	//��������
	int _useCount; //��������
	int _removeCount;	//ɾ������
};

} //namespace tcp_net

using namespace tcp_net;

#endif
