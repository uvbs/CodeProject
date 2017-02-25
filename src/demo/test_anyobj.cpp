////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_anyobj.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 1:38
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_anyobj.h"

void test_anyobj()
{
	Any testAny(1);
	int* pINT = any_cast<int>(&testAny);
	std::cout<<*pINT<<std::endl;

	testAny = std::string("HelloWorld!");
	std::string* pSTRING = any_cast<std::string>(&testAny);
	std::cout<<pSTRING->c_str()<<std::endl;

}

