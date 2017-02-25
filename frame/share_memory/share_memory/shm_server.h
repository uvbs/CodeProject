/**********************************
** @file: shm_server.h
** @brief:  SHMServerÖ÷º¯Êý
** @autor: by Mr.Chen
** @date: 2016/07/16 14:01
**********************************/
#ifndef _SHM_SERVER_H
#define _SHM_SERVER_H

#include "logic.h"
#include "time_system.h"
#include "log.h"

class ShmServer
{
public:
	ShmServer();
	~ShmServer();
	bool	init();
	bool	start();
	bool	wait();

private:
	TimeSystem* _time_system;
	LogSystem*  _log_system;
    LogicThread   _thread;
};

#endif	//_SHM_SERVER_H
