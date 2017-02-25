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
 * @brief task���ֻ࣬�����й��̵߳Ĳ���
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
     * @brief �����߳�
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
     * @brief �̺߳���
     *
     * @param [in/out] void* args ������ָ��
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
     * @brief ʵ�ʵ��̺߳������ɼ̳���ʵ�֣�����̵߳Ĳ���
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
