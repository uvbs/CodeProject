#include "socketdef.h"
#include "logic.h"
#include "assert.h"
#include "game_player_mgr.h"
#include "helper.h"
#include "log.h"
#include "scene.h"

/**************************************************
	接入线程
**************************************************/
bool IncomingThread::init(int nPlayerCount /*= 1024*/)
{
	__ENTER_FUNCTION

	_pGamePlayerMgr = GamePlayerMgr::getSinglePtr();
	Assert(_pGamePlayerMgr);
	bool ret = _pGamePlayerMgr->init(nPlayerCount, "127.0.0.1", 8888) ;
	Assert(ret);

	return true;
	__LEAVE_FUNCTION
	return false ;
}

void IncomingThread::do_service()
{
	__ENTER_FUNCTION

	_pGamePlayerMgr->_tid = getCurrTid();
	bool ret = false ;
	uint uLastTime = TimeSystem::getSinglePtr()->getRunTime();
	while (true)
	{
		uint uTime = TimeSystem::getSinglePtr()->getRunTime();
		uint uDelta = uTime - uLastTime ;
		uLastTime = uTime ;
		if (uDelta >= 100) ;
		else { sys_util::sleep(100 - uDelta); }
		__MYTRY
		{
			ret = _pGamePlayerMgr->select(); Assert(ret);
			ret = _pGamePlayerMgr->processExceptions(); Assert(ret);
			ret = _pGamePlayerMgr->processInputs(); Assert(ret);
			ret = _pGamePlayerMgr->processOutputs(); Assert(ret);
			ret = _pGamePlayerMgr->processCmds(); Assert(ret);
			ret = _pGamePlayerMgr->processCacheCmds(); Assert(ret);
			ret = _pGamePlayerMgr->heartBeat(uTime); Assert(ret);
		}
		__MYCATCH
		{
			_pGamePlayerMgr->relive();
			MyExceptionLog();
		}
	}
	_pGamePlayerMgr->kickAllPlayers();

	__LEAVE_FUNCTION
}

/**************************************************
	逻辑线程
**************************************************/
int g_logic_frametime_max		= 0;
int g_logic_frametime_min		= 0x7FFFFFFF;
uint64 g_logic_frametime_total	= 0;
int g_logic_frametime_average	= 0;
int g_logic_framecount	= 0;

void	LogicThread::init(int index) 
{
	_pScene = new Scene(index);
	Assert(_pScene);
	_pScene->init(kMaxPlayerCount);
}

LogicThread::~LogicThread()
{
	SAFE_DELETE(_pScene);
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
	_pScene->setTid(getCurrTid());
	uint uLastTime = TimeSystem::getSinglePtr()->getRunTime();
	while (true)
	{
		uint uCurrTime = TimeSystem::getSinglePtr()->getRunTime();
		uint uDelta = uCurrTime - uLastTime ;
		uLastTime = uCurrTime ;
		g_logic_framecount++;
		g_logic_frametime_total += uDelta;
		if (static_cast<int>(uDelta) > g_logic_frametime_max)
		{
			g_logic_frametime_max = uDelta;
		}
		if (static_cast<int>(uDelta) < g_logic_frametime_min)
		{
			g_logic_frametime_min = uDelta;
		}
		g_logic_frametime_average = g_logic_frametime_total /  g_logic_framecount;
		if (g_logic_framecount % 100 == 0)
		{
			LogSystem::getSinglePtr()->cacheLog( LOG_FILE_DEBUG, "logicthread frame info: max=[%d] min=[%d] average=[%d] count=[%d]",
				g_logic_frametime_max, g_logic_frametime_min, g_logic_frametime_average, g_logic_framecount);
		}
		if (uDelta >= 100) ;
		else { sys_util::sleep(100 - uDelta); }
		__MYTRY
		{
			bool ret = _pScene->tick(uCurrTime);
			Assert(ret);
		}
		__MYCATCH
		{
			MyExceptionLog();
		}	
	}
	__LEAVE_FUNCTION
}

WX_IMPLEMENT_SINGLEON(LogicThreadMgr);

//初始化
bool LogicThreadMgr::init()
{
	_nThreadsCount = kMaxLogicThreadsCount;
	Assert(_nThreadsCount > 0);
	//int count = SceneMgr::getSinglePtr()->getSceneCount();
	//for (int i = 0; i < count; ++i)
	//{
	//	Scene* pScene = SceneMgr::getSinglePtr()->getSceneByIndex(i);
	//	Assert(pScene);
	//	_threads[i % _nThreadsCount].addScene(pScene);
	//}
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

const LogicThread* LogicThreadMgr::getThread(int index) const
{
	if (index < 0 || index >= _nThreadsCount) { return NULL;}
	return &_threads[index];
}

const LogicThread*  LogicThreadMgr::getMinloadThread() const
{
	__ENTER_FUNCTION
	int minIndex = 0;
	for (int i = 1; i < _nThreadsCount; ++i)
	{
		if (_threads[i].getload()  < _threads[minIndex].getload())
		{
			minIndex = i;
		}
	}
	return &_threads[minIndex];
	__LEAVE_FUNCTION
	return NULL;
}
