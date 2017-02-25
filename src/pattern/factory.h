////////////////////////////////////////////////////////////////////////////////////
// 
// @file: factory.h
// @author: by Mr.Chen
// @date: 2015/4/23/ 22:03
// @brief: �򵥹���
//	1. ʹ�����װ
//	2. ����չ��ʹ�ü̳����̬��
//	3. ʹ��һ������������������ʵ���Ĺ��̣�����ǹ�����	
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef _FACTORY_H
#define _FACTORY_H

#include "type.h"

namespace factory_pattern {

// �������
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

// �ӷ�������
class OperationAdd : public Operation
{
public:
	double getResult()
	{
		return _memberA + _memberB;
	}
};

// ����������
class OperationSub : public Operation
{
	double getResult()
	{
		return _memberA - _memberB;
	}
};

// �˷�������
class OperationMul : public Operation
{
	double getResult()
	{
		return _memberA * _memberB;
	}
};

// ����������
class OperationDiv : public Operation
{
	double getResult()
	{
		double result = 0.0;
		if (_memberB == 0)
		{
			throw;// new exception("��������Ϊ0");
		}
		result = _memberA / _memberB;
		return result;
	}
};

// �����๤��
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

