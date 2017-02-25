////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_time.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 0:55
// @brief: 
//
///////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_time.h"
#include <iostream>
using std::cout;
using std::endl;

void test_time()
{
	uint curHourMinute = TimeSystem::getSinglePtr()->getCurrHourMinute();
	uint day = TimeSystem::getSinglePtr()->getAfterNDays(20140730, 3);
	Timer tick;
	tick.beginTimer(500, TimeSystem::getSinglePtr()->getRunTime());
	while (true)
	{
		cout << "time wait" ;//<< endl;
		if (tick.isReach(TimeSystem::getSinglePtr()->getRunTime()))
		{
			cout << "time is up" << endl;
			tick.endTimer();
			break;
		}
	}

}
