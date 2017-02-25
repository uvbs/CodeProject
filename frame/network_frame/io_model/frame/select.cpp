// selectsrv.cpp : 定义控制台应用程序的入口点。
//
// 选择模型
#include "select.h"

#if defined (_WIN32)

namespace select_server
{

int g_iTotalConn=0;
SOCKET g_CliSocketArr[FD_SETSIZE];

int ConditionFunc(LPWSABUF lpCallerId,LPWSABUF lpCallerData,LPQOS lpSQOS,LPQOS lpGQOS,
                         LPWSABUF lpCalleeId,LPWSABUF lpCalleeData,GROUP FAR * g,DWORD dwCallbackData)
{
    if (g_iTotalConn < 3)
	    return CF_ACCEPT;
    else
	    return CF_REJECT;
}

void* WorkerThread(void* lpParam)
{
	int i;
	fd_set fdread; // 读集
	int ret;
	struct timeval tv = {1, 0}; //Maximum time for select to wait
	char szMessage[MSGSIZE];

	while (true)
	{
		// 初始化读集
		FD_ZERO(&fdread);
		// 将当前所有的客户端套接字加入到读集fdread中
		for (i = 0; i < g_iTotalConn; i++)
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

		for (i = 0; i < g_iTotalConn; i++)
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
					send(sockConn, szMessage, ::strlen(szMessage), 0);
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
    listen(sListen, 3);

    // Create worker thread
    Thread t;
    t.start(WorkerThread,NULL);

    while (true)
    {
        // Accept a connection
        sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
        //sClient = WSAAccept(sListen, (struct sockaddr *)&client, &iaddrSize, ConditionFunc, NULL);
        printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        // Add socket to g_CliSocketArr
        g_CliSocketArr[g_iTotalConn++] = sClient;
    }

    return 0;
}

} //namespace SelectServer

#endif //(_WIN32)
