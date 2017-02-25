////////////////////////////////////////////////////////////////////////////////////
// 
// @file: assert.h
// @author: by Mr.Chen
// @date: 2014/5/11	01:37
// @brief: assert
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __ASSERT_H_
#define __ASSERT_H_

#include "type.h"

namespace sys_util {

	const int MAX_SHOWMESSAGE_LENGTH = 1024; //服务器对话框长度
	////////////////////////////////////////////////////////////////////////////////
	//	assert
	////////////////////////////////////////////////////////////////////////////////
	//异常
	void	__assert__(const char* file, uint line, const char* function, const char* exception);
	void	__assertex__(const char* file, uint line, const char* function, const char* exception, const char* msg);
	void	__assertspecial__(const char* file, uint line, const char* function, const char* exception, const char* msg, ...);

	#if defined(_WIN32)
	#define Assert(exception) ((void)((exception) ? 0 : (__assert__(__FILE__, __LINE__, __FUNCTION__, #exception), 0)))
	#define AssertEx(exception, msg) ((void)((exception) ? 0 : (__assertex__(__FILE__, __LINE__, __FUNCTION__, #exception, msg), 0)))
	#define AssertSpecial(exception, msg, ...) ((void)((exception) ? 0 : (__assertspecial__(__FILE__, __LINE__, __FUNCTION__, #exception, msg, __VA_ARGS__),0)))
	#endif

	#if defined(_LINUX64)
	#define Assert(exception) {if(!(exception)){__assert__(__FILE__, __LINE__, __PRETTY_FUNCTION__, #exception); }}
	#define AssertEx(exception, msg) {if(!(exception)){__assertex__(__FILE__, __LINE__, __PRETTY_FUNCTION__, #exception, msg); }}
	#define AssertSpecial(exception, msg, ...) {if(!(exception)){__assertspecial__(__FILE__, __LINE__, __PRETTY_FUNCTION__, #exception, msg, ##__VA_ARGS__);}}
	#endif

	////////////////////////////////////////////////////////////////////////////////
	//	try...catch
	////////////////////////////////////////////////////////////////////////////////
	#if defined(_WIN32)
	#define __ENTER_FUNCTION {try{
	#define __LEAVE_FUNCTION }catch(...){AssertSpecial(false,__FUNCTION__);}}
	#endif

	#if defined(_LINUX64)
	#define __ENTER_FUNCTION {try{
	#define __LEAVE_FUNCTION }catch(...){AssertSpecial(false,__PRETTY_FUNCTION__);}}
	#endif

	#define __MYTRY try
	#define __MYCATCH catch(...)

}// namespace sys_util
using namespace sys_util;

#endif
