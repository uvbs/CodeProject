////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_taskqueue.h
// @author: by Mr.Chen
// @date: 2015/4/22/ 23:46
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "type.h"
#include "p2ptask.h"
#include "p2pmsg.h"
#include "maths.h"
#include <vector>

class test_task
{
private:
	std::vector<P2pTask*>  m_pMsgTasks;
	int m_threadsnum;
	int m_maxMqLength;

public:
	test_task()
	{
		m_threadsnum = 1;
		m_maxMqLength = 10;
	}

	void startTask()
	{
		for (int i=0; i<m_threadsnum; ++i) 
		{
			P2pTask* pMsgTask = new P2pTask(0, 0, i, m_maxMqLength);
			m_pMsgTasks.push_back(pMsgTask);
			pMsgTask->run();
		}
	}

	void dispatchTask()
	{
		std::vector<P2pTask*>::iterator it = m_pMsgTasks.begin();
		for (; it!=m_pMsgTasks.end(); ++it)
		{
            int len = get_rand(1, m_maxMqLength);
            for (int i = 0; i < len; ++i)
			{
				P2pMsgInfo msg;
				msg.setMsgType(MSG_DATA);
				(*it)->add(msg);
			}
		}
	}

	void statisticTask()
	{
		std::vector<P2pTask*>::iterator it = m_pMsgTasks.begin();
		for (; it!=m_pMsgTasks.end(); ++it)
		{
			P2pMsgInfo msg;
			msg.setMsgType(MSG_STATIC);
			(*it)->add(msg);
		}
	}

	void stopTask()
	{
		std::vector<P2pTask*>::iterator it = m_pMsgTasks.begin();
		for (; it!=m_pMsgTasks.end(); ++it)
		{
			P2pMsgInfo msg;
			msg.setMsgType(MSG_STOP);
			(*it)->add(msg);
			(*it)->stop();
			(*it)->wait();
		}
	}

	int getThreadNum(void) { return m_threadsnum; }
	int getMaxMqLength(void) { return m_maxMqLength; }

};

int test_taskqueue();
