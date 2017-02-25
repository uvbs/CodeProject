////////////////////////////////////////////////////////////////////////////////////
// 
// @file: SEHFile.h
// @author: by Mr.Chen
// @date: 2014/8/21	19:10
// @brief: 系统结构化异常 only for windows
//			  ref from http://www.codeproject.com/tools/minidump.asp 
//			  generate stack frame pointers for all functions - same as /Oy- in the project
////////////////////////////////////////////////////////////////////////////////////
#ifndef __SEH_FILE_H_
#define __SEH_FILE_H_

#include "type.h"

#ifdef _WIN32

#include <shlwapi.h>
#include <stdio.h>
#pragma comment(lib, "shlwapi")

//#include "<new.h>"
#pragma optimize("y", off)

long WINAPI X_ProcessException(PEXCEPTION_POINTERS pException);

// 抛出字符串形式异常
void X_ThrowStringException(const wchar_t* szFmt, ...);

// 抛出断言形式异常
void X_ThrowAssertException(const wchar_t* szFileName, int nFileLine, const wchar_t* pExpr);

// 显示一个内部异常
void  X_ProcessInnerException(PEXCEPTION_POINTERS pException, HWND hParentWnd, const wchar_t* szTitle);

// 生成基本描述文件
bool CreateSmallDumpInfo(PEXCEPTION_POINTERS pException, uint dwLastError);

// 生成完整表述文件
bool CreateBigInfoFile(PEXCEPTION_POINTERS pException, uint dwLastError);

// 生成dump报告
bool CreateDumpHelpFile(PEXCEPTION_POINTERS pException);

#endif	// _WIN32

#endif	//__SEH_FILE_H_

