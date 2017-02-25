// eventOverlaph : 定义控制台应用程序的入口点。
// 
/* ! < 
	用事件通知方式实现的重叠I/O模型
	这个模型与上述其他模型不同的是它使用Winsock2提供的异步I/O函数WSARecv。
	在调用WSARecv时, 指定一个WSAOVERLAPPED结构，这个调用不是阻塞的，也就是说，它会立刻返回。
	一旦有数据到达的时候，被指定的WSAOVERLAPPED结构中的hEvent被Signaled。
	由于语句：g_CliEventArr[g_iTotalConn] = g_pPerIODataArr[g_iTotalConn]->overlap.hEvent；
	使得与该套接字相关联的WSAEVENT对象也被Signaled，所以WSAWaitForMultipleEvents的调用操作成功返回。
	我们现在应该做的就是用与调用WSARecv相同的WSAOVERLAPPED结构为参数调用WSAGetOverlappedResult，从而
	得到本次I/O传送的字节数等相关信息。在取得接收的数据后，把数据原封不动的发送到客户端，然后重新
	激活一个WSARecv异步操作。
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

