#ifndef _P2P_TASK_H
#define _P2P_TASK_H

#include "task.h"
#include "p2pmsg.h"

class P2pTask : public Task<P2pMsgInfo>
{
public:
    P2pTask(int initial, int waittime, int taskindex, int mqMaxLength):
        Task<P2pMsgInfo>(initial, waittime, taskindex),m_recvCount(0),m_discardCount(0)
    {
        m_mqMaxLength = mqMaxLength;
    }
	
    P2pTask(int taskindex, int mqMaxLength):
        Task<P2pMsgInfo>(0, 0, taskindex)
    {
        m_mqMaxLength = mqMaxLength;
    }
	
    virtual void svc();
    virtual int onEvent(P2pMsgInfo* package);
    int     add(P2pMsgInfo data);
    int     getAndZeroRecvCount();
    int     getAndZeroDiscardCount();
	
private:
    int         m_mqMaxLength;
    int         m_recvCount;
    int         m_discardCount;
};

#endif  //__p2p_task__h__

