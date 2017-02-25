#include "helper.h"
#ifdef _WIN32
#include <windows.h>
#include <process.h>
#endif

#ifdef _LINUX64
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#endif

namespace sys_util {

	void	sleep(uint millionseconds)
	{
#if defined(_WIN32)
		Sleep(millionseconds) ;

#elif defined(_LINUX64)
		struct timeval tv;
		tv.tv_sec=0;
		tv.tv_usec = millionseconds;
		select(0,NULL,NULL,NULL,&tv);
#endif
	}

	ThreadId	getCurrTid()
	{
#if defined(_WIN32)
		return GetCurrentThreadId() ;

#elif defined(_LINUX64)
		return pthread_self();
#endif
	}
	
} //namespace sys_util

