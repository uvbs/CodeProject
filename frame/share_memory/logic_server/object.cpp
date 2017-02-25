#include "object.h"

Object::Object() : _create_time(0), _last_time(0), _curr_time(0),
	m_PoolID(-1), m_ObjectID(-1) {}

Object::~Object()
{
	cleanUp();
}

bool	Object::init()
{
	_attr_fresh_timer.beginTimer(1000, _create_time);
	return true;
}

void	Object::cleanUp()
{
	_create_time = 0;
	_last_time = 0;
	_curr_time = 0;
	m_ObjectID  = -1;
}

bool	Object::heartBeat(uint uTime)
{
	if (_curr_time == 0) 
	{
		_create_time = uTime;
		_last_time   = uTime;
	}
	else
	{
		_last_time = _curr_time;
	}
	_curr_time = uTime;
	if (_attr_fresh_timer.isSet() && _attr_fresh_timer.isReach(uTime))
	{
		sendFreshAttrs();
	}
	return true;
}
