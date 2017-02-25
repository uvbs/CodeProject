#ifndef _MSG_QUEUE_H
#define _MSG_QUEUE_H

#ifdef _LINUX64
#include <semaphore.h>
#endif
#include <list>
#include "mutex.h"


/**
 * @brief 实际的线程函数，由继承类实现，完成线程的操作
 * 无名信号灯（linux）
**/
#ifdef _LINUX64
class NoNameSemLock {
protected:
    sem_t	sem_id;
    int	m_waittime;	//信号灯等待时间，单位秒，0为一直等待
public:
    NoNameSemLock(int initial=0, int waittime=0);
    ~NoNameSemLock();
public:	
    int p();	
    int v();	
    int getvalue(int *sval);
    void destory();
public:
    inline int lock()	{	return p();	}
    inline int unlock()	{	return v();	}
};
#endif


template<class QITEMTYPE>
class MsgQueue
{
    typedef std::list<QITEMTYPE> ItemList;

private:
#ifdef _LINUX64
    NoNameSemLock * m_sem;
#endif
    Mutex m_locker;
    ItemList m_queue;
    int m_waittime;	//pop等待之间，单位秒，0为一直等待a
    int m_initial;	//队列初始长度

public:
    int size()
    {
        int msg_size = 0;
        Lock lock(m_locker);
        {
            msg_size=m_queue.size();
        }
        return msg_size;
    };

    //从队列中获取数据
    //返回值 　< 0 失败 超时时errno为ETIMEDOUT
    //	   >= 0 pop出数据后消息队列的长度
    int pop(QITEMTYPE& data)
    {
        int rv = 0;
        int msg_size=0;
#ifdef _LINUX64
        rv = m_sem->p();
#endif
        if(rv == 0){
            Lock lock(m_locker);
            {
                if (m_queue.size() > 0)
                {
                    data= m_queue.front();
                    m_queue.pop_front();
                    msg_size=m_queue.size();
                }
            }
            return msg_size;
        }
        return rv;
    }

    //向队列中push数据
    //返回值 push后消息队列长度
    int push(QITEMTYPE data)
    {
        int msg_size=0;
        {	
            Lock lock(m_locker);
            //if(m_Buf.size() >= m_QueueLimit) {
            //    P2P_LOG_WARNING("DROP[int udpserver], UdpQueueSize[%d]", m_QueueLimit);
            //    return;
            //}
            m_queue.push_back(data);
            msg_size=m_queue.size();
                        
        }
#ifdef _LINUX64
        m_sem->v();
#endif
        return msg_size;
    }

    MsgQueue():m_initial(0),m_waittime(0)
    {
#ifdef _LINUX64
        m_sem=new NoNameSemLock(m_initial, m_waittime);
#endif
    }

    MsgQueue(int initial, int waittime):m_initial(initial),m_waittime(waittime)
    {
#ifdef _LINUX64
        m_sem=new NoNameSemLock(m_initial, m_waittime);
#endif
    }

    ~MsgQueue()
    {
#ifdef _LINUX64
        delete m_sem;
#endif
    }
};

#endif  //__msgqueue__h__

