////////////////////////////////////////////////////////////////////////////////////
// 
// @file: helper.h
// @author: by Mr.Chen
// @date: 2014/8/21	14:27
// @brief: 其他常用帮助函数
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __HELP_H_
#define __HELP_H_

#include "type.h"
#include <iostream>
using std::cout;
using std::endl;

namespace sys_util {

	////////////////////////////////////////////////////////////////////////////////
	//	thread sleep
	////////////////////////////////////////////////////////////////////////////////
	void			sleep(uint millionseconds);
	ThreadId	getCurrTid();

	template <class T>
	void output(T *array, int length)
	{
		for (int i = 0; i < length; ++i) {
			std::cout << array[i] << " ";
		}
		cout << endl;
	}

} //namespace sys_util
using namespace sys_util;


#endif	// __HELP_H_

