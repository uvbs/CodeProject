/***************************************************
** @file: CSHeartBeatPacket.h
** @author: Mr.Chen
** @date: 2016/7/5
** @brief: 
***************************************************/
#ifndef __CS_HEART_BEAT_PACKET_H__
#define __CS_HEART_BEAT_PACKET_H__

#include "type.h"
#include "packet.h"
#include "packet_factory.h"
#include "packetdef.h"

class CSHeartBeatPacket : public Packet 
{
public:
	CSHeartBeatPacket() {}

	//公用继承接口
	virtual bool	fill( SocketStream& ioStream , EM_TYPE_MSG_MODE mode);
	virtual int	execute( Player* pPlayer ) ;
	virtual Packet_ID	getPacketID()const { return PACKET_CS_HEARTBEAT; }
};


class CSHeartBeatPacketFactory : public PacketFactory 
{
public:
	Packet_ID	getPacketID() const { return PACKET_CS_HEARTBEAT; }
	int		getPacketMaxSize() const 
	{
		return sizeof(CSHeartBeatPacket) - sizeof(Packet); 
	}
private:
	Packet*	__create() 
	{ 
		Packet* pPacket = new CSHeartBeatPacket();
		return pPacket; 
	}
};


class CSHeartBeatPacketHandler 
{
public:
#ifdef _SERVER_
	static int	execute(CSHeartBeatPacket* pPacket, Player* pPlayer);
#else
	static	 int	execute(CSHeartBeatPacket* pPacket, Player* pPlayer){ return PACKET_EXE_ERROR; }
#endif
};

#endif
