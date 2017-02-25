////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_thread.h
// @author: by Mr.Chen
// @date: 2015/4/21/ 23:56
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "type.h"
#include "thread.h"
#include <iostream>
using std::cout;
using std::endl;

class TestThread
{
public:
	void test()
	{
		cout << "test thread." << endl;
	}
	static void* run(void* param)
	{
		TestThread* p = (TestThread*)param;
		p->test();
		return NULL;
	}
	void start()
	{
		m_thread.start(&TestThread::run, this);
	}
	void wait()
	{
		m_thread.wait();
	}

private:
	Thread m_thread;
};

void test_thread();
