// CompletionPort.cpp : �������̨Ӧ�ó������ڵ㡣
//
/*!<
	�����Ӧ�ó�����Ҫͬʱ��������������ǧ���׽��ֵ�ʱ��
    ����ϣ������ϵͳ�ڰ�װ��CPU���������࣬
    Ӧ�ó��������Ҳ����������������Ӧ���ǲ��á���ɶ˿ڡ�ģ�͡�
	
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
