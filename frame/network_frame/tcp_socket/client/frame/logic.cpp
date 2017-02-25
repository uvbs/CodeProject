#include "socketdef.h"
#include "logic.h"
#include "assert.h"
#include "helper.h"
#include "log.h"
#include "robot.h"

/**************************************************
	逻辑线程
**************************************************/
LogicThread::LogicThread()
{
	for (int i = 0; i < kMaxPlayerCount; ++i)
	{
		_pRobot[i] = NULL;
	}
}

void	LogicThread::init(int index) 
{
	for (int i = 0; i < kMaxPlayerCount; ++i)
	{
		int userid = index * 100 + i;
		_pRobot[i] = new Robot(userid);
		Assert(_pRobot[i]);
		_pRobot[i]->init();
	}
}

LogicThread::~LogicThread()
{
	for (int i = 0; i < kMaxPlayerCount; ++i)
	{
		SAFE_DELETE(_pRobot[i]);
	}
}

void* LogicThread::run(void* param)
{
	LogicThread* p = (LogicThread*)param;
	if (p) { p->do_service();}
	return NULL;
}

void	LogicThread::do_service()
{
	__ENTER_FUNCTION
	while (true)
	{
		for (int i = 0; i < kMaxPlayerCount; i++)
		{
			if (!_pRobot[i]) continue;
			__MYTRY
			{
				uint uTime = TimeSystem::getSinglePtr()->getRunTime();
				bool ret = _pRobot[i]->select(); Assert(ret);
				ret = _pRobot[i]->processExceptions(); Assert(ret);
				ret = _pRobot[i]->processInputs(); Assert(ret);
				ret = _pRobot[i]->processOutputs(); Assert(ret);
				ret = _pRobot[i]->processCmds(); Assert(ret) ;
				ret = _pRobot[i]->heartBeat(uTime); Assert(ret);
			}
			__MYCATCH
			{
				MyExceptionLog();
			}
		}
		sys_util::sleep(100);
	}
	__LEAVE_FUNCTION
}

WX_IMPLEMENT_SINGLEON(LogicThreadMgr);

//初始化
bool LogicThreadMgr::init()
{
	_nThreadsCount = kMaxLogicThreadsCount;
	Assert(_nThreadsCount > 0);
	for (int i = 0; i < _nThreadsCount; ++i)
	{
		_threads[i].init(i);
	}
	return true;
}

bool LogicThreadMgr::start()
{
	for (int i = 0; i < _nThreadsCount; i++)
	{
		_threads[i].start();
	}
	return true;
}

bool LogicThreadMgr::wait()
{
	for (int i = 0; i < _nThreadsCount; i++)
	{
		_threads[i].wait();
	}
	return true;
}
