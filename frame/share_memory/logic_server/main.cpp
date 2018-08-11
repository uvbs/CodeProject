/***************************************************
** @file: main.cpp
** @author: Mr.Chen
** @date: 2016/7/25
** @brief: 
***************************************************/
#include "seh_file.h"
#include "logic_server.h"
#include "myassert.h"

int main(int argc, char* argv[])
{
#ifdef _WIN32
	SetUnhandledExceptionFilter(X_ProcessException);
	//_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
#endif

__ENTER_FUNCTION

	LogicServer s;
	bool bRet = s.init();
    MyAssert(bRet);

	bRet = s.loop();
    MyAssert(bRet);

	bRet = s.exit();
    MyAssert(bRet);

	return 0;
__LEAVE_FUNCTION
	return 0;
}
