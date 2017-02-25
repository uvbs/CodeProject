////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_io_model.cpp
// @author: by Mr.Chen
// @date: 2015/4/23/ 0:22
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_io_model.h"

int test_io_model()
{
#ifdef _WIN32

	//SelectServer::startSrv();
	//MultiThreadSelectServer::startSrv();
	//EventSelect::startSrv();
	//EventOverlap::startSrv();
	//CompletionOverlap::startSrv();
	completion_port::startSrv();

#endif

	return 0;
}
