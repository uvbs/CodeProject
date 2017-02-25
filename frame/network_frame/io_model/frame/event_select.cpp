// WSAEventSelect.cpp : 定义控制台应用程序的入口点。
//
#include "event_select.h"

#if defined (_WIN32)

namespace event_select
{

int g_iTotalConn = 0;
SOCKET g_CliSocketArr[MAXIMUM_WAIT_OBJECTS];
WSAEVENT g_CliEventArr[MAXIMUM_WAIT_OBJECTS];

void* WorkerThread(void* lpParam)
{
	int ret, index;
	WSANETWORKEVENTS NetworkEvents;
	char szMessage[MSGSIZE];

	while (true)
	{
		ret = WSAWaitForMultipleEvents(g_iTotalConn, g_CliEventArr, false, 1000, false);
		if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
		{
			continue;
		}

		index = ret - WSA_WAIT_EVENT_0;
		WSAEnumNetworkEvents(g_CliSocketArr[index], g_CliEventArr[index], &NetworkEvents);

		if (NetworkEvents.lNetworkEvents & FD_READ)
		{
			// Receive message from client
			ret = recv(g_CliSocketArr[index], szMessage, MSGSIZE, 0);
			if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
			{
				Cleanup(index);
			}
			else
			{
				szMessage[ret] = '\0';
				send(g_CliSocketArr[index], szMessage, ::strlen(szMessage), 0);
			}
		}

		if (NetworkEvents.lNetworkEvents & FD_CLOSE)
		{
			Cleanup(index);
			printf("Client socket %d closed.\n", g_CliSocketArr[index]);
		}
	}
	return 0;
}

void Cleanup(int index)
{
	closesocket(g_CliSocketArr[index]);
	WSACloseEvent(g_CliEventArr[index]);

	if (index < g_iTotalConn - 1)
	{
		g_CliSocketArr[index] = g_CliSocketArr[g_iTotalConn - 1];
		g_CliEventArr[index] = g_CliEventArr[g_iTotalConn - 1];
	}

	g_iTotalConn--;
}

int startSrv()
{
    WSADATA  wsaData;
    SOCKET sListen, sClient;
    SOCKADDR_IN local, client;
    int iaddrSize = sizeof(SOCKADDR_IN);

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
        sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
        printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        // Associate socket with network event
        g_CliSocketArr[g_iTotalConn] = sClient;
        g_CliEventArr[g_iTotalConn] = WSACreateEvent();
        WSAEventSelect(g_CliSocketArr[g_iTotalConn], g_CliEventArr[g_iTotalConn], 
            FD_READ | FD_CLOSE);
        g_iTotalConn++;
    }

    return 0;
}

} //namespace EventSelect

#endif //(_WIN32)
