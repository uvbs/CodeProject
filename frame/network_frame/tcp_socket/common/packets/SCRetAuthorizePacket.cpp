#include "SCRetAuthorizePacket.h"

int SCRetAuthorizePacket::execute(Player* pPlayer)
{
	__ENTER_FUNCTION
	return SCRetAuthorizePacketHandler::execute(this, pPlayer);
	__LEAVE_FUNCTION
	return PACKET_EXE_ERROR;
}
