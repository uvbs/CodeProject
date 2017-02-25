// selectsrv.h : 定义控制台应用程序的入口点。
//
// 选择模型
/*
Select(选择)模型是Widows Sock中最常见的I/O模型。之所以称其为“Select模型”，
是由于它的“中心思想”便是利用select函数，实现对I/O的管理。最初设计该模型时，
主要面向的是某些使用UNIX操作系统的计算机，它们采用的是Berkeley套接字方案。
Select模型已集成到Widows sock 1.1中，它使那些想避免在套接字调用过程中被
无辜“锁定”的应用程序，采取一种有序的方式，同时进行对多个套接字的管理。
由于Winsock 1.1向后兼容于Berkeley套接字实施方案，所以假如有一个Berkeley套接字应用
使用了select函数，那么从理论角度讲，毋需对其进行任何修改，便可正常运行。
(节选自《Widows网络编程》第八章) 下面的这段程序就是利用选择模型实现的
Echo服务器的代码(已经不能再精简了)：
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
