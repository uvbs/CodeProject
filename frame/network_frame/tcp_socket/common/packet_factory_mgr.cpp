#include "packet_factory_mgr.h"
#include "myassert.h"
#include "macrodef.h"
#include "CSAuthorizePacket.h"
#include "SCRetAuthorizePacket.h"
#include "CSEnterScenePacket.h"
#include "SCRetEnterScenePacket.h"
#include "CSHeartBeatPacket.h"

WX_IMPLEMENT_SINGLEON(tcp_net::PacketFactoryMgr);

namespace tcp_net {

PacketFactoryMgr::PacketFactoryMgr()
{
	_ppFactories = new PacketFactory*[PACKET_NUMBER];
	MyAssert(_ppFactories);
	_pStatistics = new int[PACKET_NUMBER];
	MyAssert(_pStatistics);
	for (int i = 0; i < PACKET_NUMBER; ++i)
	{
		_ppFactories[i] = NULL;
		_pStatistics[i] = 0;
	}
}

PacketFactoryMgr::~PacketFactoryMgr()
{
	SAFE_DELETE_ARRAY(_ppFactories);
	SAFE_DELETE_ARRAY(_pStatistics);
}

bool PacketFactoryMgr::init()
{
	__registerFactory();
	return true;
}

Packet* PacketFactoryMgr::createPacket(Packet_ID id)
{
	if (id < 0 || id >= PACKET_NUMBER)
	{
		return NULL;
	}
	PacketFactory* pFactory = _ppFactories[id];
	if (pFactory == NULL)
	{
		return NULL;
	}
	Packet* pPacket = pFactory->createPacket();
	if (pPacket == NULL)
	{
		MyAssert(0);
	}
	//增加包的创建计数
	++_pStatistics[id];
	return pPacket;
}

bool PacketFactoryMgr::removePacket(Packet* pPacket)
{
	if (pPacket == NULL)
	{
		return false;
	}
	Packet_ID id = pPacket->getPacketID();
	if (id <= PACKET_INVALID || id > PACKET_NUMBER)
	{
		return false;
	}
	PacketFactory* pFactory = _ppFactories[id];
	if (pFactory != NULL)
	{
		return pFactory->removePacket(pPacket);
	}
	return false;
}

int PacketFactoryMgr::getPacketMaxSize(Packet_ID id) const
{
	if (id <= PACKET_INVALID || id > PACKET_NUMBER)
	{
		return -1;
	}
	PacketFactory* pFactory = _ppFactories[id];
	if (pFactory == NULL)
	{
		return -1;
	}
	return pFactory->getPacketMaxSize();
}

bool PacketFactoryMgr::addFactory(PacketFactory* pFactory) 
{
	if (pFactory == NULL)
	{
		return false;
	}
	Packet_ID id = pFactory->getPacketID();
	if (id <= PACKET_INVALID || id > PACKET_NUMBER)
	{
		return false;
	}
	if (_ppFactories[id] != NULL) 
	{//重复设定
		MyAssert(0) ;
		return false;
	}
	_ppFactories[id] = pFactory ;
	return true;
}

bool	PacketFactoryMgr::checkPacketID(Packet_ID id)
{
	if (id <= PACKET_INVALID || id > PACKET_NUMBER)
	{
		return false;
	}
	if (_ppFactories[id] == NULL)
	{
		return false;
	}
	return true;
}

void PacketFactoryMgr::__registerFactory()
{
	PacketFactory *pPacketFactory = NULL;

	pPacketFactory = new CSAuthorizePacketFactory(); addFactory(pPacketFactory);
	pPacketFactory = new SCRetAuthorizePacketFactory(); addFactory(pPacketFactory);
	pPacketFactory = new CSEnterScenePacketFactory(); addFactory(pPacketFactory);
	pPacketFactory = new SCRetEnterScenePacketFactory(); addFactory(pPacketFactory);
	pPacketFactory = new CSHeartBeatPacketFactory(); addFactory(pPacketFactory);
}

} //namespace tcp_net
