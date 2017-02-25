////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_taskqueue.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 23:48
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_taskqueue.h"
#include "helper.h"

int test_taskqueue()
{
	int bQuit = false;
	test_task aserver;
	aserver.startTask();

	while (!bQuit)
	{
		aserver.dispatchTask();
		aserver.statisticTask();
		sys_util::sleep(60);
	}
	aserver.stopTask();

	return 0;
}
