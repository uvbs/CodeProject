/**********************************
** @file: logic_server.h
** @brief:  
** @autor: by Mr.Chen
** @date: 2016/07/24 0:13
**********************************/
#ifndef _LOGIC_SERVER_H
#define _LOGIC_SERVER_H

#include "macrodef.h"
#include "log.h"
#include "time_system.h"
#include "object_human_pool.h"

class LogicServer
{
public :
	LogicServer();
	~LogicServer();

    bool  initUser(const char* user, const char* name, const char* dir, Guid& guid, FullUserData& value);

	bool	init();
	bool	loop();
	bool	exit();

private :
	TimeSystem* m_pTimeController;
	LogSystem* m_pLogSystem;
	ObjectHumanPool* m_pHumanPool;
	static int _s_serial;
};

#endif	//_LOGIC_SERVER_H

