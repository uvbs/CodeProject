#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _LINUX64
#include <unistd.h>
#include <netinet/tcp.h>
#endif
#include "tcp_recv_service.h"

void TcpRecvService::pipe_readcb(evutil_socket_t readpipefd, short what, void* arg)
{
	try {
		if(what&EV_READ){
			TcpRecvService* pService = (TcpRecvService*)arg;

			char buf[1024] = {0};
			int buflen = 1024;
			int recvlen = 0;
            
#ifdef _LINUX64
            recvlen = read(readpipefd, buf, buflen);
#else
            if(!ReadFile((HANDLE)readpipefd,buf,buflen,(LPDWORD)&recvlen,NULL))
                return;
#endif

			int fdcount = recvlen/sizeof(pipe_msg);
			int reallen = fdcount*sizeof(pipe_msg);

			printf("TcpRecvService::pipe_readcb, readpipefd:%d, recvlen:%d.\n", readpipefd, recvlen);
	        
			struct event_base *base = pService->getBase();
			if(base == NULL){
				printf("TcpRecvService::pipe_readcb, service is not init, eventbase is NULL.\n");
				return;
			}

			for(int i = 0; i< fdcount; i++){
				pipe_msg connmsg;
				memcpy((void*)&connmsg, buf+i*sizeof(pipe_msg), sizeof(pipe_msg));
				int newfd = connmsg.fd;
				ServerClient* srvclient = (ServerClient*)connmsg.ptr;

				printf("TcpRecvService::pipe_readcb get a socketfd:%d from pipe.\n", newfd);

				if(srvclient){
					// Disable the Nagle (TCP No Delay) algorithm 
					//int flag = 1; 
					//int ret = setsockopt(newfd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
					//if (ret)
					//{
					//	printf("setsockopt error code:%d\n", ret);
					//}
					struct bufferevent *bev = bufferevent_socket_new(base, newfd, BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE);
					if (!bev) {
						printf("TcpRecvService::pipe_readcb::Error constructing bufferevent,fd:%d\n", newfd);
						event_base_loopbreak(base);
						return;
					}
					if (newfd<0)
					{
						return;
					}
					srvclient->setFD(newfd);
					srvclient->setState(ST_CONN);
					srvclient->setBev(bev);

					bufferevent_setcb(bev,
						Client::conn_readcb, Client::conn_writecb, ServerClient::conn_eventcb, (void*)srvclient);
					bufferevent_enable(bev, EV_READ);

					srvclient->setConnectTimeout();
					srvclient->setTcpRecvService(pService);
				}
			}
		}
	}catch(...) {}
}

void TcpRecvService::pipe_channel_readcb(evutil_socket_t readpipefd, short what, void* arg)
{
	try {
		if(what&EV_READ){
			TcpRecvService* pService = (TcpRecvService*)arg;

			char buf[1024] = {0};
			int buflen = 1024;
			int recvlen = 0;
            
#ifdef _LINUX64
            recvlen = read(readpipefd, buf, buflen);
#else
            if(!ReadFile((HANDLE)readpipefd,buf,buflen,(LPDWORD)&recvlen,NULL))
                return;
#endif

			int count = recvlen/sizeof(channel_pipe_msg);
			printf("TcpRecvService::pipe_channel_readcb, readpipefd:%d, recvlen:%d.\n", readpipefd, recvlen);

			struct event_base *base = pService->getBase();
			if(base == NULL){
				return;
			}

			for(int i = 0; i<count; i++)
			{
				channel_pipe_msg connmsg;
				memcpy((void*)&connmsg, buf+i*sizeof(channel_pipe_msg), sizeof(channel_pipe_msg));
				ServerClient* srvclient = (ServerClient*)connmsg.client;
				char* data = (char*)connmsg.userdata;
				int len = connmsg.len;
				if (srvclient)
				{
					srvclient->send_msg(data,len);
					delete data;
				}
			}
		}
	}catch(...) {}
}

int TcpRecvService::initService(int readpipefd, int writepipefd, int index)
{
    m_base = event_base_new();
    if(m_base == NULL){
        printf("TcpRecvService::initService fail, create eventbase fail.\n");
        return -1;
    }

	//与listen线程通信
    m_readpipefd = readpipefd;
    m_writepipefd = writepipefd;
    m_index = index;
    struct event *event_pipe = event_new(m_base, m_readpipefd, EV_READ|EV_PERSIST, TcpRecvService::pipe_readcb, this);
    if(event_pipe){
        event_add(event_pipe, NULL);
        printf("TcpRecvService::initService success, index:%d, readpipefd:%d.\n", m_index, m_readpipefd);
    }
    else{
        printf("TcpRecvService::initService fail, event_pipe is NULL.\n");
    }

	//与channel线程通信
	int pipe_fd[2];

#ifdef _LINUX64
	if(pipe(pipe_fd)<0){
		printf("TcpRecvService::initService fail, pipe create error.\n");
		return -1;
	}
#else
    //HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL; //使用系统默认的安全描述符
    sa.bInheritHandle = TRUE; //一定要为TRUE，不然句柄不能被继承。
    CreatePipe((PHANDLE)&pipe_fd[0],(PHANDLE)&pipe_fd[1],&sa,0); //创建pipe内核对象,设置好hReadPipe,hWritePipe.
#endif
	m_channelrdpipefd = pipe_fd[0];
	m_channelwrpipefd = pipe_fd[1];
	struct event *event_pipe2 = event_new(m_base, m_channelrdpipefd, EV_READ|EV_PERSIST, TcpRecvService::pipe_channel_readcb, this);
	if(event_pipe2){
		event_add(event_pipe2, NULL);
	}
	else{
		printf( "TcpRecvService::initService fail, event_pipe2 is NULL.\n");
	}
    /* 
    struct bufferevent *bev = bufferevent_socket_new(m_base, -1, BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE);    
    if (!bev) {
        fprintf(stderr, "Error constructing bufferevent!");
        event_base_loopbreak(m_base);
        return -1;
    }

    bufferevent_setfd(bev, m_inputpipefd);
    bufferevent_setcb(bev, TcpRecvService::conn_readcb, NULL, NULL, this);
    bufferevent_enable(bev, EV_READ);
    */
}
