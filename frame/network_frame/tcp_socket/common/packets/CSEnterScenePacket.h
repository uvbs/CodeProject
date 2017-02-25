/***************************************************
** @file: CSEnterScenePacket.h
** @author: Mr.Chen
** @date: 2016/7/5
** @brief: 
***************************************************/
#ifndef _CS_ENTER_SCENE_PACKET_H
#define _CS_ENTER_SCENE_PACKET_H

#include "type.h"
#include "packet.h"
#include "packet_factory.h"
#include "packetdef.h"

class CSEnterScenePacket : public Packet 
{
public:
	CSEnterScenePacket() { cleanUp(); }

	//公用继承接口
	virtual bool	fill( SocketStream& ioStream , EM_TYPE_MSG_MODE mode);
	virtual int	execute(Player* pPlayer) ;
	virtual Packet_ID	getPacketID() const { return PACKET_CS_ENTER_SCENE; }
	virtual void	cleanUp()
	{
		Packet::cleanUp();
		_sceneId = -1;
	}

public:
	int		getSceneId() const { return _sceneId; }
	void	setSceneId(int id) { _sceneId = id ; }

private:
	int _sceneId;  //即线程索引
};


class CSEnterScenePacketFactory : public PacketFactory 
{
public:
	Packet_ID	getPacketID() const { return PACKET_CS_ENTER_SCENE; }
	int		getPacketMaxSize() const 
	{
		return sizeof(CSEnterScenePacket) - sizeof(Packet); 
	}
private:
	Packet*	__create() 
	{ 
		Packet* pPacket = new CSEnterScenePacket();
		return pPacket; 
	}
};


class CSEnterScenePacketHandler 
{
public:
#ifdef _SERVER_
	static int execute(CSEnterScenePacket* pPacket, Player* pPlayer) ;
#else
	static int execute(CSEnterScenePacket* pPacket, Player* pPlayer){ return PACKET_EXE_ERROR; }
#endif
};


#endif	//_CS_ENTER_SCENE_PACKET_H
