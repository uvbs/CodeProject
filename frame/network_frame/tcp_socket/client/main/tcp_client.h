
#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H

#include "type.h"
#include "logic.h"
#include "time_system.h"
#include "log.h"
#include "packet_factory_mgr.h"


class TcpSocketClient
{
public :
	~TcpSocketClient();
	bool	init();
	bool	start();
	bool	wait();

private :
	TimeSystem*	_pTimeSystem;
	LogSystem*	_pLogSystem;
	PacketFactoryMgr*	_pPacketFactoryMgr;
	LogicThreadMgr	_logicThreadsMgr;
};

#endif
