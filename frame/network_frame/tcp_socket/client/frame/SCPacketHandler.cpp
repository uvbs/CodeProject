/***************************************************
** @file: SCPacketHandler.cpp
** @author: Mr.Chen
** @date: 2016/7/5
** @brief: 
***************************************************/
#include "SCRetAuthorizePacket.h"
#include "CSEnterScenePacket.h"
#include "SCRetEnterScenePacket.h"
#include "robot.h"
#include "log.h"
#include <stdio.h>

int	 SCRetAuthorizePacketHandler::execute(SCRetAuthorizePacket* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION
	Assert(pPacket);
	Assert(pPlayer);
	Robot* pHuman = (Robot*)pPlayer;
	pHuman->_status = EM_PLAYER_STATUS_AUTHORIZED;

	CSEnterScenePacket packet;
	//packet.setSceneId(0);
	pHuman->sendPacket(&packet);

	LogSystem::getSinglePtr()->cacheLog(LOG_FILE_DEBUG, "SCRetAuthorizePacket execute OK.");
	return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
	return PACKET_EXE_ERROR;
}

int SCRetEnterScenePacketHandler::execute(SCRetEnterScenePacket* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION
	Assert(pPacket);
	Assert(pPlayer);

	Robot* pHuman = static_cast<Robot*>(pPlayer);
	switch (pPacket->getResult())
	{
	case SCRetEnterScenePacket::EM_ENTER_SUCCESS:
		{
			pHuman->_status = EM_PLAYER_STATUS_NORMAL;
			printf("enter success, user = %s\n", pHuman->getUserId());
		}
		break;
	default:
		break;
	}

	LogSystem::getSinglePtr()->cacheLog(LOG_FILE_DEBUG, 
		"SCRetEnterScenePacket execute OK, user = %s", pHuman->getUserId());

	return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
		return PACKET_EXE_ERROR;
}
