// completionOverlap.cpp : 定义控制台应用程序的入口点。
//

#include "completion_overlap.h"

#ifdef _WIN32

namespace completion_overlap
{

SOCKET g_sNewClientConnection;
BOOL   g_bNewConnectionArrived = false;

void* WorkerThread(void* lpParam)
{
    LPPER_IO_OPERATION_DATA lpPerIOData = NULL;

    while (true)
    {
        if (g_bNewConnectionArrived)
        {
            // Launch an asynchronous operation for new arrived connection
            lpPerIOData = (LPPER_IO_OPERATION_DATA)HeapAlloc(
                GetProcessHeap(),
                HEAP_ZERO_MEMORY,
                sizeof(PER_IO_OPERATION_DATA));
            lpPerIOData->Buffer.len = MSGSIZE;
            lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
            lpPerIOData->sClient = g_sNewClientConnection;

            WSARecv(lpPerIOData->sClient,
                &lpPerIOData->Buffer,
                1,
                &lpPerIOData->NumberOfBytesRecvd,
                &lpPerIOData->Flags,
                &lpPerIOData->overlap,
                CompletionROUTINE); 

            g_bNewConnectionArrived = false;
        }

        SleepEx(1000, true);
    }
    return 0;
}

void CALLBACK CompletionROUTINE(DWORD dwError,
                                DWORD cbTransferred,
                                LPWSAOVERLAPPED lpOverlapped,
                                DWORD dwFlags)
{
    LPPER_IO_OPERATION_DATA lpPerIOData = (LPPER_IO_OPERATION_DATA)lpOverlapped;

    if (dwError != 0 || cbTransferred == 0)
    {
        // Connection was closed by client
        closesocket(lpPerIOData->sClient);
        HeapFree(GetProcessHeap(), 0, lpPerIOData);
    }
    else
    {
        lpPerIOData->szMessage[cbTransferred] = '\0';
        send(lpPerIOData->sClient, lpPerIOData->szMessage, cbTransferred, 0);

        // Launch another asynchronous operation
        memset(&lpPerIOData->overlap, 0, sizeof(WSAOVERLAPPED));
        lpPerIOData->Buffer.len = MSGSIZE;
        lpPerIOData->Buffer.buf = lpPerIOData->szMessage;    

        WSARecv(lpPerIOData->sClient,
            &lpPerIOData->Buffer,
            1,
            &lpPerIOData->NumberOfBytesRecvd,
            &lpPerIOData->Flags,
            &lpPerIOData->overlap,
            CompletionROUTINE);
    }
}

int startSrv()
{
	WSADATA     wsaData;
	SOCKET      sListen;
	SOCKADDR_IN local, client;
	//DWORD       dwThreadId;
	int         iaddrSize = sizeof(SOCKADDR_IN);

	// Initialize Windows Socket library
	WSAStartup(0x0202, &wsaData);

	// Create listening socket
	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Bind
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));

	// Listen
	listen(sListen, 3);

	// Create worker thread
    Thread t;
    t.start(WorkerThread,NULL);

	while (true)
	{
		// Accept a connection
		g_sNewClientConnection = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
		g_bNewConnectionArrived = true;
		printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	}
}

} //namespace completion_overlap

#endif //_WIN32
