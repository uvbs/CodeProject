////////////////////////////////////////////////////////////////////////////////////
// 
// @file: player_pool.h
// @author: by robert
// @date: 2014/11/3	17:30
// @brief: 连接池
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __PLAYER_POOL_H_
#define __PLAYER_POOL_H_

#include "type.h"
#include "player.h"
#include "lock.h"
#include "singleton.h"
#include "macrodef.h"
#include "assert.h"

template<class Player_T>
class PlayerPool : public Singleton<PlayerPool<Player_T> >
{
public :
	PlayerPool() : _pPlayerPool(NULL), _capacity(0) {}
	~PlayerPool()
	{
		SAFE_DELETE_ARRAY(_pPlayerPool);
	}

	//初始化
	bool init(int count)
	{
		Assert(count > 0);
		_capacity = count;
		_pPlayerPool = new Player_T[_capacity];
		return true;
	}

	//根据玩家的PlayerID取得数据指针
	Player_T* getPlayer(int playerId)
	{
		Assert(_pPlayerPool);
		if (playerId < 0 || playerId >= _capacity)
		{
			return NULL;
		}
		return &_pPlayerPool[playerId];
	}

	//从空闲数据池里分配一个玩家数据
	Player_T* allocPlayer()
	{
		AutoMLock autoLock(_lock);
		for (int i = 0; i < _capacity; ++i)
		{
			if (_pPlayerPool[i].isIdle())
			{
				_pPlayerPool[i].cleanUp();
				_pPlayerPool[i].setPoolID(i);
				return &_pPlayerPool[i];
			}	
		}
		return NULL;
	}

	//将一个玩家数据清空回收
	bool recyclePlayer(int playerId)
	{
		AutoMLock autoLock(_lock);
		if (playerId < 0 || playerId >= _capacity)
		{
			return false;
		}
		_pPlayerPool[playerId].cleanUp();
		_pPlayerPool[playerId].setPoolID(-1);
		return true;
	}

private:
	MLock _lock ;	//锁
	Player_T* _pPlayerPool; //池
	int _capacity;
};

#endif	// __PLAYER_POOL_H_

