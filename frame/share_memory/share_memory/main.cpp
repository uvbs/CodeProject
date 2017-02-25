/**********************************
** @file: main.cpp
** @brief:  SHMServer运行入口
** @autor: by Mr.Chen
** @date: 2016/07/16 15:28
**********************************/
#include "seh_file.h"
#include "shm_server.h"
#include "assert.h"

int		main(int argc, char* argv[])
{
#ifdef _WIN32
	SetUnhandledExceptionFilter(X_ProcessException);
#endif
	__ENTER_FUNCTION

	ShmServer s;
	bool ret = s.init();
    Assert(ret);
	ret = s.start();
    Assert(ret);
	ret = s.wait();
    Assert(ret);

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
