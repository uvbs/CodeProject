#ifdef _WIN32
#include "socket.h"
#endif
#include "shm_server.h"
#include "helper.h"

/**************************************************
	ShmServer
**************************************************/
ShmServer::ShmServer() : _time_system(NULL), _log_system(NULL) 
{}
ShmServer::~ShmServer()
{
	SAFE_DELETE(_time_system);
	SAFE_DELETE(_log_system);
}

bool	ShmServer::init()
{
	__ENTER_FUNCTION
    //ʱ��ϵͳ
	_time_system = TimeSystem::getSinglePtr();
	Assert(_time_system);
	_time_system->start();
    //��־ϵͳ
	_log_system = LogSystem::getSinglePtr();
	Assert(_log_system);
	bool bRet = _log_system->init();
    Assert(bRet);
	//SMU Pool Thread
    bRet = _thread.init();
    Assert(bRet);
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "smu pool thread init OK.");
	
	return true;
	__LEAVE_FUNCTION
	return false;
}

bool	ShmServer::start()
{
    _thread.start();
    return true;
}

bool	ShmServer::wait()
{
    _thread.wait();
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "Exit ShmServer OK.");
	return true;
}