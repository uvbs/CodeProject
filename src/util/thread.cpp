#include "thread.h"
#include "type.h"

namespace sys_util {

#if defined(_WIN32)
uint __stdcall PlatformThreadFunction(void* p)
{
	if (NULL == p)
		return 0;

	Thread::ThreadStartInfo* pInfo = static_cast<Thread::ThreadStartInfo*>(p);
	pInfo->function(pInfo->param);

	return 0;
}

#elif defined(_LINUX64)
void* PlatformThreadFunction(void* p)
{
	if (NULL == p)
		return NULL;

	Thread::ThreadStartInfo* pInfo = static_cast<Thread::ThreadStartInfo*>(p);
	pInfo->function(pInfo->param);

	return NULL;
}
#endif

Thread::Thread() 
{
#if defined(_WIN32)
	_handle = NULL;
#endif
	_status = TS_SLEEP;
	_tid	 = 0;	
}

Thread::~Thread() 
{
#if defined(_WIN32)
	if (_handle)
	{
		CloseHandle(_handle);
		_handle = NULL;
	}

#endif
	_status = TS_STOP;
	_tid	 = 0;
}

int Thread::start(cb_t function, void* param, bool startAtOnce)
{
	_startInfo.function	= function;
	_startInfo.param		= param;

#if defined(_WIN32)
	_handle = (HANDLE)_beginthreadex(NULL, 0, PlatformThreadFunction, &_startInfo, CREATE_SUSPENDED, &_tid);

#elif defined(_LINUX64)
	int result = pthread_create(&_tid, 0, PlatformThreadFunction, &_startInfo);
	if (result != 0)
	{
		return -1;
	}
#endif
	if (startAtOnce)
	{
		resume();
	}

	return 0;
}

void Thread::resume()
{
	if (TS_SLEEP != _status)
	{
		return;
	}
#if defined(_WIN32)
	ResumeThread(_handle);
#endif
	_status = TS_RUN;
}

void Thread::wait()
{
	if (_tid == 0)
	{
		return;
	}

#if defined(_WIN32)
	DWORD exitCode;
	while (true)
	{
		if (GetExitCodeThread(_handle, &exitCode) != 0)
		{
			if (exitCode != STILL_ACTIVE)
			{
				break;
			}
			else
			{
				WaitForSingleObject(_handle,INFINITE);
			}
		}
		else
		{
			// log something here
			break;
		}
	}

	CloseHandle(_handle);
	_handle = NULL;

#elif defined(_LINUX64)
	void* exitCode;
	if (_tid != pthread_self())
	{
		int r = pthread_join(_tid, &exitCode);
		if (r != 0)
		{
			std::cerr << "pthread_join() returned " << r << std::endl;
		}
	}
#endif

	_tid = 0;
}

} // namespace sys_util
