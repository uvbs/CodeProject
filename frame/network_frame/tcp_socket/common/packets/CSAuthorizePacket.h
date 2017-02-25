/***************************************************
** @file: CSAuthorizePacket.h
** @author: Mr.Chen
** @date: 2016/7/4
** @brief: Client与Server建立链接后的第一个包
***************************************************/
#ifndef _CS_AUTHORIZE_PACKET_H
#define _CS_AUTHORIZE_PACKET_H

#include "type.h"
#include "packet.h"
#include "packetdef.h"
#include "packet_factory.h"
#include "common.h"
#include "string_util.h"
#include <string.h>

class CSAuthorizePacket : public Packet 
{
public:
	CSAuthorizePacket() { cleanUp(); }

	//公用继承接口
	virtual bool	fill(SocketStream& ioStream , EM_TYPE_MSG_MODE mode);
	virtual int	execute(Player* pPlayer);
	virtual Packet_ID	getPacketID() const { return PACKET_CS_AUTHORIZE; }
	virtual void	cleanUp()
	{
		Packet::cleanUp();
		memset(_userId,0,sizeof(_userId));
		memset(_key, 0, sizeof(_key));
	}

public:
	//使用数据接口
	const char*	getUserId() const { return _userId; }
	void	setUserId(const char* user) { str_util::strcpy_s(_userId, user, sizeof(_userId)); }

	const char*	getKey() { return _key; }
	void	setKey(const char* key) { str_util::strcpy_s(_key, key, sizeof(_key)); }

private:
	//数据
	char	_userId[MAX_USER_LEN];
	char	_key[32];
};

class CSAuthorizePacketFactory : public PacketFactory 
{
public:
	Packet_ID	getPacketID() const { return PACKET_CS_AUTHORIZE; }
	int		getPacketMaxSize() const
	{
		return sizeof(CSAuthorizePacket) - sizeof(Packet); 
	}
private:
	Packet*	__create()
	{ 
		Packet* pPacket = new CSAuthorizePacket();
		return pPacket; 
	}
};

class CSAuthorizePacketHandler 
{
public:
#ifdef _SERVER_
	static int execute( CSAuthorizePacket* pPacket, Player* pPlayer ) ;
#else
	static int execute( CSAuthorizePacket* pPacket, Player* pPlayer ){ return PACKET_EXE_ERROR; }
#endif
};

#endif	//_CS_AUTHORIZE_PACKET_H
