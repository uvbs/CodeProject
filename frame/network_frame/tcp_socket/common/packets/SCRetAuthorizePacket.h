/***************************************************
** @file: SCRetAuthorizePacket.h
** @author: Mr.Chen
** @date: 2016/7/5
** @brief: 
***************************************************/
#ifndef _SC_RET_AUTHORIZE_PACKET_H
#define _SC_RET_AUTHORIZE_PACKET_H

#include "type.h"
#include "packet.h"
#include "packet_factory.h"
#include "packetdef.h"

class SCRetAuthorizePacket : public Packet 
{
public:
	//公用继承接口
	virtual bool	fill(SocketStream& ioStream, EM_TYPE_MSG_MODE mode) { return true; }
	virtual int	execute(Player* pPlayer);
	virtual Packet_ID	getPacketID() const { return PACKET_SC_RET_AUTHORIZE; }

};

class SCRetAuthorizePacketFactory : public PacketFactory 
{
public:
	Packet_ID	getPacketID() const { return PACKET_SC_RET_AUTHORIZE; }
	int		getPacketMaxSize() const 
	{
		return sizeof(SCRetAuthorizePacket) - sizeof(Packet); 
	}
private:
	Packet*		 __create() 
	{ 
		Packet* pPacket = new SCRetAuthorizePacket();
		return pPacket; 
	}
};

class SCRetAuthorizePacketHandler 
{
public:
#ifdef _SERVER_
	static int execute(SCRetAuthorizePacket* pPacket, Player* pPlayer) { return PACKET_EXE_CONTINUE; }
#else
	static int execute(SCRetAuthorizePacket* pPacket, Player* pPlayer);
#endif
};

#endif
