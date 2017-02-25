////////////////////////////////////////////////////////////////////////////////////
// 
// @file: factory.h
// @author: by Mr.Chen
// @date: 2015/4/23/ 22:03
// @brief: 简单工厂
//	1. 使用类封装
//	2. 易扩展（使用继承与多态）
//	3. 使用一个单独的类来做创造实例的过程（这就是工厂）	
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef _FACTORY_H
#define _FACTORY_H

#include "type.h"

namespace factory_pattern {

// 运算基类
class Operation
{
public:
	virtual double getResult() { return 0; }

public:
	void	setA(double val) { _memberA = val; }
	double	getA() { return _memberA; }
	void	setB(double val) { _memberB = val; }
	double	getB() { return _memberB; }

protected:
	double _memberA;
	double _memberB;
};

// 加法运算类
class OperationAdd : public Operation
{
public:
	double getResult()
	{
		return _memberA + _memberB;
	}
};

// 减法运算类
class OperationSub : public Operation
{
	double getResult()
	{
		return _memberA - _memberB;
	}
};

// 乘法运算类
class OperationMul : public Operation
{
	double getResult()
	{
		return _memberA * _memberB;
	}
};

// 除法运算类
class OperationDiv : public Operation
{
	double getResult()
	{
		double result = 0.0;
		if (_memberB == 0)
		{
			throw;// new exception("除数不能为0");
		}
		result = _memberA / _memberB;
		return result;
	}
};

// 运算类工厂
class OperationFactory
{
public:
	static Operation* createOperate(char oper)
	{
		Operation *pOperObj = NULL;
		switch(oper)
		{
		case '+':
			{
				pOperObj = new OperationAdd();
			}
			break;
		case '-':
			{
				pOperObj = new OperationSub();
			}
			break;
		case '*':
			{
				pOperObj = new OperationMul();
			}
			break;
		case '/':
			{
				pOperObj = new OperationDiv();
			}
			break;
		default:
			return NULL;
		}
		return pOperObj;
	}
};

void test();

} //namespace factory_pattern
using namespace factory_pattern;

#endif // _FACTORY_H

