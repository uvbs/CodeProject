#ifndef _listen_service_h_
#define _listen_service_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <vector>

#ifdef _LINUX64
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#elif _WIN32
#include <windows.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>

#include "tcp_recv_service.h"
#include "thread.h"

class listenthread : public Thread
{    
    virtual void thread(){return;};
};

class ListenService
{
public:
    ListenService()
    {
        m_base=NULL;
        m_nMaxTcpService = 0;
    };

public:
    int initService(int TcpServiceCount);

    void startService();
   
    void freeService();

    virtual void run()    
    {        
        m_thread.start(run_svr, this);    
    };

    static void* run_svr(void *args)    
    {        
        ListenService *t = (ListenService *) args;        
        t->svc();        
        return NULL;    
    };

    virtual void svc()    
    {
        startService();        
    };

    void setConnectTimeout(unsigned int msec)
    {
        m_connectTimeout.tv_sec = msec/1000;
        m_connectTimeout.tv_usec = msec%1000*1000;
    }
				   
   void setRecvTimeout(unsigned int msec)
   {
        m_recvTimeout.tv_sec = msec/1000;
        m_recvTimeout.tv_usec = msec%1000*1000;
   }
							 
   void setPacketTimeout(unsigned int msec)
   {
        m_packetTimeout.tv_sec = msec/1000;
        m_packetTimeout.tv_usec = msec%1000*1000;
   }
 
   void setWriteTimeout(unsigned int msec)
   {
        m_writeTimeout.tv_sec = msec/1000;
        m_writeTimeout.tv_usec = msec%1000*1000;
   }

   struct timeval* getConnectTimeout(){return &m_connectTimeout;};
   struct timeval* getRecvTimeout(){return &m_recvTimeout;};
   struct timeval* getPacketTimeout(){return &m_packetTimeout;};
   struct timeval* getWriteTimeout(){return &m_writeTimeout;};

public:
    struct event_base *getBase()
    {
        return m_base;
    };

public:
    TcpRecvService* getRandomService(unsigned int fd);
    TcpRecvService* getService(int nThreadIndex = -1);
private:
    listenthread        m_thread;
    struct event_base *m_base;
    std::vector<TcpRecvService*>    m_pRecvServices;
    int                m_nMaxTcpService;
    struct timeval     m_connectTimeout;
    struct timeval     m_recvTimeout;
    struct timeval     m_packetTimeout;
    struct timeval     m_writeTimeout;

};
#endif
