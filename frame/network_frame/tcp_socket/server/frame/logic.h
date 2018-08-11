#ifndef _LOGIC_H
#define _LOGIC_H

#include "type.h"
#include "thread.h"
#include "macrodef.h"
#include "singleton.h"
#include "time_system.h"
#include "scene.h"

class GamePlayerMgr;

/**************************************************
	接入线程
**************************************************/
class IncomingThread
{
public :
	IncomingThread() : _pGamePlayerMgr(NULL) {}
	~IncomingThread() { SAFE_DELETE(_pGamePlayerMgr); }

	bool	init(int nPlayerCount = 1024);
	void	start() { _thread.start(&IncomingThread::run, this); }
	void	wait() { _thread.wait();}
	void	do_service();
	static void* run(void* param)
	{
		IncomingThread* p = (IncomingThread*)param;
		if (p) { p->do_service();}
		return NULL;
	}

private :
	GamePlayerMgr* _pGamePlayerMgr;
	Thread _thread;
};


/**************************************************
	逻辑线程
**************************************************/
class LogicThread
{
public:
	LogicThread() : _pScene(NULL) {}
	~LogicThread();
	
	int		getload() const { MyAssert(_pScene); return _pScene->getPlayerCount(); }
	Scene*	getScene() const { return _pScene; }
	void	init(int index);
	void	start() { _thread.start(&LogicThread::run, this); }
	void	wait() { _thread.wait();}
	void	do_service();
	static void* run(void* param);

private:
	static const int kMaxPlayerCount = 1024;
	Scene* _pScene;
	Thread _thread;
};

class LogicThreadMgr : public Singleton<LogicThreadMgr>
{
public:
	LogicThreadMgr() : _nThreadsCount(0) {}

	bool	init();
	bool	start();
	bool	wait();
	const LogicThread*	getMinloadThread() const;
	const LogicThread* getThread(int index) const;

private:
	static const int	kMaxLogicThreadsCount = 8;
	LogicThread	_threads[kMaxLogicThreadsCount];
	int	 _nThreadsCount;
};

#endif
