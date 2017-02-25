////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_strutil.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 0:04
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_strutil.h"

void test_strutil()
{
	const char* str = "abcd/bcd\\def\n";
	vector<string> ret;
	strToVector(str,ret,"\\/\n");
	string str1 = "abcDEdfg";
	toUpper(str1);
	toLower(str1);
	char* str2 = "1234abcd";
	string ret2 = bin2hex(str2, str_util::strlen(str2));
	unsigned char ret3[9]={0};
	unsigned char* ret4 = ret3;
	hex2bin(ret2,ret4,8);

}
