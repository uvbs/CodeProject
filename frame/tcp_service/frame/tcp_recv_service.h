#ifndef __TcpRecvService__h__
#define __TcpRecvService__h__

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "thread.h"
#include "server_client.h"

#pragma pack(push,1)

struct pipe_msg
{
    int    fd;
    void*  ptr;
};

struct channel_pipe_msg
{
	void* client; //连接指针
	void* userdata; //数据指针
	int len;	//数据长度
};

class recvthread : public sys_util::Thread
{
    //virtual void thread(){return;};
};

#pragma pack(pop)

/**
 * @brief  
 * @see 
**/
class TcpRecvService
{
private:
    recvthread		   m_thread;
    struct event_base*     m_base;
    int                    m_readpipefd; 
    int                    m_writepipefd;
    int			   m_channelrdpipefd;
    int			   m_channelwrpipefd;
    int                    m_index; 
protected:
    bool                         m_shutdown;
public:
    TcpRecvService(){
        m_shutdown = false;
        m_base = NULL;
        m_readpipefd = 0;
        m_writepipefd = 0;
        m_index = 0;
    };
    virtual ~TcpRecvService(){};

public:
    struct event_base*  getBase()
    {
        return m_base;
    };

    int getReadPipeFD()
    {
        return m_readpipefd;
    };

    int getWritePipeFD()
    {
        return m_writepipefd;
    };

	int getChannelWrPipeFD()
	{
		return m_channelwrpipefd;
	}

    int getIndex()
    {
        return m_index;
    };

public:
    static void pipe_readcb(evutil_socket_t fd, short what, void* arg);
	static void pipe_channel_readcb(evutil_socket_t fd, short what, void* arg);
    
    int initService(int inputpipefd, int outputpipefd, int index);

    /**
     * @brief 启动线程
     *
     * @param [in/out] 
     * @return void
     * @retval   
     * @see 
     * @author zhaojianlong
     * @date 2011/08/11
    **/
    virtual void run()
    {
        m_thread.start(run_svr, this);
    }

    /**
     * @brief 线程函数
     *
     * @param [in/out] void* args 类对象的指针
     * @return void
     * @retval   
     * @see 
     * @author zhaojianlong
     * @date 2011/08/11
    **/
    static void* run_svr(void *args)
    {
        TcpRecvService *t = (TcpRecvService *) args;
        t->svc();
        return NULL;
    }

    /**
     * @brief 实际的线程函数，由继承类实现，完成线程的操作
     *
     * @param [in/out]
     * @return void
     * @retval   
     * @see 
     * @author zhaojianlong
     * @date 2011/08/11
    **/
    virtual void svc()
    {
        if(m_base){
            printf("TcpRecvService start, index:%d\r\n", m_index);
            event_base_dispatch(m_base);
            printf("TcpRecvService end, index:%d\r\n", m_index);
        }
    };

    void stop(){m_shutdown = true;};

    void wait(){m_thread.wait();};
};

#endif
