// eventOverlaph : �������̨Ӧ�ó������ڵ㡣
// 
/* ! < 
	���¼�֪ͨ��ʽʵ�ֵ��ص�I/Oģ��
	���ģ������������ģ�Ͳ�ͬ������ʹ��Winsock2�ṩ���첽I/O����WSARecv��
	�ڵ���WSARecvʱ, ָ��һ��WSAOVERLAPPED�ṹ��������ò��������ģ�Ҳ����˵���������̷��ء�
	һ�������ݵ����ʱ�򣬱�ָ����WSAOVERLAPPED�ṹ�е�hEvent��Signaled��
	������䣺g_CliEventArr[g_iTotalConn] = g_pPerIODataArr[g_iTotalConn]->overlap.hEvent��
	ʹ������׽����������WSAEVENT����Ҳ��Signaled������WSAWaitForMultipleEvents�ĵ��ò����ɹ����ء�
	��������Ӧ�����ľ����������WSARecv��ͬ��WSAOVERLAPPED�ṹΪ��������WSAGetOverlappedResult���Ӷ�
	�õ�����I/O���͵��ֽ����������Ϣ����ȡ�ý��յ����ݺ󣬰�����ԭ�ⲻ���ķ��͵��ͻ��ˣ�Ȼ������
	����һ��WSARecv�첽������
*/

#ifndef _EVENT_OVERLAP_
#define _EVENT_OVERLAP_

#if defined (_WIN32)

#include "type.h"
#include "macrodef.h"
#include "socketdef.h"
#include "thread.h"

namespace event_overlap
{
typedef struct
{
	WSAOVERLAPPED overlap;
	WSABUF Buffer;
	char szMessage[MSGSIZE];
	DWORD NumberOfBytesRecvd;
	DWORD Flags;
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

void* WorkerThread(void*);

void Cleanup(int);

int startSrv();

} //namespace event_overlap


#endif //(_WIN32)

#endif //_EVENT_OVERLAP_

