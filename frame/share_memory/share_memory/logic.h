/***************************************************
** @file: logic.h
** @author: Mr.Chen
** @date: 2016/7/22
** @brief: Logicœﬂ≥Ã
***************************************************/
#ifndef _LOGIC_H
#define _LOGIC_H

#include "macrodef.h"
#include "shm_def.h"
#include "log.h"
#include "share_memory_pool.h"
#include "thread.h"


extern bool g_exit;

struct ShmPoolHandler_T
{
    ShmPoolHandler_T() : _key(0), _type(SMU_INVALID), _pool(NULL), _handler(NULL) {}
    SM_Key	 _key;
    SMU_TYPE	_type;
    void* _pool;
    void* _handler;
};

class LogicThread
{
public:
    LogicThread();
    ~LogicThread();

    bool	init();
    void	start() { _thread.start(&LogicThread::run, this); }
    void	wait() { _thread.wait();}
    void	do_service();
    static void* run(void* param);
	void	save(bool forceall = false);
    SM_COMMANDS checkCmd();
	bool	checkExit();

public:
	static	 void* handler(void* param);
#if defined (_WIN32)
	static bool WINAPI closeHandler(DWORD type);
#elif defined (_LINUX64)
	static void closeHandler(int signo);
#endif

private:
    Thread _thread;
    ShmPoolHandler_T _shm[SMU_NUMBER];
};



#endif	//_LOGIC_H
