#ifndef _LOGIC_H
#define _LOGIC_H

#include "type.h"
#include "thread.h"
#include "macrodef.h"
#include "singleton.h"
#include "time_system.h"


/**************************************************
	Âß¼­Ïß³Ì
**************************************************/
class Robot;
class LogicThread
{
public:
	LogicThread();
	~LogicThread();
	
	void	init(int index);
	void	start() { _thread.start(&LogicThread::run, this); }
	void	wait() { _thread.wait();}
	void	do_service();
	static void* run(void* param);
	
private:
	static const int kMaxPlayerCount = 16;
	Robot* _pRobot[kMaxPlayerCount];
	Thread _thread;
};

class LogicThreadMgr : public Singleton<LogicThreadMgr>
{
public:
	LogicThreadMgr() : _nThreadsCount(0) {}

	bool	init();
	bool	start();
	bool	wait();

private:
	static const int	kMaxLogicThreadsCount = 4;
	LogicThread	_threads[kMaxLogicThreadsCount];
	int	 _nThreadsCount;
};

#endif
