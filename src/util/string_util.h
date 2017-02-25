////////////////////////////////////////////////////////////////////////////////////
// 
// @file: string_util.h
// @author: by Mr.Chen
// @date: 2014/5/1	19:48
// @brief: 字符串处理类，ref from CYOU and PWRD.
//          
////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __STRUTIL_H_
#define __STRUTIL_H_

#include "type.h"
#include <vector>
#include <string>
#include <stdarg.h>

using std::string;
using std::vector;

namespace str_util {

	//字符串拷贝
	char* strcpy(char* pDestination, const char*  pSource);		
	char* strncpy(char* pDestination, const char* pSource, uint length);	
	// 注意！destSizeInByte为目的缓冲区的sizeof，而非strlen
	char* strcpy_s(char* pDestination, const char* pSource, const int maxlen);

	// 字符串组合
	char* strcat(char* pDestination, const char* pSource);				
	char* strncat(char* pDestination, const char* pSource, uint length);	

	// 字符串比较
	int strcmp(const char* pSource1, const char* pSource2);								
	int strncmp(const char* pSource1, const char*  pSource2, uint length);	

	// 计算字符串长度
	int strlen(const char* pSource);	
	int strUtf8len(const char* pSource);

	// 查找字符串
	const char* strstr(const char* pHaystack, const char* pNeedle);

	//格式化字符串
	int snprintf(char *buf, int maxlen, const char *format, ...);
	int vsnprintf(char* buf, int maxlen, const char* format, va_list varList);

	// 分解字符串
	char* strtok(char* pSource, const char* pDelimit);		
	char* strtok_s(char*  pSource, const char* pDelimit, char** pContext);
	// 切分字符串
	int	 strToVector(const char* strSource, vector<string>& vRet, 
						   const char* strKey="\\/\n", bool bOneOfKey = true, bool bIgnoreEmpty = true);
	bool split(const string& str, char delim, vector<string>& r);

	// 从buffer中解析出一行
	const char* getLineFromMemory(char* pStringBuf, int nBufSize, 
		const char* pMemStart, const char* pMemEnd);

	//字符串转整型
	int strToInt(const char* pSource);
    //字符串转无符号整型
    uint strToUint(const char*  pSource, char**  pEnd = NULL, int nBase = 10);
	//字符串转64位整型
	int64 strToInt64(const char*  pSource, char** pEnd = NULL, int nBase = 10);
	//字符串转64位无符号整型
    uint64 strToUint64(const char*  pSource, char** pEnd = NULL, int nBase = 10);
	//字符串转长整型
	long strToL(const char*  pSource, char** pEnd = NULL, int nBase = 10);
	//字符串转无符号长整型
	ulong strToUL(const char*  pSource, char** pEnd = NULL, int nBase = 10);
	//字符串转浮点型
	float strToFloat(const char* pSource);
	//字符串转浮点型
	double strToDouble(const char* pSource, char** pEnd);

	//整型转字符串
	char* int32ToStr(int nValue, char* pString, int nRadix);
	bool uint64ToStr(uint64 i, char* pDest, int base = 10);
	bool floatToStr(float i, char* pDest, int base = 10);

	// 去除字符串中空格和tab
	int	 trimRight(char* s);
	bool trimLeft(char* pSource);						
	bool trimRightV2(char* pSource);
	bool trim(char* pSource);
    // 删除字符串str的收尾字符c
    bool trim(string& str, char c) ;

	//判断字符串是否数字
	bool isInteger(const char* pSource);
	bool isFloat(const char* pSource);

	//杂项
	void toUpper(std::string &str);
	void toLower(std::string &str);
	std::string bin2hex(const void *data, unsigned int len);
	bool hex2bin(std::string &hex_str, unsigned char *&data, unsigned int len);
    string bytesToHexString(const unsigned char* input, size_t length);

    //编码转换
    bool gbkToUtf8(const char *src, string& result);
    bool utf8ToGbk(const char *src, string& result);

} // namespace str_util
using namespace str_util;

#endif	// __STRUTIL_H_
