#include "helper.h"
#include "macrodef.h"
#include "logic.h"
#include "share_memory_pool.h"
#include "shm_logic_handler.h"
#include "shm_util.h"
#include "string_util.h"
#include <stdio.h>

#ifdef _LINUX64
#include <signal.h>
#endif

bool g_exit = false;

/**************************************************
	逻辑线程
**************************************************/
LogicThread::LogicThread()
{
///##################################
///###NEW_SMU宏
///##################################
#define NEW_SMU(TYPE, KEY, SMU) \
    _shm[TYPE]._key = KEY; \
    _shm[TYPE]._type = TYPE; \
    _shm[TYPE]._pool = new ShareMemPool<SMU>;\
    _shm[TYPE]._handler = new ShareMemLogicHandler<SMU>;\

    //从配置文件中读取SMO的类型和数量，再据此新建SHM处理机
    int humanKey = 0x1000;
    int serialKey = 0x2000;
    int mailKey = 0x3000;
    NEW_SMU(HUMAN_SMU, humanKey, HumanSMU);
    NEW_SMU(SERIAL_SMU, serialKey, SerialSMU);
    NEW_SMU(MAIL_SMU, mailKey, MailSMU);
}

bool	LogicThread::init() 
{
///##################################
///###INIT_SMU宏
///##################################
#define INIT_SMU(TYPE, SMU, COUNT) \
    ShareMemPool<SMU>* p##SMU##Pool = static_cast<ShareMemPool<SMU>*>(_shm[TYPE]._pool); \
    key = _shm[TYPE]._key; \
    ret = p##SMU##Pool->init(COUNT, key, SM_ShareMemory); MyAssert(ret);\
    ShareMemLogicHandler<SMU>* p##SMU##Handler = static_cast<ShareMemLogicHandler<SMU>*>(_shm[TYPE]._handler);\
    ret = p##SMU##Handler->init(p##SMU##Pool); MyAssert(ret);

    SM_Key key = 0;
    bool ret = false;
    INIT_SMU(HUMAN_SMU, HumanSMU, 1024);
    LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "Initialize HumanSMUPool OK.") ;
    INIT_SMU(SERIAL_SMU, SerialSMU, EM_OBJ_TYPE_NUM);
    LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "Initialize SerialIDSMUPool OK.") ;
    INIT_SMU(MAIL_SMU, MailSMU, 1024);
    LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "Initialize MailSMUPool OK.");

	// register signals
	//    // Ctrl + C or kill -2 pid
	//    signal(SIGINT, (void (*)(int))LoopInterrupt);
	//    // kill pid or kill -15 pid
	//    signal(SIGTERM, (void (*)(int))LoopInterrupt);
	//#ifdef SIGHUP
	//    signal(SIGHUP, SIG_IGN);
	//#endif
	//#ifdef SIGBREAK
	//    // Ctrl + Break in Windows
	//    signal(SIGBREAK, (void (*)(int))LoopInterrupt);
	//#endif
	//#ifdef SIGPIPE
	//    // ignore the SIGPIPE when Linux socket was closed abnormally
	//    signal(SIGPIPE, SIG_IGN);
	//#endif

#if defined (_WIN32)
	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)&closeHandler, true)) {}
#elif defined (_LINUX64)
	signal(SIGABRT, closeHandler);
	signal(SIGKILL,  closeHandler);
	signal(SIGSTOP, closeHandler);
	signal(SIGTERM, closeHandler);
	signal(SIGPIPE, closeHandler);
	signal(SIGSEGV, closeHandler);
#endif

    return ret;
}

LogicThread::~LogicThread()
{
///##################################
///###DEL_SMU宏
///##################################
#define DEL_SMU(TYPE, SMU) \
    ShareMemPool<SMU>* p##SMU##Pool = static_cast<ShareMemPool<SMU>*>(_shm[TYPE]._pool); \
    ShareMemLogicHandler<SMU>* p##SMU##Handler = static_cast<ShareMemLogicHandler<SMU>*>(_shm[TYPE]._handler);\
    ret = p##SMU##Pool->finalize(); MyAssert(ret);\
    SAFE_DELETE(p##SMU##Pool);\
    SAFE_DELETE(p##SMU##Handler);

    bool ret = false;
    DEL_SMU(HUMAN_SMU, HumanSMU);
    DEL_SMU(SERIAL_SMU, SerialSMU);
    DEL_SMU(MAIL_SMU, MailSMU);
}

void*	LogicThread::handler(void* param)
{
	LogicThread* p = (LogicThread*)param;
	if (p) { p->save(true);}
	return NULL;
}

void	LogicThread::save(bool forceall/* = false*/)
{
///##################################
///###HANDLE_SMU宏
///##################################
#define HANDLE_SMU(TYPE, SMU, CMD_TYPE) \
	ShareMemLogicHandler<SMU>* p##SMU##Handler = static_cast<ShareMemLogicHandler<SMU>* >(_shm[TYPE]._handler);\
	if (p##SMU##Handler)  { p##SMU##Handler->setCmdType(CMD_TYPE); \
	p##SMU##Handler->heartBeat(); }

	SM_COMMANDS cmd = CMD_UNKNOW;
	if (forceall) { cmd = CMD_SAVE_ALL; }
	else { cmd = checkCmd(); }

	HANDLE_SMU(HUMAN_SMU, HumanSMU, cmd);
	HANDLE_SMU(MAIL_SMU, MailSMU, cmd);
	HANDLE_SMU(SERIAL_SMU, SerialSMU, cmd);
}

void* LogicThread::run(void* param)
{
	LogicThread* p = (LogicThread*)param;
	if (p) { p->do_service();}
	return NULL;
}

SM_COMMANDS LogicThread::checkCmd()
{
    if (remove("save.cmd") == -1)
    {
        return CMD_UNKNOW;
    }
    return CMD_SAVE_ALL;
}

bool	LogicThread::checkExit()
{
	if (remove("exit.cmd") == -1)
	{
		return false;
	}
	return true;
}

#if defined (_WIN32)
bool WINAPI LogicThread::closeHandler(DWORD type)
{
	g_exit = true;
	switch(type)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_BREAK_EVENT:
		{
			//handler(this);
			break;
		}
	default:
		break;
	}
	return true;
}
#elif defined (_LINUX64)
void LogicThread::closeHandler(int signo) 
{
	g_exit = true;
	//handler(this);
}
#endif

void	LogicThread::do_service()
{
	while (!g_exit)
	{
		if (checkExit())
		{
			save(true);
			g_exit = true;
			LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "Get exit command.");
			break;
		}
		save();
		sys_util::sleep(100);
	}
}
