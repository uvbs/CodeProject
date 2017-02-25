/********************************************************************
	CREATED:	2012/01/31
	FILENAME:	tpl_method.h
	AUTHOR:	By Mr.Chen
	PURPOSE:	ģ�巽��ģʽ
		����һ�������е��㷨�Ǽܣ�����һЩ�����ӳٵ������С�
		ʹ��������Բ��ı�һ���㷨�Ľṹ�������ض�����㷨��ĳЩ�ض����衣

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
	�����࣬��ʵҲ��һ������ģ�壬���岢ʵ����һ��ģ��ķ�����
	���ģ�巽��һ����һ�����巽������������һ�������߼��ĹǼܣ�
	���߼�����ɲ�������Ӧ�ĳ�������У��Ƴٵ�������ʵ�֡�
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
	ÿһ��ConcreteClass�����Ը�����Щ���󷽷��Ĳ�ͬʵ�֣�
	�Ӷ�ʹ�ö����߼���ʵ�ָ�����ͬ��
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
