// completion_overlap.h : �������̨Ӧ�ó������ڵ㡣
//
/* ! <
	��������̷�ʽʵ�ֵ��ص�I/Oģ��
	�����������ʵ���ص�I/O�����¼�֪ͨ�򵥵öࡣ�����ģ���У����߳�ֻ�ò�ͣ�Ľ������Ӽ��ɣ�
	�����߳��ж���û���µĿͻ������ӱ�����������У���Ϊ�Ǹ��ͻ����׽��ּ���һ���첽��WSARecv������
	Ȼ�����SleepExʹ�̴߳���һ�ֿɾ���ĵȴ�״̬����ʹ��I/O��ɺ�CompletionROUTINE���Ա��ں˵��á�
	��������̲߳�����SleepEx�����ں������һ��I/O�������޷�����������̣���Ϊ������̵�����Ӧ��
	�͵�������WSARecv�첽�����Ĵ�����ͬһ���߳�֮�ڣ�����������ڵ�ʵ�ִ���Ƚϼ򵥣���ȡ�����յ������ݣ�Ȼ������ԭ�ⲻ���ķ��͸��ͻ��ˣ��������
	������һ��WSARecv�첽������
	ע�⣺�������õ��ˡ�β�����ݡ��������ڵ���WSARecv��ʱ�򣬲���lpOverlappedʵ����ָ��һ��������ö�
	�ĽṹPER_IO_OPERATION_DATA������ṹ����WSAOVERLAPPED���⣬�������Ǹ����˻������Ľṹ��Ϣ��
	���⻹�����ͻ����׽��ֵ���Ҫ����Ϣ�������������������ͨ������lpOverlapped�õ��Ĳ�������
	WSAOVERLAPPED�ṹ�����к��β��İ����ͻ����׽��ֺͽ������ݻ���������Ҫ��Ϣ�������Һ������
	��ɶ˿ڵ�ʱ�򻹻�ʹ�õ���
*/

#ifndef _COMPLETION_OVERLAP_
#define _COMPLETION_OVERLAP_

#ifdef _WIN32

#include "type.h"
#include "macrodef.h"
#include "socketdef.h"
#include "thread.h"

namespace completion_overlap
{

typedef struct
{
	WSAOVERLAPPED overlap;
	WSABUF        Buffer;
	char					 szMessage[MSGSIZE];
	DWORD         NumberOfBytesRecvd;
	DWORD         Flags; 
	SOCKET        sClient;
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

void* WorkerThread(void*);
void CALLBACK CompletionROUTINE(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
int startSrv();

} //namespace completion_overlap

#endif //_WIN32

#endif //#define _COMPLETION_OVERLAP_
