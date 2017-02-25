////////////////////////////////////////////////////////////////////////////////////
// 
// @file: SEHFile.h
// @author: by Mr.Chen
// @date: 2014/8/21	19:10
// @brief: ϵͳ�ṹ���쳣 only for windows
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

// �׳��ַ�����ʽ�쳣
void X_ThrowStringException(const wchar_t* szFmt, ...);

// �׳�������ʽ�쳣
void X_ThrowAssertException(const wchar_t* szFileName, int nFileLine, const wchar_t* pExpr);

// ��ʾһ���ڲ��쳣
void  X_ProcessInnerException(PEXCEPTION_POINTERS pException, HWND hParentWnd, const wchar_t* szTitle);

// ���ɻ��������ļ�
bool CreateSmallDumpInfo(PEXCEPTION_POINTERS pException, uint dwLastError);

// �������������ļ�
bool CreateBigInfoFile(PEXCEPTION_POINTERS pException, uint dwLastError);

// ����dump����
bool CreateDumpHelpFile(PEXCEPTION_POINTERS pException);

#endif	// _WIN32

#endif	//__SEH_FILE_H_

