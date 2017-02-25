/**********************************
** @file: shmdef.h
** @brief:  �����ڴ���ض���
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

typedef  uint	SM_Key; // ShareMemory��ֵ����

#if defined(_WIN32)
#define	INVALID_SM_HANDLE	((void*)0)

#elif defined(_LINUX64)
#define	INVALID_SM_HANDLE	(-1)

#endif

enum
{
	SM_USE_INVALID = -1,
	SM_USE_FREE,				//--0, ���У�δ���κν�ɫռ��
	SM_USE_READYFREE,		//--1, ׼������
	SM_USE_FREED,			//--2, �����ߣ����ݱ������
	SM_USE_HOLDDATA,		//--3, ����
	SM_USE_NUMBER,
};

enum
{
	SM_INVALID = -1,
	SM_FREE,					// --0, �����ڴ����
	SM_C_READ,				// --1, �����ڴ��Լ���ȡ
	SM_C_WRITE,			// --2, �����ڴ��Լ�д
	SM_S_READ,				// --3, Server��
	SM_S_WRITE,			// --4, Serverд
	SM_NUMBER,
};

//const X_INT HUMAN_KEY = 1000;
//const X_INT SERIAL_KEY = 1001;
//const X_INT ARENA_KEY = 1002;
//const X_INT FIGHT_KEY = 1003;
//const X_INT MAIL_KEY = 1004;
const int MAX_SERVER_IDLE_TIME = 5*60*1000;  //������ֹͣ��Ӧʱ��(����)
const int kUserHoldTime = 5 * 60 * 1000;
const int kUserSaveInterval = 60 * 1000;

//�����ڴ�����ڷ���������
enum SMPOOL_TYPE
{
	SM_ShareMemory,
	SM_LogicServer,
};

//���洢��������
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

