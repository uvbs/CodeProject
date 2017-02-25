
#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include "type.h"
#include "player_pool.h"
#include "game_player.h"
#include "logic.h"
#include "time_system.h"
#include "scene.h"
#include "packet_factory_mgr.h"

class LogSystem;

class TcpSocketServer
{
public :
	~TcpSocketServer();
	bool	init();
	bool	start();
	bool	wait();

private :
	TimeSystem*	_pTimeSystem;
	LogSystem*	_pLogSystem;
	PacketFactoryMgr*	_pPacketFactoryMgr;
	PlayerPool<GamePlayer>*	_pGamePlayerPool;
	
	IncomingThread	_incomingThread;
	LogicThreadMgr* _pLogicThreadsMgr;
};

#endif
