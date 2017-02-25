#include "game_player.h"
#include "player_pool.h"
#include "time_system.h"
#include "player_pool.h"
#include <string.h>

WX_IMPLEMENT_SINGLEON(PlayerPool<GamePlayer>);

GamePlayer::GamePlayer(bool isAsSocketCli) : Player(isAsSocketCli)
{
	_status = EM_PLAYER_STATUS_IDLE ;
	memset(_userId, 0, sizeof(_userId));
}

GamePlayer::~GamePlayer()
{
	_status = EM_PLAYER_STATUS_INVALID;
	memset(_userId, 0, sizeof(_userId));
}

bool	GamePlayer::init()
{
	uint time = TimeSystem::getSinglePtr()->getRunTime();
	_kickTimer.beginTimer(2 * 60 * 1000, time);
	return true ;
}

void	GamePlayer::cleanUp()
{
	Player::cleanUp() ;
	_status = EM_PLAYER_STATUS_IDLE;
	memset(_userId, 0, sizeof(_userId));
	_kickTimer.endTimer();
}

bool	GamePlayer::isIdle()
{
	if (_status == EM_PLAYER_STATUS_IDLE && _poolID < 0)
	{
		return true;
	}
	return false;
}

bool	GamePlayer::heartBeat(uint uTime, int nFlag)
{
	if (_kickTimer.isSet() && _kickTimer.isReach(uTime))
	{
		_kickTimer.endTimer();
		return false; //Ìßµô
	}
	return true;
}

bool	GamePlayer::free()
{
	PlayerPool<GamePlayer>::getSinglePtr()->recyclePlayer(getPlayerID());
	return true ;
}

void GamePlayer::resetKickTime( )
{
	_kickTimer.endTimer();
	_kickTimer.beginTimer(2*60*1000, TimeSystem::getSinglePtr()->getRunTime());
}
