// event_select.h : 定义控制台应用程序的入口点。
//
/*
Winsock提供了另一个有用的异步I/O模型。
和WSAAsyncSelect模型类似的是，它也允许
应用程序在一个或多个套接字上，接收以事件
为基础的网络事件通知。对于由WSAAsyncSelect模型
采用的网络事件来说，它们均可原封不动地移植到新模型。
在用新模型开发的应用程序中，也能接收和处理所有那些事件。
该模型最主要的差别在于网络事件会投递至一个事件对象句柄，
而非投递至一个窗口例程。选自《Windows网络编程》。
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
