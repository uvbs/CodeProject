#include "listener.h"

int SrvListener::initListener()
{
    if(m_handler == NULL){
        printf("SrvListener::initListener fail, handler is NULL.\n");
        return -1;
    }

    m_port = m_handler->getPort();
    m_socketlisten = socket(AF_INET, SOCK_STREAM, 0);

    if (m_socketlisten < 0)
    {
        printf("SrvListener::initListener Failed to create listen socket.\n");
        return -1;
    }

    memset(&m_addresslisten, 0, sizeof(m_addresslisten));

    m_addresslisten.sin_family = AF_INET;
    m_addresslisten.sin_addr.s_addr = INADDR_ANY;
    m_addresslisten.sin_port = htons(m_port);

    return 0;
}

int SrvListener::initListener(BaseHandler* handler) 
{
    m_handler = handler;

    if(m_handler == NULL){
        printf("SrvListener::initListener fail, handler is NULL.\n");
        return -1;
    }

    m_port = m_handler->getPort();
    m_socketlisten = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socketlisten < 0)
    {
        printf( "SrvListener::initListener Failed to create listen socket.\n");
        return -1;
    }

    memset(&m_addresslisten, 0, sizeof(m_addresslisten));
    
    m_addresslisten.sin_family = AF_INET;
    m_addresslisten.sin_addr.s_addr = INADDR_ANY;
    m_addresslisten.sin_port = htons(m_port);
    
    return 0;
}
   
int SrvListener::registerToService(ListenService *pSerivce, int backlog)
{
    if(m_handler == NULL){
        printf("SrvListener::registerToService fail, handler is NULL.\n");
        return -1;
    }

    struct event_base *base = pSerivce->getBase();
    if(base == NULL){
        printf("SrvListener::registerToService fail, Service is not init, eventbase is NULL.\n");
        return -1;
    }

    m_listener = evconnlistener_new_bind(base, SrvListener::listener_cb, (void *)this,
        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, backlog,
        (struct sockaddr*)&m_addresslisten,
        sizeof(struct sockaddr));

    if (!m_listener) {
        printf("SrvListener::registerToService fail, Could not create a listener!\n");
        return -1;
    }
    else{
        printf("SrvListener::registerToService Listen in port %d success.\n", getPort());
    }

    setBase(base);
    setService(pSerivce);

    return 0;
}

void SrvListener::listener_cb(struct evconnlistener *evlistener, evutil_socket_t fd,struct sockaddr *sa, int socklen, void *user_data)
{
    SrvListener* pListener  = (SrvListener*)user_data;
    if(pListener == NULL){
        printf("SrvListener::listener_cb fail, Listener(user_data) is NULL.\n");
        return;
    }

    ListenService *lService= pListener->getService();
    if(pListener == NULL){
        printf("SrvListener::listener_cb fail, Listener getService is NULL.\n");
        return;
    }

    TcpRecvService *rService= lService->getRandomService(fd);
    struct sockaddr_in *sin = (struct sockaddr_in *)sa;
    BaseHandler *pHandler = pListener->getHandler();
    if(pHandler == NULL){
        printf("SrvListener::listener_cb fail, Listener get Handler Fail, fd:%d\n", fd);
        return;
    }

    ServerClient* srvclient = pHandler->onAccept(sin, socklen);
    if(srvclient){
        char* ip = inet_ntoa(sin->sin_addr);
        unsigned port = ntohs(sin->sin_port);
        srvclient->setClientAddr(ip, port);
        srvclient->setParser(pHandler->getParser());
        srvclient->setHandler(pHandler);

        srvclient->setConnectTimeoutValue(pListener->getConnectTimeout());
        srvclient->setRecvTimeoutValue(pListener->getRecvTimeout());
        srvclient->setPacketTimeoutValue(pListener->getPacketTimeout());
        srvclient->setWriteTimeoutValue(pListener->getWriteTimeout());

        srvclient->setListener(pListener);
        srvclient->setThreadIndex(rService->getIndex());

        int writePipeFD = rService->getWritePipeFD();
        struct pipe_msg msg;
        msg.fd = fd;
        msg.ptr = (void*)srvclient;
        int writelen = 0;

#ifdef _LINUX64
        writelen = write(writePipeFD, (char*)&msg, sizeof(struct pipe_msg));
#else
        if(!WriteFile((HANDLE)writePipeFD, (char*)&msg, sizeof(struct pipe_msg), (LPDWORD)&writelen, NULL))
            return;

#endif
        
        printf("SrvListener listener_cb:: accept a connect in listener port:%d, fd:%d, host:%s, port:%d\n", 
            pListener->getPort(), fd, ip, port);
    }
}

void SrvListener::freeListener()
{
    if(m_listener){
        evconnlistener_free(m_listener);
    }
}

