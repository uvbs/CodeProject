/**********************************
** @file: shmdef.h
** @brief:  共享内存相关定义
** @autor: by Mr.Chen
** @date: 2016/07/10 22:17
**********************************/
#ifndef _SHM_DEF_H
#define _SHM_DEF_H

#include "type.h"
#include "bitflags.h"
#include "timer.h"

#if defined(_WIN32)
typedef	void*		SM_Handle;

#elif defined(_LINUX64)
typedef	int		SM_Handle;
#endif

typedef  uint	SM_Key; // ShareMemory键值类型

#if defined(_WIN32)
#define	INVALID_SM_HANDLE	((void*)0)

#elif defined(_LINUX64)
#define	INVALID_SM_HANDLE	(-1)

#endif

enum
{
	SM_USE_INVALID = -1,
	SM_USE_FREE,				//--0, 空闲，未被任何角色占用
	SM_USE_READYFREE,		//--1, 准备下线
	SM_USE_FREED,			//--2, 已下线，数据保存完毕
	SM_USE_HOLDDATA,		//--3, 在线
	SM_USE_NUMBER,
};

enum
{
	SM_INVALID = -1,
	SM_FREE,					// --0, 共享内存空闲
	SM_C_READ,				// --1, 共享内存自己读取
	SM_C_WRITE,			// --2, 共享内存自己写
	SM_S_READ,				// --3, Server读
	SM_S_WRITE,			// --4, Server写
	SM_NUMBER,
};

//const X_INT HUMAN_KEY = 1000;
//const X_INT SERIAL_KEY = 1001;
//const X_INT ARENA_KEY = 1002;
//const X_INT FIGHT_KEY = 1003;
//const X_INT MAIL_KEY = 1004;
const int MAX_SERVER_IDLE_TIME = 5*60*1000;  //服务器停止响应时间(毫秒)
const int kUserHoldTime = 5 * 60 * 1000;
const int kUserSaveInterval = 60 * 1000;

//共享内存池所在服务器类型
enum SMPOOL_TYPE
{
	SM_ShareMemory,
	SM_LogicServer,
};

//所存储数据类型
enum SMU_TYPE
{
	SMU_INVALID = -1,
	HUMAN_SMU,
	SERIAL_SMU,
	MAIL_SMU,
	SMU_NUMBER,
};

enum  SM_COMMANDS
{
	CMD_INVALID = -1,
	CMD_UNKNOW,
	CMD_SAVE_ALL,
	CMD_CLEAR_ALL,
	CMD_NUMBER,
};

#endif	//_SHM_DEF_H

