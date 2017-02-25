#ifndef __connector__h__
#define __connector__h__

#ifdef _LINUX64
#include <sys/time.h>
#endif

#include <time.h>

#include "listen_service.h"
#include "tcp_recv_service.h"
#include "client.h"
#include "handler.h"

class Connector : public Client
{
public:
    Connector();
    virtual ~Connector();
public:
    virtual int onConnect() = 0;    
    virtual int onConnectFail() = 0;
    virtual int onHeader(const char* buf, size_t buf_len);
    virtual int onData(char* buf, size_t buf_len)
    {
        return 0;
    };

public:    
    static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);
public:
    int registerToService(ListenService *pSerivce, int nThreadIndex = -1);  
    int connect(char* ip, unsigned short port);
public:
    struct event_base* getBase()
    {
        return m_base;
    };

    ListenService* getService()
    {
        return m_service;
    };

    void setHandler(BaseHandler* handler);
    BaseHandler* getHanler();

    unsigned short GetServerPort();
    const char* GetServerIP();
protected:
    int reConnect();
protected:
    //event_base,实际是service中的base
    struct event_base  *m_base; 
    ListenService         *m_service;
    struct sockaddr_in m_server_sin;
    BaseHandler        *m_handler;
};
#endif
