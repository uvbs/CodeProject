////////////////////////////////////////////////////////////////////////////////////
// 
// @file: serializer.h
// @author: by Mr.Chen
// @date: 2014/5/15	15:05
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef _SERIALIZER_H
#define _SERIALIZER_H

#include "type.h"
#include "assert.h"

namespace file_parser {

// 扩展类型
// 整型数组
struct IntArray
{
public:
	int _nCount;
	IntArray() 
	{
		_pArray = NULL; 
		_nCount = 0; 
	}
	~IntArray() {}
	int operator[](const int i) const 
	{ 
		Assert(i < _nCount && i >= 0); 
		return _pArray[i]; 
	}
	int count() const { return _nCount; }
	bool isValid() const
	{
		return (_pArray != NULL);
	}
	int* init(int size)
	{
		if (size > 0)
		{
			_pArray = new int[size];
			_nCount = size;
			return _pArray;
		}
		return NULL;
	}
	bool isContains(const int value) const
	{
		for (int i = 0; i < count(); ++i)
		{
			if (value == _pArray[i])
			{
				return true;
			}
		}
		return false;
	}
private:
	int* _pArray;
};

// 浮点数组
struct FloatArray
{
	float* _pArray;
	int	 _nCount;
	FloatArray() 
	{
		_pArray = NULL; 
		_nCount = 0; 
	}
	~FloatArray() {}
	float operator[](const int i) const 
	{ 
		Assert(i < _nCount && i >= 0); 
		return _pArray[i]; 
	}
};

// 基类
class ISerializer
{
public:
	virtual ISerializer& operator << (int& value)		= 0;
	virtual ISerializer& operator << (uint& value)		= 0;
	virtual ISerializer& operator << (int64& value)	= 0;
	virtual ISerializer& operator << (uint64& value)	= 0;
	virtual ISerializer& operator << (float& value)	    = 0;
	virtual ISerializer& operator << (char*& value)	= 0;
	virtual ISerializer& operator << (IntArray& value) = 0;
	virtual ISerializer& operator << (FloatArray& value)= 0;
	virtual void skipField() = 0;
	virtual void setCheckColumn(bool check) = 0;
};

} // namespace file_parser
using namespace file_parser;

#endif // __SERIALIZER_H_

