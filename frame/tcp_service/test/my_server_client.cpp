#include "my_server_client.h"
#include "http_request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MyServerClient::onData(char* buf, size_t buf_len)
{
    return 0;
}

int MyServerClient::onPacket(char* buf, size_t buf_len)
{
    m_handler->handleData(this, buf, buf_len);

    return 0;
}

int MyServerClient::onClose(int type)
{
    m_handler->decClientCount();
    if(type == 1){
        printf("MyServerClient::onClose, connection close fd:%d, clientCount:%d", 
            m_fd, m_handler->getClientCount());
    }
    else{
        printf("MyServerClient::onClose, connection error fd:%d, errno:%d, errstring:%s, clientCount:%d",
            m_fd, getSockErrorCode(), getSockErrorString(), m_handler->getClientCount());
    }
    
    delete this;
    return 0;
}

int MyServerClient::onTimeout()
{
    if(m_tostate == TO_CONN){
        printf("MyServerClient::onTimeout, type is connect ,fd:%d", m_fd);
    }
    else if(m_tostate == TO_RECV){
        printf("MyServerClient::onTimeout, type is recv ,fd:%d", m_fd);
    }
    else if(m_tostate == TO_CONN){
        printf("MyServerClient::onTimeout, type is packet ,fd:%d", m_fd);
    }
    this->close();
    return 0;
}

int MyServerClient::onSendTimeout()
{
    printf("MyServerClient::onSendTimeout fd:%d", m_fd);
    this->close();
    return 0;
}
