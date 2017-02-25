#include "msg_queue.h"
#include <time.h>

#ifdef _LINUX64
NoNameSemLock::NoNameSemLock(int initial, int waittime)
{	
	sem_init(&sem_id, 0, initial); 
    m_waittime=waittime;
}

NoNameSemLock::~NoNameSemLock()
{	
	destory();
}

int NoNameSemLock::p()			
{
	if(m_waittime>0)
    { 
        struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec+=m_waittime;
        return sem_timedwait(&sem_id, &ts);
	}
	else
    {
		return sem_wait(&sem_id);
	}
}

int NoNameSemLock::v()
{
    return sem_post(&sem_id);
}

int NoNameSemLock::getvalue(int *sval)	
{ 
    return sem_getvalue(&sem_id, sval);
}

void NoNameSemLock::destory()
{ 
    sem_destroy(&sem_id);
}

#endif
