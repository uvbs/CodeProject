#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#ifdef _LINUX64
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "connector.h"
 
Connector::Connector()
{
    m_base = NULL;
    m_service = NULL;
    m_handler = NULL;
};

Connector::~Connector()
{
};

int Connector::registerToService(ListenService *lService, int nThreadIndex)
{
    TcpRecvService *rService = lService->getService(nThreadIndex);
    m_threadIndex = rService->getIndex();
    m_service = lService;

    struct event_base *base = rService->getBase();
    if(base == NULL){
        printf("Connector::registerToService fail, service is not init, base is NULL");
        return -1;
    }

    if(m_bev){
        bufferevent_free(m_bev);
    }
       
    m_bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE);
    if(m_bev == NULL){
        printf("Connector::registerToService fail, create bufferevent fail");
        return -1;
    }

    m_base = base;
    
    return 0;
}

void Connector::setHandler(BaseHandler* handler)
{
    m_handler = handler;
    m_parser = handler->getParser();
}

BaseHandler* Connector::getHanler()
{
    return m_handler;
}

unsigned short Connector::GetServerPort()
{
    return ntohs(m_server_sin.sin_port);
}

const char* Connector::GetServerIP()
{
    return inet_ntoa(m_server_sin.sin_addr);
}

int Connector:: connect(char* ip, unsigned short port)
{
    if(m_bev == NULL){
        printf("Connector:: connect error, bufferevent is NULL");
        return -1;
    }

    memset(&m_server_sin, 0, sizeof(m_server_sin));
    m_server_sin.sin_family = AF_INET;
    m_server_sin.sin_addr.s_addr = inet_addr(ip);
    m_server_sin.sin_port = htons(port);

    bufferevent_setcb(m_bev, NULL, NULL, Connector::conn_eventcb, (void*)this);
    bufferevent_socket_connect(m_bev, (struct sockaddr *)&m_server_sin, sizeof(m_server_sin));

    return 0;
}

int Connector::reConnect()
{
    if(m_bev){
        bufferevent_free(m_bev);
    }

    m_bev = bufferevent_socket_new(m_base, -1, BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE);
    if(m_bev == NULL){
        printf("Connector::registerToService fail, create bufferevent fail");
        return -1;
    }

    bufferevent_setcb(m_bev, NULL, NULL, Connector::conn_eventcb, (void*)this);
    bufferevent_socket_connect(m_bev, (struct sockaddr *)&m_server_sin, sizeof(m_server_sin));
    return 0;
}

int Connector::onHeader(const char* buf, size_t buf_len)
{
    size_t headerlen;
    size_t bodylen;
    size_t packetlen;

    int ret = m_parser->parseHeader(buf, buf_len, headerlen, bodylen, packetlen);
    if(ret == 0){
        m_headerlen = headerlen;
        m_bodylen = bodylen;
        m_packetlen = packetlen;
        return 0;
    }
    else if(ret == 1){
        //??：1???：o?：：?
        return 1;
    }
    else{
         return 2;
    }
}

void Connector::conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
    if (events & BEV_EVENT_CONNECTED) {
        Connector* conn = (Connector*)user_data;
	ListenService* pService = conn->getService();

	conn->setConnectTimeoutValue(pService->getConnectTimeout());
	conn->setRecvTimeoutValue(pService->getRecvTimeout());
	conn->setPacketTimeoutValue(pService->getPacketTimeout());
	conn->setWriteTimeoutValue(pService->getWriteTimeout());
        
	conn->setFD(bufferevent_getfd(bev));
        bufferevent_setcb(bev, Client::conn_readcb, Client::conn_writecb, Connector::conn_eventcb, user_data);
        bufferevent_enable(bev, EV_READ|EV_WRITE);
        conn->setState(ST_CONN);
        conn->onConnect();
	conn->setConnectTimeout();
    }
    else if (events & BEV_EVENT_EOF) {
        Connector* conn = (Connector*)user_data;
        conn->setState(ST_CLOSE);
        bufferevent_setcb(bev,NULL, NULL, NULL, NULL);
        conn->onClose(1);
    }
    else if (events & BEV_EVENT_ERROR) {
        int errcode = EVUTIL_SOCKET_ERROR();
        //printf("Connector::conn_eventcb errinfo:%d/%s\r\n", errcode, evutil_socket_error_to_string(errcode));
        Connector* conn = (Connector*)user_data;
        if(conn->getState() == ST_INIT){
            conn->onConnectFail();
        }
        else{//ST_CONN
            conn->setState(ST_CLOSE);
            bufferevent_setcb(bev,NULL, NULL, NULL, NULL);
            conn->onClose(2);
        }
    }
    else if(events & BEV_EVENT_TIMEOUT){
        Connector* conn = (Connector*)user_data;
        int ret = 0;
        if(events & BEV_EVENT_READING){
            ret = conn->onTimeout();
        }
        else{
            ret = conn->onSendTimeout();
        }
        if(ret == 0){
            conn->setState(ST_CLOSE);
        }  
        bufferevent_enable(bev, EV_READ|EV_WRITE);
    }
    //bufferevent_free(bev);
}

