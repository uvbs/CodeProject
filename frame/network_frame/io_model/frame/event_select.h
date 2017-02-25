// event_select.h : �������̨Ӧ�ó������ڵ㡣
//
/*
Winsock�ṩ����һ�����õ��첽I/Oģ�͡�
��WSAAsyncSelectģ�����Ƶ��ǣ���Ҳ����
Ӧ�ó�����һ�������׽����ϣ��������¼�
Ϊ�����������¼�֪ͨ��������WSAAsyncSelectģ��
���õ������¼���˵�����Ǿ���ԭ�ⲻ������ֲ����ģ�͡�
������ģ�Ϳ�����Ӧ�ó����У�Ҳ�ܽ��պʹ���������Щ�¼���
��ģ������Ҫ�Ĳ�����������¼���Ͷ����һ���¼���������
����Ͷ����һ���������̡�ѡ�ԡ�Windows�����̡���
*/

#ifndef _EVENT_SELECT_
#define _EVENT_SELECT_

#if defined (_WIN32)
#include "type.h"
#include "macrodef.h"
#include "socketdef.h"
#include "thread.h"

namespace event_select
{

void Cleanup(int index);

void* WorkerThread(void* lpParam);

int startSrv();

} //namespace EventSelect

#endif //_WIN32

#endif // _EVENT_SELECT_
