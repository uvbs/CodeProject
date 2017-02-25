#include "p2ptask.h"
#include <string>

void P2pTask::svc()
{
    printf("ProcessTask(svc) processtask start, index:%d\n", m_taskindex);
    P2pMsgInfo mi;
    int counter=0;
    while(!m_shutdown)
    {
        int mc = m_mq->pop(mi);
        //counter++;
        //if(counter>= 10000){
        //    counter = 0;
        //    com_writelog(COMLOG_DEBUG, "processtask msg task index:%d, length:%d", m_taskindex, mc);
        //} 
        if(mc >= 0){
            svr_msg_type smtype=mi.getMsgType();
            if(smtype == MSG_DATA){
                //P2P_LOG_PUSH("datalen", "%d", mi.getDataLen());
                //P2P_LOG_PUSH("task index", "%d", m_taskindex);
                //P2P_LOG_NOTICE("processtask get datamsg");
                counter++;          
                onEvent(&mi);
	            mi.releaseData();
            }
            else if(smtype == MSG_STATIC){
                printf("ProcessTask(svc) process data count:%d , size:%d, index:%d\n",
                    counter, this->size(), m_taskindex);
                counter = 0;
            }
            else if(smtype == MSG_STOP){
               printf(" ProcessTask(svc) processtask stop,index:%d\n", m_taskindex);
               break;
            } 
            else{
                    printf("Unknow Msg");
            }
        }
	else{

#ifdef _LINUX64
            if(errno == ETIMEDOUT){
	        //printf("getData timeout\r\n");
            }
#elif _WIN32
            if(errno == 10060L) {
                //printf("getData timeout\r\n");
            }
#endif
        }
    }
}

int P2pTask::add(P2pMsgInfo data)
{
    int mqLength = size();
    svr_msg_type smtype=data.getMsgType();
    
    if (m_mqMaxLength > 0 && mqLength >= m_mqMaxLength){
        if(smtype == MSG_DATA)
        {   
            m_recvCount++;
            m_discardCount++;
            //com_writelog(COMLOG_WARNING, "ProcessTask(svc) add mq is full, mq length:%d, task index:%d", 
            //mqLength, m_taskindex);
            return -1;
        }    
    }
    else{
        if(smtype == MSG_DATA){
            m_recvCount++;
        }
    }

    return Task<P2pMsgInfo>::add(data);
}

int P2pTask::onEvent(P2pMsgInfo* package)
{
    printf("on event !\n");
    return 0;   
}

int P2pTask::getAndZeroRecvCount()
{
    int tmpcount=m_recvCount;
    m_recvCount=0;
    return tmpcount;
}

int P2pTask::getAndZeroDiscardCount()
{
    int tmpcount=m_discardCount;
    m_discardCount=0;
    return tmpcount;
}

