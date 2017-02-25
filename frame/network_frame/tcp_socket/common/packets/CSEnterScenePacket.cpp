#include "CSEnterScenePacket.h"

bool	CSEnterScenePacket::fill(SocketStream& ioStream, EM_TYPE_MSG_MODE mode) 
{
	ioStream.fill((char*)(&_sceneId), sizeof(_sceneId), mode, _size) ;
	return true;
}

int CSEnterScenePacket::execute(Player* pPlayer)
{
	__ENTER_FUNCTION

	return CSEnterScenePacketHandler::execute(this, pPlayer) ;

	__LEAVE_FUNCTION
	return PACKET_EXE_ERROR;
}
