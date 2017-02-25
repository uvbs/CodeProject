#include <iostream>
//first include
#include "socketdef.h"
#include "ssample.h"
//test SEH
#include "log.h"
#include "seh_file.h"
#include "test_algorithm.h"
#include "test_container.h"
#include "test_boost.h"
#include "test_allocator.h"
#include "test_anyobj.h"
#include "test_bitset.h"
#include "test_cbfunc.h"
#include "test_conhash.h"
#include "test_datastruct.h"
#include "test_designpattern.h"
#include "test_httpclient.h"
#include "test_inireader.h"
#include "test_jsonc.h"
#include "test_maths.h"
#include "test_mysql.h"
#include "test_reflect.h"
#include "test_reflectV2.h"
#include "test_smartptr.h"
#include "test_spider.h"
#include "test_strutil.h"
#include "test_tabreader.h"
#include "test_tabreaderV2.h"
#include "test_taskqueue.h"
#include "test_thread.h"
#include "test_time.h"
#include "test_tinyxml.h"
#include "test_varsystem.h"
#include "test_log4cplus.h"

using namespace std;
using std::string;

int main()
{
#ifdef _WIN32
	__try
#endif
	{
		TimeSystem::getSinglePtr()->start();
		LogSystem::getSinglePtr()->init(1024*1024);
		LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "start");

		//int *pTest = NULL;
		//int aaa = *pTest;
        //test_log4cplus();
        test_boost();
		test_algorithm();
        test_allocator();
        test_container();
		test_anyobj();
		test_bitset();
		test_bitflags();
		test_cbfunc();
		test_conhash();
		test_datastruct();
		test_pattern();
		//test_httpost();
		//test_httpclient_jsonc();
		test_inireader();
		test_jsonc();
		test_maths();
		//test_mysql();
		test_reflect();
		test_reflectV2();
		test_shareptr();
        //test_shareptr2();
        test_safeptr();
		test_tabreader();
		test_tabreaderV2();
		//test_taskqueue();
		//test_tcpsocketIO();
		//test_thread();
		//test_time();
		test_XmlTest();
		test_varsystem();
		//wait
		cin.get();
	}

#ifdef _WIN32
	__except (
		X_ProcessException(GetExceptionInformation()), \
		TerminateProcess(GetCurrentProcess(), 0), \
		EXCEPTION_EXECUTE_HANDLER) 
	{}
#endif

	return 0;
}
