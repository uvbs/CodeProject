////////////////////////////////////////////////////////////////////////////////////
// 
// @file: timer.h
// @author: by Mr.Chen
// @date: 2014/8/3	18:39
// @brief: 定时器
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __TIMER_H_
#define __TIMER_H_

#include "type.h"

namespace sys_util {

//计时器
class Timer
{
public:
	Timer();

	bool	isSet() const { return _isSet; }				//是否有效
	uint	getStartTime() const { return _startTime; }	//获得开始时间
	void	setTerm(uint nTerm){ _term = nTerm; }		//设置有效时间
	uint	getTerm() const { return _term; }				//获得有效时间
		
	bool	beginTimer(uint nTerm, uint nNowTime);	//开始计时
	void	endTimer();											//结束计时
	bool	isReach(uint uNowTime) ;				//是否时间到

	Timer& operator = (const Timer& rhs)
	{
		_startTime	= rhs.getStartTime();
		_term		= rhs.getTerm();
		_isSet		= rhs.isSet();
		return *this;
	};

private:
	uint	_startTime;		//开始时间
	uint	_term;				//有效时间
	bool	_isSet;				//是否有效
};

} // namespace sys_util
using namespace sys_util;

#endif // __TIMER_H_
