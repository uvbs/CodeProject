#include "socketdef.h"
#include "logic.h"
#include "myassert.h"
#include "helper.h"
#include "log.h"
#include "robot.h"

/**************************************************
	�߼��߳�
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
		MyAssert(_pRobot[i]);
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
				bool ret = _pRobot[i]->select(); MyAssert(ret);
				ret = _pRobot[i]->processExceptions(); MyAssert(ret);
				ret = _pRobot[i]->processInputs(); MyAssert(ret);
				ret = _pRobot[i]->processOutputs(); MyAssert(ret);
				ret = _pRobot[i]->processCmds(); MyAssert(ret) ;
				ret = _pRobot[i]->heartBeat(uTime); MyAssert(ret);
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

//��ʼ��
bool LogicThreadMgr::init()
{
	_nThreadsCount = kMaxLogicThreadsCount;
	MyAssert(_nThreadsCount > 0);
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
