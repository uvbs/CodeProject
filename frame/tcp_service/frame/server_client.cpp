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
#include "server_client.h"
#include "handler.h"
#include "listener.h"

ServerClient::ServerClient()
{
    m_listener = NULL;
    m_handler = NULL;
};

ServerClient::~ServerClient()
{
};

void ServerClient::conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
    if (events & BEV_EVENT_EOF) {
        //连接正常关系
        ServerClient* client = (ServerClient*)user_data;
        client->setState(ST_CLOSE);
        bufferevent_setcb(bev,NULL, NULL, NULL, NULL);
        client->onClose(CLIENT_FD_CLOSE);
        //delete client;
    }
    else if (events & BEV_EVENT_ERROR) {
        //连接出错
        ServerClient* client = (ServerClient*)user_data;
        client->setState(ST_CLOSE);
        bufferevent_setcb(bev,NULL, NULL, NULL, NULL);
        client->onClose(CLIENT_FD_ERROR);
        //delete client;
    }
    else if(events & BEV_EVENT_TIMEOUT){
        //连接超时
        ServerClient* client = (ServerClient*)user_data;
        int ret = 0;
        if(events & BEV_EVENT_READING){
            ret = client->onTimeout();
        }
        else{
            ret = client->onSendTimeout();
        } 
        if(ret == 0){
            client->setState(ST_CLOSE);
        } 
        bufferevent_enable(bev, EV_READ|EV_WRITE);
        //delete client;
    }
    //bufferevent_free(bev);
}

void ServerClient::setClientAddr(char* ip, unsigned short port)
{
    memset(&m_client_sin, 0, sizeof(m_client_sin));
    m_client_sin.sin_family = AF_INET;
    m_client_sin.sin_addr.s_addr = inet_addr(ip);
    m_client_sin.sin_port = htons(port);
}

unsigned short ServerClient::GetPort()
{	
    return ntohs(m_client_sin.sin_port);
}

const char* ServerClient::GetIP()
{	
    return inet_ntoa(m_client_sin.sin_addr);
}

BaseHandler* ServerClient::getHandler()
{
    return m_handler;
}

void ServerClient::setHandler(BaseHandler* handler)
{
    m_handler = handler;
}

ListenService* ServerClient::getListenService()
{
    return m_listener->getService();
}

int ServerClient::onHeader(const char* buf, size_t buf_len)
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
        //°ü?′ê?è?
        return 1;
    }
    else{
         return 2;
    }
}


