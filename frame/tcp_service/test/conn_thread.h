#ifndef __CONN_THREAD__h__
#define __CONN_THREAD__h__

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "thread.h"
#include "listen_service.h"
#include "connector.h"


/**
 * @brief task基类，只包含有关线程的操作
 * @see 
 * @author zhaojianlong
 * @date 2011/08/11
**/
class ConnThread
{
private:
    Thread			    m_thread;
    ListenService*   m_service;                      
protected:
    bool                  m_shutdown;
public:
    ConnThread(){
        m_shutdown = false;
    };
    virtual ~ConnThread(){};

public:
    void setService(ListenService* lService)
    {
        m_service = lService;
    }
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
        ConnThread *t = (ConnThread *) args;
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
        mySleep(2);
        while(1){
            * parser = new MyParser(4);
            MyHandler* handler = new MyHandler(0);
            handler->setParser(parser);

            MyConnector* conn = new MyConnector();
            conn->setHandler(handler);
            conn->registerToService(m_service, -1);
            conn->connect("127.0.0.1", 8877);
            mySleep(1);
        }
    };

    void stop(){m_shutdown = true;};
    void wait(){m_thread.wait();};
};

#endif
