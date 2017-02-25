#ifndef _listener2_h_
#define _listener2_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#ifdef _LINUX64
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "handler.h"
#include "server_client.h"
#include "listen_service.h"

/*
      监听级基类，负责开始在端口上监听，接收新连接
      通过调用继承类的makeClient函数创建客户端连接类
*/
class SrvListener
{
public:
    SrvListener()
    {
        m_port = 0;
        m_socketlisten = 0;
        m_listener = NULL;
        m_base = NULL;
        m_pService = NULL;
        m_handler = NULL;

    };

public:
    void setPort(unsigned short port)
    {
        m_port = port;
    };
    
    unsigned short getPort()
    {
        return m_port;
    };

    struct timeval* getConnectTimeout(){return m_pService->getConnectTimeout();};
    struct timeval* getRecvTimeout(){return m_pService->getRecvTimeout();};
    struct timeval* getPacketTimeout(){return m_pService->getPacketTimeout();};
    struct timeval* getWriteTimeout(){return m_pService->getWriteTimeout();};

    void setBase(struct event_base *base)
    {
        m_base = base;
    }

    struct event_base *getBase()
    {
        return m_base;
    }

    void setService(ListenService *pService)
    {
        m_pService = pService;
    }

    ListenService* getService()
    {
        return m_pService;
    };

    struct sockaddr_in* getAddr()
    {
        return &m_addresslisten;
    }
    
    BaseHandler* getHandler()
    {
        return m_handler;
    };

    void setHandler(BaseHandler* handler)
    {
        m_handler = handler;
    };

public:
    //初始化listener，创建socket
    int initListener();
    int initListener(BaseHandler* handler);

    //注册到service，开始监听
    int registerToService(ListenService *pSerivce, int backlog);

    //监听回调函数
    static void listener_cb(struct evconnlistener *evlistener, evutil_socket_t fd,struct sockaddr *sa, int socklen, void *user_data);
    void freeListener();

private:
    //handler
    BaseHandler*    m_handler;

    //端口
    unsigned short m_port;

    //监听socket
    int    m_socketlisten;

    //监听地址
    struct sockaddr_in m_addresslisten;

    //evconnlistener
    struct evconnlistener *m_listener;

    //event_base,为service中的event_base
    struct event_base *m_base;

    //service
    ListenService     *m_pService;
};

#endif
