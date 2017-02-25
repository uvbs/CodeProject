/********************************************************************
	CREATED:	2012/01/31
	FILENAME:	tpl_method.h
	AUTHOR:	By Mr.Chen
	PURPOSE:	模板方法模式
		定义一个操作中的算法骨架，而将一些步骤延迟到子类中。
		使得子类可以不改变一个算法的结构，即可重定义该算法的某些特定步骤。

*********************************************************************/
#ifndef _TPL_METHOD_H
#define _TPL_METHOD_H

#include "type.h"
#include <iostream>
using std::cout;
using std::endl;
using std::cin;

namespace tpl_method_pattern
{
/**
	抽象类，其实也是一个抽象模板，定义并实现了一个模板的方法。
	这个模板方法一般是一个具体方法，他给出了一个顶级逻辑的骨架，
	而逻辑的组成步骤在相应的抽象操作中，推迟到子类中实现。
*/
class AbstractClass 
{
public:
	void templateMethod()
	{
		primitiveMethod1();
		primitiveMethod2();
	}

private:
	virtual void primitiveMethod1() = 0;
	virtual void primitiveMethod2() = 0;
};


/**
	每一个ConcreteClass都可以给出这些抽象方法的不同实现，
	从而使得顶级逻辑的实现各不相同。
*/
class ConcreteClass1 : public AbstractClass
{
private:
	virtual void primitiveMethod1()
	{
		cout << "in ConcreteClass1::primitiveMethod1() " << endl;
	}

	virtual void primitiveMethod2()
	{
		cout << "in ConcreteClass1::primitiveMethod2() " << endl;
	}
};

class ConcreteClass2 : public AbstractClass
{
private:
	virtual void primitiveMethod1()
	{
		cout << "in ConcreteClass2::primitiveMethod1() " <<endl;
	}

	virtual void primitiveMethod2()
	{
		cout << "in ConcreteClass2::primitiveMethod2() " <<endl;
	}
};

void test();

} // namespace tpl_method_pattern

#endif // _TPL_METHOD_H
