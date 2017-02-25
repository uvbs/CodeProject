// selectsrv.h : �������̨Ӧ�ó������ڵ㡣
//
// ѡ��ģ��
/*
Select(ѡ��)ģ����Widows Sock�������I/Oģ�͡�֮���Գ���Ϊ��Selectģ�͡���
���������ġ�����˼�롱��������select������ʵ�ֶ�I/O�Ĺ��������Ƹ�ģ��ʱ��
��Ҫ�������ĳЩʹ��UNIX����ϵͳ�ļ���������ǲ��õ���Berkeley�׽��ַ�����
Selectģ���Ѽ��ɵ�Widows sock 1.1�У���ʹ��Щ��������׽��ֵ��ù����б�
�޹�����������Ӧ�ó��򣬲�ȡһ������ķ�ʽ��ͬʱ���жԶ���׽��ֵĹ���
����Winsock 1.1��������Berkeley�׽���ʵʩ���������Լ�����һ��Berkeley�׽���Ӧ��
ʹ����select��������ô�����۽ǶȽ��������������κ��޸ģ�����������С�
(��ѡ�ԡ�Widows�����̡��ڰ���) �������γ����������ѡ��ģ��ʵ�ֵ�
Echo�������Ĵ���(�Ѿ������پ�����)��
*/

#ifndef _SELECT_SERVER_
#define _SELECT_SERVER_

#if defined (_WIN32)

#include "type.h"
#include "macrodef.h"
#include "socketdef.h"
#include "thread.h"

namespace select_server
{
   int ConditionFunc(LPWSABUF lpCallerId,LPWSABUF lpCallerData,LPQOS lpSQOS,LPQOS lpGQOS,
        LPWSABUF lpCalleeId,LPWSABUF lpCalleeData,GROUP FAR * g,DWORD dwCallbackData);

    void* WorkerThread(void* lpParam);

    int startSrv();

}  //namespace SelectServer

using namespace select_server;

#endif //(_WIN32)

#endif //#define _SELECT_SERVER_
