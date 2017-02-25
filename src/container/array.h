////////////////////////////////////////////////////////////////////////////////////
// 
// @file: 5-array.h
// @author: by Mr.Chen
// @date: 2014/10/21	16:50
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __ARRAY_H_
#define  __ARRAY_H_

#include <stdarg.h>

//N维数组
class Array
{
public:
	enum
	{
		MAX_ARRAY_DIM=8,
	};

public:
	Array(int dim,...);
	~Array();
	
	int locate(va_list ap);
	bool get(int* e,...);
	bool set(int e,...);

private:
	int *m_base; //数组元素基址
	int m_dim; //数组维数
	int *m_bounds; //数组维界基址
	int *m_constants; //数组映像函数常量基址
};

#endif

