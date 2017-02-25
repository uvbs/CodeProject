////////////////////////////////////////////////////////////////////////////////////
// 
// @file: packet_factory_mgr.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:27
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __PACKET_FACTORY_MGR_H_
#define __PACKET_FACTORY_MGR_H_

#include "type.h"
#include "packet.h"
#include "packet_factory.h"
#include "packetdef.h"
#include "singleton.h"

namespace tcp_net {

////////////////////////////////////////////////////////////////////////////////
//	class PacketFactoryMgr
////////////////////////////////////////////////////////////////////////////////
class PacketFactoryMgr : public Singleton<PacketFactoryMgr>
{
public:
	PacketFactoryMgr();
	~PacketFactoryMgr();

public:
	// ��ʼ��
	bool init();
	// ���������
	Packet* createPacket(Packet_ID id);
	// ɾ�������
	bool removePacket(Packet* pPacket);
	// ��ȡ���������ܳ���
	int	 getPacketMaxSize(Packet_ID id) const;
	//
	bool addFactory( PacketFactory* pFactory ) ;
	//���PacketID
	bool checkPacketID(Packet_ID id);

private:
	// ע�������
	void __registerFactory();

private:
	PacketFactory** _ppFactories;		//������
	int* _pStatistics;	//����������ͳ��
};

} //namespace tcp_net
using namespace tcp_net;

#endif
