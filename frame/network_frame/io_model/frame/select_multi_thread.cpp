// SelectMultiThread.cpp : �������̨Ӧ�ó������ڵ㡣
//
// ѡ��ģ�� +  ���߳�

#include "select_multi_thread.h"

#if defined (_WIN32)

namespace select_multi_thread_server
{

// ʵ����������
int g_iTotalConn = 0;
// Socket����
SOCKET g_CliSocketArr[MAX_THREADS_NUM*MAX_CLIENTS_NUM_PER_THREAD];

void* WorkerThread(void* lpParam)
{
	int i;
	fd_set fdread; // ����
	int ret;
	struct timeval tv = {1, 0}; //Maximum time for select to wait
	char szMessage[MSGSIZE];
	char szSendMessage[MSGSIZE] = {0};

	//�ù����̴߳���Ŀͻ�����ʼ���
	int start = ((int)lpParam)*MAX_CLIENTS_NUM_PER_THREAD;
	
	while (true)
	{
		//����ù����̴߳���Ŀͻ�����ֹ���
		//����g_iTotalConn��仯�����Է���ѭ������
		int end = (((int)lpParam)+1)*MAX_CLIENTS_NUM_PER_THREAD < g_iTotalConn 
			? (((int)lpParam)+1)*MAX_CLIENTS_NUM_PER_THREAD : g_iTotalConn;

		// ��ʼ������
		FD_ZERO(&fdread);

		// ����ǰ�Լ�����Ŀͻ��˵��׽��ּ��뵽����fdread��
		for (i = start; i < end; i++)
		{
			FD_SET(g_CliSocketArr[i], &fdread);
		}

		// We only care read event
		// The select function returns the total number of socket handles that 
		// are ready and contained in the fd_set structures, zero if the time limit expired, 
		// or SOCKET_ERROR if an error occurred. 
		ret = select(0, &fdread, NULL, NULL, &tv);

		if (ret == 0 || ret == SOCKET_ERROR)
		{
			// Time expired or an error occurred
			continue;
		}

		for (i = start; i < end; i++)
		{
			if (FD_ISSET(g_CliSocketArr[i], &fdread))
			{
				// A read event happened on g_CliSocketArr[i]
				SOCKET sockConn = g_CliSocketArr[i];
				ret = recv(sockConn, szMessage, MSGSIZE, 0);
				if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
				{
					// Client socket closed
					printf("Client socket %d closed.\n", sockConn);
					closesocket(sockConn);
					// g_CliSocketArr[i--] = g_CliSocketArr[--g_iTotalConn];
					--g_iTotalConn;
					for (int j=i; j<g_iTotalConn; j++)
					{
						g_CliSocketArr[j] = g_CliSocketArr[j+1];
					}
				}
				else
				{
					// We received a message from client
					szMessage[ret] = '\0';
					memset(szSendMessage, 0, MSGSIZE);
					sprintf(szSendMessage, "From thread: %d, %s", (int)lpParam, szMessage);
					send(sockConn, szSendMessage, ::strlen(szSendMessage), 0);
				}
			}
		}
	}

	return 0;
}

int startSrv()
{
    WSADATA wsaData;
    SOCKET sListen, sClient;
    SOCKADDR_IN local, client;
    int iaddrSize = sizeof(SOCKADDR_IN);
    unsigned dwThreadId;
    // �߳�����
    HANDLE threadArray[MAX_THREADS_NUM];

    // Initialize Windows socket library
    WSAStartup(0x0202, &wsaData);

    // Create listening socket
    sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Bind
    local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));

    // Listen
    listen(sListen, MAX_THREADS_NUM*MAX_CLIENTS_NUM_PER_THREAD);

    // Create worker thread
    Thread t[MAX_THREADS_NUM];
    for (int i=0; i<MAX_THREADS_NUM; i++)
    {
        //threadArray[i] = (HANDLE) _beginthreadex(NULL, 0, WorkerThread, (VOID*)i, 
        //    CREATE_SUSPENDED, &dwThreadId);
        /*threadArray[i] = */t[i].start(WorkerThread,(void*)i, false);
    }

    while (true)
    {
        // Accept a connection
        sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
        printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        // Add socket to g_CliSocketArr
        if (g_iTotalConn >= MAX_THREADS_NUM*MAX_CLIENTS_NUM_PER_THREAD)
            continue;
        //ResumeThread(threadArray[g_iTotalConn/MAX_CLIENTS_NUM_PER_THREAD]);
        t[g_iTotalConn/MAX_CLIENTS_NUM_PER_THREAD].resume();
        g_CliSocketArr[g_iTotalConn++] = sClient;
    }

    return 0;
}

} //namespace MultiThreadSelectServer

#endif //(_WIN32)
