#ifndef _my_handler_h_
#define _my_handler_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _LINUX64
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#endif

#include <time.h>
#include <map>
#include "my_server_client.h"
#include "my_connector.h"
#include "handler.h"
#include "http_request.h"

typedef std::map<std::string, MyConnector*> CHANNELID2CONNECTORMAP;

class MyHandler:public BaseHandler
{
public:
    MyHandler(unsigned short port):BaseHandler(port)
    {}

    virtual ServerClient* onAccept(struct sockaddr_in *sin, int socklen)
    {
        ServerClient* client = new MyServerClient;
        addClientCount();
        printf("MyHandler::onAccept handler->ClientCount:%d\r\n", getClientCount());
        return client;
    };

    virtual int handleData(ServerClient* client, char* buf, size_t buf_len);
    virtual int handleData(Connector* client, char* buf, size_t buf_len);
    int handleReadTimeout(Connector* client);

public:
    int packSuccessRes(char *&retBuf, int& retBufLen, const char* body, int bodylen);

    int handleResp(Connector* client, httpRequest* httpReq);
    int pushData(Connector* client, httpRequest* httpReq);
    int regUser(ServerClient* client, httpRequest* httpReq);

public:
    int addConnector(std::string channelid, MyConnector* conn)
    {
        //m_cid2connLock.Lock();
        m_cid2connMap.insert(CHANNELID2CONNECTORMAP::value_type(channelid,conn));
        //m_cid2connLock.Unlock();
    };

    MyConnector* getConnector(std::string channelid)
    {
        //m_cid2connLock.Lock();
        CHANNELID2CONNECTORMAP::iterator itco=m_cid2connMap.find(channelid);
        //m_cid2connLock.Unlock();
        if(itco == m_cid2connMap.end()){
            return NULL;
        }
        else{
            MyConnector *conn = itco->second;
            return conn;
        }
    };

    int delConnector(std::string channelid)
    {
        //m_cid2connLock.Lock();
        CHANNELID2CONNECTORMAP::iterator itco=m_cid2connMap.find(channelid);
        if(itco == m_cid2connMap.end()){
            //m_cid2connLock.Unlock();
            return -1;
        }
        else{
	    MyConnector *conn = itco->second;	
            m_cid2connMap.erase(itco);
	    conn->setInvalid();
	    conn->setClose();
	    conn->close();
            //m_cid2connLock.Unlock();
            return 0;
        }
    };

private:
    CHANNELID2CONNECTORMAP m_cid2connMap;
};

#endif
