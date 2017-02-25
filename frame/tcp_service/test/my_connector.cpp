#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _LINUX64
#include <unistd.h>
#include <sys/socket.h>
#endif
#include "my_handler.h"
#include "my_connector.h"

//int MyConnector::onConnect()
//{
//    m_connRetryCount = m_connRetryCount % 10;
//	struct sockaddr_in sa;
//	socklen_t len = sizeof(struct sockaddr_in);
//	getsockname(m_fd, (struct sockaddr*)&sa, &len);
//    com_writelog(COMLOG_NOTICE, "MyConnector::onConnect fd:%d, register channel:%s, ip:%s, local port:%d", 
//        m_fd, m_channelID, GetServerIP(), ntohs(sa.sin_port));
//    char reqbuf[128]={0};
//    sprintf(reqbuf,
//        "GET /register?fid=%s HTTP/1.1\r\nContent-Length:%d\r\n\r\n", m_channelID, 0);
//    int headerlen = strlen(reqbuf);
//
//    send_msg(reqbuf, headerlen);
//    return 0;
//}

int MyConnector::onPacket(char* buf, size_t buf_len)
{
    //com_writelog(COMLOG_NOTICE, "MyConnector::onPacket fd:%d, %d", m_fd, buf_len);
    m_handler->handleData(this, buf, buf_len);
	this->m_readTimeoutCount = 0;
    return 0;
}

int MyConnector::onClose(int type)
{
    if(type == 1){
         printf("MyConnector::onClose, connection close fd:%d\r\n", m_fd);
    }
    else{
        printf("MyConnector::onClose, connection error fd:%d\r\n", m_fd);
    }

    if(m_bClose == true)
	{
		delete this;
    }
    else{
        //重新连接source    
       
    }
    return 0;
}

int MyConnector::onTimeout()
{
    if(m_tostate == TO_CONN){
        printf("MyConnector::onTimeout, type is connect ,fd:%d\r\n", m_fd);
    }
	//debug
    else if(m_tostate == TO_RECV){
         printf("MyConnector::onTimeout, type is recvt ,fd:%d\r\n", m_fd);
    }
	//debug
    else if(m_tostate == TO_PACKET){
        printf("MyConnector::onTimeout, type is packet ,fd:%d\r\n", m_fd);
    }
    
    this->onClose(1);
    return 0;
}

int MyConnector::onSendTimeout()
{
    printf("MyConnector::onTimeout, fd:%d\r\n", m_fd);
    this->close();
}
