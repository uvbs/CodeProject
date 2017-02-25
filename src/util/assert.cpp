#include "assert.h"
#include "macrodef.h"
#include <stdio.h>
#include <stdarg.h>

namespace sys_util {

	void __show__(char* str)
	{
		//±£´æÈÕÖ¾
#ifdef __SAVELOG__
		if (LogSystem::getSinglePtr())
		{
			LogSystem::getSinglePtr()->saveLog(LOG_FILE_FATAL, str);
		}
#endif
		printf("Assert:%s", str);
		throw(1);
	}

	void	__assert__(const char* file, uint line, const char* function, const char* exception)
	{
		char strLog[MAX_SHOWMESSAGE_LENGTH] = {0};

#ifdef _WIN32
		tsnprintf( strLog, sizeof(strLog), "[%s][%d][%s][%s]", file, line, function, exception ) ;
#endif

#ifdef _LINUX64
		tsnprintf( strLog, sizeof(strLog), "[%s][%d][%s][%s]\n", file, line, function, exception ) ;
#endif
		__show__(strLog);
	}

	void	__assertex__(const char* file, uint line, const char* function, const char* exception, const char* msg)
	{
		char strLog[MAX_SHOWMESSAGE_LENGTH] = {0};

#ifdef _WIN32
		tsnprintf( strLog, sizeof(strLog), "[%s][%d][%s][%s]\n[%s]", file, line, function, exception ,msg ) ;
#endif

#ifdef _LINUX64
		tsnprintf( strLog, sizeof(strLog), "[%s][%d][%s][%s]\n[%s]\n", file, line, function, exception ,msg ) ;
#endif
		__show__(strLog);
	}

	void	__assertspecial__(const char* file, uint line, const char* function, const char* exception, const char* msg, ...)
	{
		char strLog[MAX_SHOWMESSAGE_LENGTH] = {0};

		va_list argp;
		va_start(argp, msg);
		char buf[MAX_SHOWMESSAGE_LENGTH] = {0};

		int nCount = tvsnprintf(buf, MAX_SHOWMESSAGE_LENGTH, msg, argp);
		va_end(argp);

#ifdef _WIN32
		tsnprintf(strLog, sizeof(strLog), "[%s][%d][%s][%s]\n[%s]", file, line, function, exception, buf);
#endif

#ifdef _LINUX64
		tsnprintf(strLog, sizeof(strLog), "[%s][%d][%s][%s]\n[%s]\n", file, line, function, exception, buf);
#endif
		__show__(strLog);
	}

} // namespace sys_util

