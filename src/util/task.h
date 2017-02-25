#ifndef _TASK_H
#define _TASK_H

#include "msg_queue.h"
#include "thread.h"
#include "type.h"

/**
 * @brief task基类，只包含有关线程的操作
 * 
**/
class BaseTask
{
private:
    Thread			m_thread;

protected:
    bool              m_shutdown;

public:
    BaseTask(){
        m_shutdown = false;
    };
    virtual ~BaseTask(){};

public:
    virtual void run()
    {
        m_thread.start(run_svr, this);
    }

    static void* run_svr(void *args)
    {
        BaseTask *t = (BaseTask *) args;
        t->svc();
        return NULL;
    }

    /**
     * @brief 实际的线程函数，由继承类实现，完成线程的操作
     *
    **/
    virtual void svc(){return;}

    void stop(){m_shutdown = true;}

    void wait(){m_thread.wait();}
};


/**
 * @brief p2ptask模板类，包含一个消息队列和一个线程，线程相关操作从基类继承而来
 * @see 
 * @author zhaojianlong
 * @date 2011/08/11
**/
template<class QITEMTYPE>
class Task : public BaseTask
{
typedef MsgQueue<QITEMTYPE> TaskQueue;

protected:
    //消息队列
    TaskQueue	*m_mq;
    //任务线程编号
    int                m_taskindex;

private:
    //m_mq pop等待之间，单位秒，0为一直等待
    int				    m_waittime;	
    //m_mq 队列初始长度
    int                m_initial;	
    //已分配的用户数（大概值，计算时不加锁）
    uint              m_usersnum;

public:
    Task():m_initial(0),m_waittime(0),m_taskindex(0),m_usersnum(0)
    {		
        m_mq = new TaskQueue(m_initial, m_waittime);
    }

    Task(int initial, int waittime):m_initial(initial),m_waittime(waittime),m_taskindex(0),m_usersnum(0)
    {
        m_mq = new TaskQueue(m_initial, m_waittime);
    }

    Task(int initial, int waittime, int taskindex):m_initial(initial),m_waittime(waittime),m_taskindex(taskindex),m_usersnum(0)
    {
        m_mq = new TaskQueue(m_initial, m_waittime);
    }

    virtual ~Task()
    {
        delete m_mq;
    }

public:
    int getTaskIndex(){return m_taskindex;};
    int size(){return m_mq->size();}; 

    /**
     * @brief 向消息队列添加数据
     *
     * @param [in/out] QITEMTYPE data 消息队列数据项
     * @return int 待定 （考虑服务器忙时可丢弃一些数据？）
     * @retval   
     * @see 
     * @author zhaojianlong
     * @date 2011/08/11
    **/
    int add(QITEMTYPE data){return m_mq->push(data);};
   
    void addUser(){m_usersnum++;};
    
    unsigned int getUserCount(){return m_usersnum;}

    void delUser(){m_usersnum--;};

    virtual void svc(){return;};

    /**
     * @brief 数据处理函数，由svc调用，继承类实现
     *
     * @param [in/out]
     * @return int 
     * @retval   
     * @see 
     * @author zhaojianlong
     * @date 2011/08/11
    **/
    virtual int onEvent(QITEMTYPE *package) = 0;

    /**
     * @brief 连接关闭函数，由svc调用，继承类实现
     *
     * @param [in/out]
     * @return int 
     * @retval   
     * @see 
     * @author zhaojianlong
     * @date 2011/08/11
    **/
    //virtual int onClose(QITEMTYPE *package) = 0;
};

#endif  //__task__h__
