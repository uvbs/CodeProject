// CompletionPort.cpp : 定义控制台应用程序的入口点。
//
/*!<
	在你的应用程序需要同时管理数百乃至上千个套接字的时候，
    而且希望随着系统内安装的CPU数量的增多，
    应用程序的性能也可以线性提升，才应考虑采用“完成端口”模型。
	
*/

#ifndef _COMPLETION_PORT_H
#define _COMPLETION_PORT_H

#if defined (_WIN32)

#include "type.h"
#include "macrodef.h"
#include "socketdef.h"
#include "thread.h"

namespace completion_port
{

typedef enum
{
	RECV_POSTED
}OPERATION_TYPE;

typedef struct
{
	WSAOVERLAPPED  overlap;
	WSABUF         Buffer;
	char					  szMessage[MSGSIZE];
	DWORD          NumberOfBytesRecvd;
	DWORD          Flags;
	OPERATION_TYPE OperationType;
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

void* WorkerThread(void* );

int startSrv();

} //namespace completion_port

#endif // _WIN32

#endif //_COMPLETION_PORT_H
