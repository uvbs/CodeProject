#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include "my_handler.h"
#include "my_packet_parser.h"
#include "listen_service.h"
#include "listener.h"

bool g_Quit = false;

void processSignal(int sig)
{
    g_Quit = true;
    printf("recv quit msg.\n");
    //exit(0);
}

int main(int argc,char **argv)
{
    //client request like this:
    //wget "http://10.48.57.62:8888/register?name=mr.chen&sex=male" -O DB.rs
    printf("start\r\n");

    signal(SIGINT, processSignal);
    signal(SIGTERM, processSignal);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
	
    int port = 8888;
    int threadnum = 5;
   
    ListenService myService;
    myService.initService(threadnum);
    myService.setConnectTimeout(5000);
    myService.setRecvTimeout(3600000);
    myService.setPacketTimeout(1000);
    myService.setWriteTimeout(3000);

    MyPacketParser* parser = new MyPacketParser();
    MyHandler* handler = new MyHandler(port);
    handler->setParser(parser);
    
    SrvListener listener;
    listener.initListener(handler);
    listener.registerToService(&myService, 2048);
    //myService.run();
    myService.startService();
    myService.freeService();
    listener.freeListener();

    return 0;
}

