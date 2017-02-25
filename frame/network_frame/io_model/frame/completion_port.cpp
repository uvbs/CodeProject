// CompletionPort.cpp : 定义控制台应用程序的入口点。
//
/*!<
	在你的应用程序需要同时管理数百乃至上千个套接字的时候，
    而且希望随着系统内安装的CPU数量的增多，
    应用程序的性能也可以线性提升，才应考虑采用“完成端口”模型。
	
*/

#include "completion_port.h"

#ifdef _WIN32

namespace completion_port
{

    void* WorkerThread(void* CompletionPortID)
    {
        HANDLE                  completionPortHandle=(HANDLE)CompletionPortID;
        DWORD                   dwBytesTransferred;
        SOCKET                  sClient;
        LPPER_IO_OPERATION_DATA lpPerIOData = NULL;

        while (true)
        {
            // 取得本次I/O的相关信息（例如套接字句柄、传送的字节数、单I/O数据结构的地址等等）
            GetQueuedCompletionStatus(
                completionPortHandle,
                &dwBytesTransferred,
                (PULONG_PTR)&sClient,
                (LPOVERLAPPED *)&lpPerIOData,
                INFINITE);
            if (dwBytesTransferred == 0xFFFFFFFF)
            {
                return 0;
            }

            if (lpPerIOData->OperationType == RECV_POSTED)
            {
                if (dwBytesTransferred == 0)
                {
                    // Connection was closed by client
                    closesocket(sClient);
                    HeapFree(GetProcessHeap(), 0, lpPerIOData);        
                }
                else
                {
                    lpPerIOData->szMessage[dwBytesTransferred] = '\0';
                    send(sClient, lpPerIOData->szMessage, dwBytesTransferred, 0);

                    // Launch another asynchronous operation for sClient
                    memset(lpPerIOData, 0, sizeof(PER_IO_OPERATION_DATA));
                    lpPerIOData->Buffer.len = MSGSIZE;
                    lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
                    lpPerIOData->OperationType = RECV_POSTED;
                    WSARecv(sClient,
                        &lpPerIOData->Buffer,
                        1,
                        &lpPerIOData->NumberOfBytesRecvd,
                        &lpPerIOData->Flags,
                        &lpPerIOData->overlap,
                        NULL);
                }
            }
        }
        return 0;
    }


    int startSrv()
    {
	    WSADATA                 wsaData;
	    SOCKET                  sListen, sClient;
	    SOCKADDR_IN             local, client;
	    //DWORD                   i, dwThreadId;
	    int                     iaddrSize = sizeof(SOCKADDR_IN);
	    HANDLE                  completionPortHandle = INVALID_HANDLE_VALUE;
	    SYSTEM_INFO             systeminfo;
	    LPPER_IO_OPERATION_DATA lpPerIOData = NULL;

	    // Initialize Windows Socket library
	    WSAStartup(0x0202, &wsaData);

	    // Create completion port
	    completionPortHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	    // Create worker thread
	    GetSystemInfo(&systeminfo);
        Thread *t = new Thread[systeminfo.dwNumberOfProcessors];
	    for (int i = 0; i < systeminfo.dwNumberOfProcessors; i++)
	    {
		    //CreateThread(NULL, 0, WorkerThread, completionPortHandle, 0, &dwThreadId);
            t[i].start(WorkerThread, completionPortHandle);
	    }

	    // Create listening socket
	    sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	    // Bind
	    local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	    local.sin_family = AF_INET;
	    local.sin_port = htons(PORT);
	    bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));

	    // Listen
	    listen(sListen, 3);

	    while (true)
	    {
		    // Accept a connection
		    sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
		    printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

		    // Associate the newly arrived client socket with completion port
		    CreateIoCompletionPort((HANDLE)sClient, completionPortHandle, (DWORD)sClient, 0);

		    // Launch an asynchronous operation for new arrived connection
		    lpPerIOData = (LPPER_IO_OPERATION_DATA)HeapAlloc(
			    GetProcessHeap(),
			    HEAP_ZERO_MEMORY,
			    sizeof(PER_IO_OPERATION_DATA));
		    lpPerIOData->Buffer.len = MSGSIZE;
		    lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
		    lpPerIOData->OperationType = RECV_POSTED;
		    WSARecv(sClient,
			    &lpPerIOData->Buffer,
			    1,
			    &lpPerIOData->NumberOfBytesRecvd,
			    &lpPerIOData->Flags,
			    &lpPerIOData->overlap,
			    NULL);
	    }

	    PostQueuedCompletionStatus(completionPortHandle, 0xFFFFFFFF, 0, NULL);
	    CloseHandle(completionPortHandle);
	    closesocket(sListen);
	    WSACleanup();
	    return 0;
    }



} //namespace completion_port

#endif //_WIN32
