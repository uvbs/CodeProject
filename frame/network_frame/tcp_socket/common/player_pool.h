////////////////////////////////////////////////////////////////////////////////////
// 
// @file: player_pool.h
// @author: by robert
// @date: 2014/11/3	17:30
// @brief: ���ӳ�
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

	//��ʼ��
	bool init(int count)
	{
		Assert(count > 0);
		_capacity = count;
		_pPlayerPool = new Player_T[_capacity];
		return true;
	}

	//������ҵ�PlayerIDȡ������ָ��
	Player_T* getPlayer(int playerId)
	{
		Assert(_pPlayerPool);
		if (playerId < 0 || playerId >= _capacity)
		{
			return NULL;
		}
		return &_pPlayerPool[playerId];
	}

	//�ӿ������ݳ������һ���������
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

	//��һ�����������ջ���
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
	MLock _lock ;	//��
	Player_T* _pPlayerPool; //��
	int _capacity;
};

#endif	// __PLAYER_POOL_H_

