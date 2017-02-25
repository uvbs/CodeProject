////////////////////////////////////////////////////////////////////////////////////
// 
// @file: factoryMethod.h
// @author: by Mr.Chen
// @date: 2014/8/14	15:28
// @brief: 工厂方法模式（扩展开放-修改封闭）
//	注意它与简单工厂模式的区别：
//	1、(简单)工厂模式中只有一个工厂，去构造出加减乘除等运算类实例；
//	2、工厂方法模式中有多个工厂，分别有加减乘除等工厂，再用各自的工厂
//		去构造出各自的运算类实例；
//	3、可以结合工厂管理器使用。
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef _FACTORY_METHOD_H
#define _FACTORY_METHOD_H

#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

namespace factory_method_pattern {

////////////////////////////////////////////
/// 运算基类
////////////////////////////////////////////
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

////////////////////////////////////////////
/// 工厂虚基类
////////////////////////////////////////////
class IFactory
{
public:
	virtual Operation* createOperate() = 0;
	virtual int getID() = 0;
};

enum
{
	OPER_INVALID = -1,
	OPER_ADD,
	OPER_SUB,
	OPER_MUL,
	OPER_DIV,
	OPER_NUM,
};

// 加法工厂
class AddFactory : public IFactory
{
public:
	Operation* createOperate()
	{
		return new OperationAdd();
	}
	int getID()
	{
		return OPER_ADD;
	}
};

// 减法工厂
class SubFactory : public IFactory
{
public:
	Operation* createOperate()
	{
		return new OperationSub();
	}
	int getID()
	{
		return OPER_SUB;
	}
};

// 乘法工厂
class MulFactory : public IFactory
{
public:
	Operation* createOperate()
	{
		return  new OperationMul();
	}
	int getID()
	{
		return OPER_MUL;
	}
};

// 除法工厂
class DivFactory : public IFactory
{
	Operation* createOperate()
	{
		return new OperationDiv();
	}
	int getID()
	{
		return OPER_DIV;
	}
};

// 工厂管理器
class FactoryMgr
{
public:
	FactoryMgr();
	~FactoryMgr();
	void addFactory(IFactory* pFactory);
	Operation* createOperate(int id);

private:
	void _registerFactory();

private:
	IFactory** pFactoryArry;
};


///////////////////////////////////////////
///当工厂方法模式中不止一类产品时候（如Operation是一类产品）
///则就变成了抽象工厂模式
///////////////////////////////////////////
// User数据表结构
class User
{
public:
	int getID() { return _id; }
	void setID(int id) { _id = id; }
	string getName() { return _name; }
	void setName(string name) { _name = name; }
private:
	int _id;
	string _name;
};
// Department数据表结构
class Department
{
public:
	int getID() { return _id; }
	void setID(int id) { _id = id; }
private:
	int _id;
};

// DB-User表虚基类
class IUser
{
public:
	virtual void insert(User* user) = 0;
	virtual User* getUser(int id) = 0;
};
// DB-Department表虚基类
class IDepartment
{
public:
	virtual void insert(Department* department) = 0;
	virtual Department* getDepartment(int id) = 0;
};
// SqlServer的User表
class SqlServerUser : public IUser
{
public:
	void insert(User* user)
	{
		cout << "在SqlServer中给User表中增加一条记录" <<endl;
	}
	User* getUser(int id)
	{
		cout << "在SqlServer中根据ID得到User表中的一条记录" <<endl;
		return NULL;
	}
};
// SqlServer的Department表
class SqlServerDepartment : public IDepartment
{
public:
	void insert(Department* department)
	{
		cout << "在SqlServer中给Department表中增加一条记录" <<endl;
	}
	Department* getDepartment(int id)
	{
		cout << "在SqlServer中根据ID得到Department表中的一条记录" <<endl;
		return NULL;
	}
};
// Access的User表
class AccessUser : public IUser
{
public:
	void insert(User* user)
	{
		cout << "在Access中给User表中增加一条记录" <<endl;
	}
	User* getUser(int id)
	{
		cout << "在Access中根据ID得到User表中的一条记录" <<endl;
		return NULL;
	}
};
// Access的User表
class AccessDepartment : public IDepartment
{
public:
	void insert(Department* department)
	{
		cout << "在Access中给Department表中增加一条记录" <<endl;
	}
	Department* getDepartment(int id)
	{
		cout << "在Access中根据ID得到Department表中的一条记录" <<endl;
		return NULL;
	}
};

// 工厂虚基类
class IDBFactory
{
public:
	virtual IUser* createUser() = 0;
	virtual IDepartment* createDepartment() = 0;
};
// 
class SqlServerFactory : public IDBFactory
{
public:
	IUser* createUser()
	{
		return new SqlServerUser();
	}
	IDepartment* createDepartment()
	{
		return new SqlServerDepartment();
	}
};
//
class AccessFactory : public IDBFactory
{
public:
	IUser* createUser()
	{
		return new AccessUser();
	}
	IDepartment* createDepartment()
	{
		return new AccessDepartment();
	}
};

void test();

} //namespace factory_method_pattern
using namespace factory_method_pattern;

#endif // _FACTORY_METHOD_H
