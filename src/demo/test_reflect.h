////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_reflect.h
// @author: by Mr.Chen
// @date: 2015/4/22/ 0:10
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "type.h"
#include "tKernel.h"
#include "tClass.h"
#include <iostream>
using std::cout;
using std::endl;

// 虚基类(利用tNode实现C++ Reflect)
class Shape : public tNode
{
public:
	virtual void draw() = 0;
	WX_DECLARE_DYNAMIC(Shape);
};

class Rect : public Shape
{
public:
	void draw()
	{
		cout << "draw rect." <<endl;
	}
	WX_DECLARE_DYNAMIC(Rect);
};

class Square : public Rect
{
public:
	void draw()
	{
		cout << "draw Square." <<endl;
	}
	WX_DECLARE_DYNAMIC(Square);
};

void test_reflect();
