#ifndef __my_server_client__h__
#define __my_server_client__h__

#include "macrodef.h"
#include "server_client.h"

class MyServerClient : public ServerClient
{
public:
    MyServerClient(){};
    virtual ~MyServerClient(){};

public:
    int onPacket(char* buf, size_t buf_len);
    int onTimeout();
    int onSendTimeout();
    int onClose(int type);

    int onData(char* buf, size_t buf_len);
};

#endif
