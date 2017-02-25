#include "timer.h"
#include "assert.h"

namespace sys_util {

Timer::Timer()
{
	_isSet		= false;
	_startTime		= 0;
	_term			= 0;
}

bool	Timer::beginTimer(uint nTerm, uint nNowTime)
{
	__ENTER_FUNCTION

	_isSet = true;
	_startTime = nNowTime;
	_term		= nTerm;

	return true;
	__LEAVE_FUNCTION
	return false;
}

void	Timer::endTimer()
{
	__ENTER_FUNCTION
	
	_isSet		= false;
	_startTime		= 0;
	_term			= 0;
	
	__LEAVE_FUNCTION
}

bool Timer::isReach(uint uNowTime)
{
	__ENTER_FUNCTION
	Assert(_isSet);

	uint nElapsedTime = 0;
	if (uNowTime >= _startTime)
	{
		nElapsedTime = uNowTime - _startTime;
	}
	else
	{
		nElapsedTime = (uint(-1) - _startTime) + uNowTime;
	}

	if (nElapsedTime < _term)
	{
		return false;
	}

	_startTime = uNowTime;

	return true;
	__LEAVE_FUNCTION
	return false;
}

} // namespace sys_util
