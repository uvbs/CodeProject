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

//����״̬
enum client_state{
    ST_INIT = 0,
    ST_CONN  =1,
    ST_HEADER  =2,
    ST_BODY =3,
    ST_CLOSE = 99
};

//��ʱʱ��״̬����Ŀǰ������ʹ���ĸ���ʱʱ��
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
    //�麯�����̳���ʵ�֣��ѽ�������ͷ����0������һ��Ҫ��дm_headerlen,m_bodylen �� m_packagelen
    //��ͷ����������1�� Э����󷵻�-1
    virtual int onHeader(const char* buf, size_t buf_len) = 0;

    //�麯����������һ�������������ϲ㴦��
    virtual int onPacket(char* buf, size_t buf_len) = 0;

    //�麯�������ӹر�,type = CLIENT_FD_CLOSE,���������رգ� CLIENT_FD_ERROR ���Ӵ���
    virtual int onClose(int type) = 0;

    //�麯������ʱ���紦���곬ʱ��Ҫ�ر����ӣ�����0�����ر����ӷ���1
    virtual int onTimeout() = 0;

    virtual int onSendTimeout() = 0;
public:
    //��������
    virtual void send_msg(char* msg, size_t msg_len);
	//���ػ�������С
	int getOutputBufferLength();
	int getInputBufferLength();
    //�ر�����
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
    //����socket
    int m_fd;

    //bufferevent
    struct bufferevent *m_bev;

    //�ͻ����հ�״̬
    client_state  m_state;

    //�ͻ��˳�ʱʱ������״̬
    timeout_state m_tostate;

    size_t m_headerlen;
    size_t m_bodylen;
    size_t m_packetlen;

    struct timeval     m_connectTimeout;
    struct timeval     m_recvTimeout;
    struct timeval     m_packetTimeout;
    struct timeval     m_writeTimeout;

    //Э����� ��
    PacketParser*        m_parser;

    char*              m_buf;
    size_t             m_buflen;

    //�߳�����
    int                m_threadIndex; 

    //������ɺ�ɾ��
    bool               m_bCloseAfterSend;
};

#endif
