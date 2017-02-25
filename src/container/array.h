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

//Nά����
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
	int *m_base; //����Ԫ�ػ�ַ
	int m_dim; //����ά��
	int *m_bounds; //����ά���ַ
	int *m_constants; //����ӳ����������ַ
};

#endif

