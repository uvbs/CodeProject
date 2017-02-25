// SelectMultiThread.cpp : 定义控制台应用程序的入口点。
//
// 选择模型 +  多线程

#ifndef _MULTI_SELECT_SERVER_
#define _MULTI_SELECT_SERVER_

#if defined (_WIN32)

#include "type.h"
#include "macrodef.h"
#include "socketdef.h"
#include "thread.h"

namespace select_multi_thread_server
{

// 开辟的最大线程数
const int MAX_THREADS_NUM = 2;
// 每个线程处理的客户端数
const int MAX_CLIENTS_NUM_PER_THREAD = 2;

// 工作线程函数
void* WorkerThread(void* lpParameter);

int startSrv();

} //namespace select_multi_thread_server
using namespace select_multi_thread_server;

#endif //(_WIN32)

#endif  //_MULTI_SELECT_SERVER_
