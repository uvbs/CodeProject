/***************************************************
** @file: SCRetEnterScenePacket.h
** @author: Mr.Chen
** @date: 2016/7/5
** @brief: 
***************************************************/
#ifndef _SC_RET_ENTER_SCENE_PACKET_H
#define _SC_RET_ENTER_SCENE_PACKET_H

#include "type.h"
#include "packet.h"
#include "packet_factory.h"
#include "packetdef.h"

class SCRetEnterScenePacket : public Packet 
{
public :
	enum EM_ENTER_RET
	{
		EM_ENTER_INVALID = -1,
		EM_ENTER_SUCCESS,
		EM_ENTER_FAIL,
	};
public:
	SCRetEnterScenePacket() { cleanUp(); }

	//公用继承接口
	virtual bool	fill(SocketStream& ioStream, EM_TYPE_MSG_MODE mode);
	virtual int	execute(Player* pPlayer) ;
	virtual Packet_ID	getPacketID() const { return PACKET_SC_RET_ENTER_SCENE; }
	virtual void	cleanUp()
	{
		Packet::cleanUp();
		_result = EM_ENTER_INVALID;
	}

public:
	EM_ENTER_RET	getResult( ){ return static_cast<EM_ENTER_RET>(_result); }
	void	setResult(EM_ENTER_RET ret){ _result = static_cast<int>(ret); }

private:
	int	 _result;
};


class SCRetEnterScenePacketFactory : public PacketFactory 
{
public:
	Packet_ID	getPacketID() const { return PACKET_SC_RET_ENTER_SCENE; }
	int		getPacketMaxSize() const 
	{
		return sizeof(SCRetEnterScenePacket) - sizeof(Packet); 
	}
private:
	Packet*	__create() 
	{ 
		Packet* pPacket = new SCRetEnterScenePacket();
		return pPacket; 
	}
};


class SCRetEnterScenePacketHandler 
{
public:
#ifdef _SERVER_
	static int	execute(SCRetEnterScenePacket* pPacket, Player* pPlayer){ return PACKET_EXE_ERROR; }
#else
	static int	execute(SCRetEnterScenePacket* pPacket, Player* pPlayer);
#endif
};

#endif
