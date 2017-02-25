#ifndef __server_client__h__
#define __server_client__h__

#include <time.h>
#include <sys/types.h>

#ifdef _LINUX64
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "client.h"
#include "handler.h"

//class BaseHandler;
class SrvListener;
class ListenService;
class TcpRecvService;

class ServerClient:public Client
{
public:
    ServerClient();
    virtual ~ServerClient();
public:    
    static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);
public:
    virtual int onHeader(const char* buf, size_t buf_len);
    virtual int onData(const char* buf, size_t buf_len)
    {
        return 0;
    };
    

public:    
    void setListener(SrvListener *listener)
    {
        m_listener = listener;
    };

    BaseHandler* getHandler();
    void setHandler(BaseHandler* handler);

    ListenService* getListenService();
	TcpRecvService* getTcpRecvService() {return m_tcpService;}
	void setTcpRecvService(TcpRecvService* p) {m_tcpService=p;}

    void setClientAddr(char* ip, unsigned short port);
    unsigned short GetPort();	
    const char* GetIP();
protected:
    //listener
    SrvListener        *m_listener;
    BaseHandler       *m_handler; 
	TcpRecvService* m_tcpService;
    struct sockaddr_in m_client_sin;
};

#endif
