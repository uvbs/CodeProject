#include "SCRetEnterScenePacket.h"
#include "assert.h"

bool SCRetEnterScenePacket::fill(SocketStream& ioStream , EM_TYPE_MSG_MODE mode) 
{
	ioStream.fill((char*)(&_result), sizeof(_result), mode, _size);
	return true;
}

int	 SCRetEnterScenePacket::execute(Player* pPlayer)
{
	__ENTER_FUNCTION
	return SCRetEnterScenePacketHandler::execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
	return PACKET_EXE_ERROR;
}
