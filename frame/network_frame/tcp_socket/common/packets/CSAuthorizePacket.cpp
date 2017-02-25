#include "CSAuthorizePacket.h"

bool CSAuthorizePacket::fill(SocketStream& ioStream , EM_TYPE_MSG_MODE mode) 
{
	ioStream.fill(_userId, sizeof(_userId), mode, _size) ;
	ioStream.fill(_key, sizeof(_key), mode, _size) ;

	return true;
}

int	 CSAuthorizePacket::execute(Player* pPlayer)
{
	__ENTER_FUNCTION
	return CSAuthorizePacketHandler::execute(this, pPlayer);
	__LEAVE_FUNCTION
	return PACKET_EXE_ERROR ;
}
