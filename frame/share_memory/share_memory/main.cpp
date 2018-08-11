/**********************************
** @file: main.cpp
** @brief:  SHMServer�������
** @autor: by Mr.Chen
** @date: 2016/07/16 15:28
**********************************/
#include "seh_file.h"
#include "shm_server.h"
#include "myassert.h"

int		main(int argc, char* argv[])
{
#ifdef _WIN32
	SetUnhandledExceptionFilter(X_ProcessException);
#endif
	__ENTER_FUNCTION

	ShmServer s;
	bool ret = s.init();
    MyAssert(ret);
	ret = s.start();
    MyAssert(ret);
	ret = s.wait();
    MyAssert(ret);

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
