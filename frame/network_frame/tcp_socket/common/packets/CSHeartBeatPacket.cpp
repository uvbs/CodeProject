#include "CSHeartBeatPacket.h"

bool CSHeartBeatPacket::fill(SocketStream& ioStream, EM_TYPE_MSG_MODE mode) 
{ return true; }

int CSHeartBeatPacket::execute(Player* pPlayer)
{
	__ENTER_FUNCTION
	return CSHeartBeatPacketHandler::execute(this, pPlayer);
	__LEAVE_FUNCTION
	return false;
}
