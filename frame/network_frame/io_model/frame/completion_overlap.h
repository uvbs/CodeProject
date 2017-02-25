// completion_overlap.h : 定义控制台应用程序的入口点。
//
/* ! <
	用完成例程方式实现的重叠I/O模型
	用完成例程来实现重叠I/O比用事件通知简单得多。在这个模型中，主线程只用不停的接受连接即可；
	辅助线程判断有没有新的客户端连接被建立，如果有，就为那个客户端套接字激活一个异步的WSARecv操作，
	然后调用SleepEx使线程处于一种可警告的等待状态，以使得I/O完成后CompletionROUTINE可以被内核调用。
	如果辅助线程不调用SleepEx，则内核在完成一次I/O操作后，无法调用完成例程（因为完成例程的运行应该
	和当初激活WSARecv异步操作的代码在同一个线程之内）。完成例程内的实现代码比较简单，它取出接收到的数据，然后将数据原封不动的发送给客户端，最后重新
	激活另一个WSARecv异步操作。
	注意：在这里用到了“尾随数据”。我们在调用WSARecv的时候，参数lpOverlapped实际上指向一个比它大得多
	的结构PER_IO_OPERATION_DATA，这个结构除了WSAOVERLAPPED以外，还被我们附加了缓冲区的结构信息，
	另外还包括客户端套接字等重要的信息。这样，在完成例程中通过参数lpOverlapped拿到的不仅仅是
	WSAOVERLAPPED结构，还有后边尾随的包含客户端套接字和接收数据缓冲区等重要信息。这在我后面介绍
	完成端口的时候还会使用到。
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
