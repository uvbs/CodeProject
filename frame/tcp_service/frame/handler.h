#ifndef __handler__h__
#define __handler__h__

#include "packet_parser.h"

class Connector;
class ServerClient;
class BaseHandler
{
public:
    BaseHandler(unsigned short port):m_port(port),m_parser(NULL)
    {
        m_clientcount = 0;
        m_connectorcount = 0;
    };

    BaseHandler():m_port(0),m_parser(NULL)
    {
        m_clientcount = 0;
        m_connectorcount = 0;
    };

    virtual ~BaseHandler(){};
public:
    void setParser(PacketParser* parser);
    PacketParser* getParser();

    void setPort(unsigned short port);
    unsigned short getPort();

    void addClientCount()
    {
       m_clientcount++; 
    };

    void decClientCount()
    {
       if(m_clientcount > 0){
          m_clientcount--;
       }
    };

    unsigned int getClientCount()
    {
        return m_clientcount;
    };

    void addConnectorCount()
    {
       m_connectorcount++;
    };

    void decConnectorCount()
    {
       if(m_connectorcount > 0){
          m_connectorcount--;
       }
    };

    unsigned int getConnectorCount()
    {
        return m_connectorcount;
    };

public:
    //生成客户端对象，如不生成返回NULL
    virtual ServerClient* onAccept(struct sockaddr_in *sin, int socklen) = 0;
    virtual int handleData(ServerClient* client, char* buf, size_t buf_len){};
    virtual int handleData(Connector* client, char* buf, size_t buf_len){};

protected:
    unsigned short     m_port;
    PacketParser*      m_parser;

    unsigned int       m_clientcount;
    unsigned int       m_connectorcount;
};

#endif
