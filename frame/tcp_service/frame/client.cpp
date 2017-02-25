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
#include "client.h"

Client::Client()
{
    m_fd = 0;
    m_bev = NULL;
    m_headerlen = 0;
    m_bodylen = 0;
    m_packetlen = 0;
    m_parser=NULL;
    m_buf=NULL;
    m_buflen = 0;
    m_bev = NULL;
    m_state = ST_INIT;
    m_connectTimeout.tv_sec = 0;
    m_connectTimeout.tv_usec = 0;
    m_recvTimeout.tv_sec = 0;
    m_recvTimeout.tv_usec = 0;
    m_packetTimeout.tv_sec = 0;
    m_packetTimeout.tv_usec = 0;
    m_threadIndex = -1;
    m_bCloseAfterSend = false;

};

Client::~Client()
{
    if(m_buflen > 0 && m_buf != NULL){
        free(m_buf);
        m_buflen = 0;
    }

    if(m_bev){
        bufferevent_free(m_bev);
        m_bev=NULL;
    }
};

char* Client::getBuf(size_t buflen)
{
    if(buflen <= 0){
        return NULL;
    }

    if(buflen <= m_buflen){
        return m_buf;
    }

    if(m_buf == NULL){
        m_buf = (char*)malloc(buflen);
        if(m_buf){
            m_buflen = buflen;
            return m_buf;
        }
        else{
            m_buflen = 0;
            return NULL;
        }
    }
    else {
        m_buf = (char*)realloc(m_buf, buflen);
        if(m_buf){
            m_buflen = buflen;
            return m_buf;
        }
        else{
            m_buflen = 0;
            return NULL;
        }
    } 
}

void Client::close()
{
    printf("Client::close fd:%d", m_fd);
    struct evbuffer *output = bufferevent_get_output(m_bev);
    size_t len = evbuffer_get_length(output);
    if(len == 0){
        setState(ST_CLOSE);
        //bufferevent_disable(m_bev, EV_READ|EV_WRITE);
        shutdown(m_fd, 0);
    }
    else{
    printf( "Client::close fd:%d, after sended", m_fd);  
	setState(ST_CLOSE);
        m_bCloseAfterSend = true;
    }
}

void Client::conn_readcb(struct bufferevent *bev, void *ctx)
{
    Client* client  = (Client *)ctx;
    struct evbuffer *evbsrc;
    size_t evblen;
    evbsrc = bufferevent_get_input(bev);
    evblen = evbuffer_get_length(evbsrc);

    if(client->getParser() == NULL){
        char* buf = client->getBuf(evblen);
        if(buf){
            evbuffer_copyout(evbsrc, buf, evblen);
            evbuffer_drain(evbsrc, evblen);
            client->onData(buf, evblen);
        }
    }
    else{
        if(client->getState() == ST_CONN || client->getState() == ST_BODY){
            //刚接收连接或者收到一个完整包，准备接收包头
            char* buf = client->getBuf(evblen);
            if(buf){
                evbuffer_copyout(evbsrc, buf, evblen);
                int ret = client->onHeader(buf, evblen);

                if(ret == 1){
                    //包头还不完整
                    client->setPacketTimeout();
                    return ;
                }
                else if(ret == 0){
                    //已接收到完整包头，接收body
                    size_t headerlen = client->getHeaderLen();
                    
                    client->setState(ST_HEADER);
                    client->setPacketTimeout();
                    if(evblen >= headerlen){
                        //bufferevent中数据长度大于包头长度，继续
                        conn_readcb(bev, ctx);
                    }
                    else{
                        return;
                    }
                }
                else if(ret == 2){
                    //协议错误,关闭连接
                    printf("Client::protocol error close,fd:%d", client->getFD());
		            //client->setState(ST_CLOSE);
                    client->close();
                    return;
                }
            }
            else{
                printf("Client::conn_readcb warning: malloc mem fail, fd:%d, len:%d", client->getFD(), evblen);
                return;
            }
        }
        else if(client->getState() == ST_HEADER){
            //已接收完整包头，接收body
            size_t packetlen = client->getPacketLen();
            if(evblen < packetlen)
            {
                //body 上不完整
                client->setPacketTimeout();
                return ;
            }
            else if(evblen >= packetlen){
                //已接收到完整body
                client->setState(ST_BODY);
                client->setRecvTimeout();
                char* buf = client->getBuf(packetlen);
                if(buf){
                    evbuffer_copyout(evbsrc, buf, packetlen);
                    evbuffer_drain(evbsrc, packetlen);
                    client->onPacket((char*)buf, packetlen);
                    if(evblen == packetlen){
                        return;
                    }
                    else{
                        //bufferevent中还有剩余数据，继续
                        conn_readcb(bev, ctx);
                    }
                }
                else{
                    printf("Client::conn_readcb warning: malloc mem fail, fd:%d, len:%d", client->getFD(), evblen);
                    return;
                }
            }
        }
    }
}

void Client::conn_writecb(struct bufferevent *bev, void *user_data)
{
    Client* client  = (Client *)user_data;
    struct evbuffer *output = bufferevent_get_output(bev);
    size_t len = evbuffer_get_length(output);
    bool bCloseAfterSend = client->getCLoseAfterSend();
    if(bCloseAfterSend == true && len == 0){
	    printf("Client::conn_writecb: close fd:%d when write",client->getFD());
        client->close();
    }

    /*
    if (len == 0) {
        printf("Client::conn_writecb send data finished, len:%lu\n", len);
    }
    else{
        printf("Client::conn_writecb send data no finished, len:%lu\n", len);
    }*/
}

void Client::send_msg(char* msg, size_t msg_len)
{
    if(getState() == ST_CLOSE || getState() == ST_INIT){
        return;
    }

	if (getState() != ST_CONN && 
		getState() != ST_HEADER &&
		getState() != ST_BODY)
	{
		return;
	}

    if(m_bev && msg && msg_len > 0){ 
        bufferevent_write(m_bev, msg, msg_len);
		bufferevent_flush(m_bev,EV_WRITE,BEV_FLUSH);
    }
}

int Client::getOutputBufferLength()
{
	struct evbuffer *output = bufferevent_get_output(m_bev);
	size_t len = evbuffer_get_length(output);
	return len;
}

int Client::getInputBufferLength()
{
	struct evbuffer *input = bufferevent_get_input(m_bev);
	size_t len = evbuffer_get_length(input);
	return len;
}

void Client::setParser(PacketParser* parser)
{
    m_parser = parser;
}

PacketParser* Client::getParser()
{
    return m_parser;
}

void Client::setConnectTimeout()
{
    bufferevent_set_timeouts(m_bev, NULL, NULL);
    bufferevent_set_timeouts(m_bev, &m_connectTimeout, &m_writeTimeout);
    m_tostate = TO_CONN;
};

void Client::setRecvTimeout()
{
    if(m_tostate != TO_RECV){
        m_tostate = TO_RECV;
        bufferevent_set_timeouts(m_bev, NULL, NULL);
        bufferevent_set_timeouts(m_bev, &m_recvTimeout, &m_writeTimeout);
    }
}

void Client::setPacketTimeout()
{
    if(m_tostate != TO_PACKET){
        m_tostate = TO_PACKET;
        bufferevent_set_timeouts(m_bev, NULL, NULL);
        bufferevent_set_timeouts(m_bev, &m_packetTimeout, &m_writeTimeout);
    }
}

void Client::setBev(struct bufferevent *bev)
{
        m_bev = bev;
};

int Client::getSockErrorCode()
{
    return evutil_socket_geterror(m_fd);
}

const char* Client::getSockErrorString()
{
    return evutil_socket_error_to_string(evutil_socket_geterror(m_fd));
}
