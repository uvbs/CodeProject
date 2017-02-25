#ifndef _TASK_H
#define _TASK_H

#include "msg_queue.h"
#include "thread.h"
#include "type.h"

/**
 * @brief task���ֻ࣬�����й��̵߳Ĳ���
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
     * @brief ʵ�ʵ��̺߳������ɼ̳���ʵ�֣�����̵߳Ĳ���
     *
    **/
    virtual void svc(){return;}

    void stop(){m_shutdown = true;}

    void wait(){m_thread.wait();}
};


/**
 * @brief p2ptaskģ���࣬����һ����Ϣ���к�һ���̣߳��߳���ز����ӻ���̳ж���
 * @see 
 * @author zhaojianlong
 * @date 2011/08/11
**/
template<class QITEMTYPE>
class Task : public BaseTask
{
typedef MsgQueue<QITEMTYPE> TaskQueue;

protected:
    //��Ϣ����
    TaskQueue	*m_mq;
    //�����̱߳��
    int                m_taskindex;

private:
    //m_mq pop�ȴ�֮�䣬��λ�룬0Ϊһֱ�ȴ�
    int				    m_waittime;	
    //m_mq ���г�ʼ����
    int                m_initial;	
    //�ѷ�����û��������ֵ������ʱ��������
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
     * @brief ����Ϣ�����������
     *
     * @param [in/out] QITEMTYPE data ��Ϣ����������
     * @return int ���� �����Ƿ�����æʱ�ɶ���һЩ���ݣ���
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
     * @brief ���ݴ���������svc���ã��̳���ʵ��
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
     * @brief ���ӹرպ�������svc���ã��̳���ʵ��
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
