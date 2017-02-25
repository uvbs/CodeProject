#ifndef __client__h__
#define __client__h__

#ifdef _LINUX64
#include <sys/time.h>
#elif _WIN32
#include <windows.h>
#endif
#include <time.h>

#define CLIENT_FD_CLOSE 1
#define CLIENT_FD_ERROR 2

//连接状态
enum client_state{
    ST_INIT = 0,
    ST_CONN  =1,
    ST_HEADER  =2,
    ST_BODY =3,
    ST_CLOSE = 99
};

//超时时间状态，即目前本连接使用哪个超时时间
enum timeout_state{
    TO_CONN = 0,
    TO_RECV = 1,
    TO_PACKET =2
};


class PacketParser;

class Client
{
public:
    Client();
    virtual ~Client();
public:
    virtual int onData(char* buf, size_t buf_len) = 0;
    //虚函数，继承类实现，已解析出包头返回0，此事一定要填写m_headerlen,m_bodylen 和 m_packagelen
    //包头不完整返回1， 协议错误返回-1
    virtual int onHeader(const char* buf, size_t buf_len) = 0;

    //虚函数，接收完一个完整包交给上层处理
    virtual int onPacket(char* buf, size_t buf_len) = 0;

    //虚函数，连接关闭,type = CLIENT_FD_CLOSE,连接正常关闭， CLIENT_FD_ERROR 连接错误
    virtual int onClose(int type) = 0;

    //虚函数，超时，如处理完超时后要关闭连接，返回0，不关闭连接返回1
    virtual int onTimeout() = 0;

    virtual int onSendTimeout() = 0;
public:
    //返回数据
    virtual void send_msg(char* msg, size_t msg_len);
	//返回缓冲区大小
	int getOutputBufferLength();
	int getInputBufferLength();
    //关闭连接
    virtual void close();
public:    
    static void conn_readcb(struct bufferevent *bev, void *ctx);    
    static void conn_writecb(struct bufferevent *bev, void *user_data);

public:    
    void setFD(int fd)
    {
        m_fd = fd;
    };

    int getFD()
    {
        return m_fd;
    };

    void setState(client_state state)
    {
         m_state = state;
    }

    enum client_state getState()
    {
        return m_state;
    }

    size_t getHeaderLen(){return m_headerlen;};
    size_t getBodyLen(){return m_bodylen;};
    size_t getPacketLen(){return m_packetlen;};

    struct timeval* getConnectTimeout(){return &m_connectTimeout;};
    struct timeval* getRecvTimeout(){return &m_recvTimeout;};
    struct timeval* getPacketTimeout(){return &m_packetTimeout;};
    struct timeval* getWriteTimeout(){return &m_writeTimeout;};    

    void setConnectTimeoutValue(struct timeval* tv)
    {
        m_connectTimeout.tv_sec = tv->tv_sec;
        m_connectTimeout.tv_usec = tv->tv_usec;
    }

    void setRecvTimeoutValue(struct timeval* tv)
    {
        m_recvTimeout.tv_sec = tv->tv_sec;
        m_recvTimeout.tv_usec = tv->tv_usec;
    }

    void setPacketTimeoutValue(struct timeval* tv)
    {
        m_packetTimeout.tv_sec = tv->tv_sec;
        m_packetTimeout.tv_usec = tv->tv_usec;
    }

    void setWriteTimeoutValue(struct timeval* tv)
    {
        m_writeTimeout.tv_sec = tv->tv_sec;
        m_writeTimeout.tv_usec = tv->tv_usec;
    }

    timeout_state getTimeoutState()
    {
        return m_tostate;
    }    

    void setConnectTimeout();
    void setRecvTimeout();
    void setPacketTimeout();

    void setParser(PacketParser* parser);
    PacketParser* getParser();

    void setBev(struct bufferevent *bev);

    void setThreadIndex(int threadindex)
    {
        m_threadIndex = threadindex;
    };

    int getThreadIndex()
    {
        return m_threadIndex;
    };

    bool getCLoseAfterSend()
    {
        return m_bCloseAfterSend;
    };

public:
    char* getBuf(size_t buflen);
    int   getSockErrorCode();
    const char* getSockErrorString();
  
protected:
    //连接socket
    int m_fd;

    //bufferevent
    struct bufferevent *m_bev;

    //客户端收包状态
    client_state  m_state;

    //客户端超时时间设置状态
    timeout_state m_tostate;

    size_t m_headerlen;
    size_t m_bodylen;
    size_t m_packetlen;

    struct timeval     m_connectTimeout;
    struct timeval     m_recvTimeout;
    struct timeval     m_packetTimeout;
    struct timeval     m_writeTimeout;

    //协议解析 器
    PacketParser*        m_parser;

    char*              m_buf;
    size_t             m_buflen;

    //线程索引
    int                m_threadIndex; 

    //发送完成后删除
    bool               m_bCloseAfterSend;
};

#endif
