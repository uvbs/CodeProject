////////////////////////////////////////////////////////////////////////////////////
// 
// @file: factoryAbstract.h
// @author: by Mr.Chen
// @date: 2014/8/14	15:57
// @brief: ���󹤳�ģʽ��ʹ�÷������&�򵥹���ģʽ���Ľ����󹤳�ģʽ��
//		����������ģʽ�в�ֹһ���Ʒʱ����Operation��һ���Ʒ�����ͱ���˳��󹤳�ģʽ
//		���磬�����������
//		��ʱ��IFactory�ӿ��д������в�Ʒ��User��Department���ĳ��󷽷���
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef _FACTORY_ABSTRACT_H
#define _FACTORY_ABSTRACT_H

#include "tNode.h"
#include "tKernel.h"
#include "tClass.h"
#include "string_util.h"
#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;

namespace factory_abstract_pattern {

// User��ṹ
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

// Department��ṹ
class Department
{
public:
	int getID() { return _id; }
	void setID(int id) { _id = id; }
private:
	int _id;
};

// User������ࣨ����tNodeʵ��C++���䣩
class IUser : public tNode
{
public:
	virtual void insert(User* user) = 0;
	virtual User* getUser(int id) = 0;
	WX_DECLARE_DYNAMIC(IUser);
};
//WX_IMPLEMENT_DYNAMIC_VIRTUAL(IUser, GETCLASS(tNode));

// Department������ࣨ����tNodeʵ��C++���䣩
class IDepartment : public tNode
{
public:
	virtual void insert(Department* department) = 0;
	virtual Department* getDepartment(int id) = 0;
	WX_DECLARE_DYNAMIC(IDepartment);
};
//WX_IMPLEMENT_DYNAMIC_VIRTUAL(IDepartment, GETCLASS(tNode));

// SqlServer��User��
class SqlServerUser : public IUser
{
public:
	void insert(User* user)
	{
		cout << "��SqlServer�и�User��������һ����¼" <<endl;
	}
	User* getUser(int id)
	{
		cout << "��SqlServer�и���ID�õ�User���е�һ����¼" <<endl;
		return NULL;
	}
	WX_DECLARE_DYNAMIC(SqlServerUser);
};
//WX_IMPLEMENT_DYNAMIC(SqlServerUser, GETCLASS(IUser));

// SqlServer��Department��
class SqlServerDepartment : public IDepartment
{
public:
	void insert(Department* department)
	{
		cout << "��SqlServer�и�Department��������һ����¼" <<endl;
	}
	Department* getDepartment(int id)
	{
		cout << "��SqlServer�и���ID�õ�Department���е�һ����¼" <<endl;
		return NULL;
	}
	WX_DECLARE_DYNAMIC(SqlServerDepartment);
};
//WX_IMPLEMENT_DYNAMIC(SqlServerDepartment, GETCLASS(IDepartment));

// Access��User��
class AccessUser : public IUser
{
public:
	void insert(User* user)
	{
		cout << "��Access�и�User��������һ����¼" <<endl;
	}
	User* getUser(int id)
	{
		cout << "��Access�и���ID�õ�User���е�һ����¼" <<endl;
		return NULL;
	}
	WX_DECLARE_DYNAMIC(AccessUser);
};
//WX_IMPLEMENT_DYNAMIC(AccessUser, GETCLASS(IUser));

// Access��Department��
class AccessDepartment : public IDepartment
{
public:
	void insert(Department* department)
	{
		cout << "��Access�и�Department��������һ����¼" <<endl;
	}
	Department* getDepartment(int id)
	{
		cout << "��Access�и���ID�õ�Department���е�һ����¼" <<endl;
		return NULL;
	}
	WX_DECLARE_DYNAMIC(AccessDepartment);
};
//WX_IMPLEMENT_DYNAMIC(AccessDepartment, GETCLASS(IDepartment));

// ʹ�÷���ļ򵥹���ģʽ
class FactoryUseReflect
{
public:
	static IUser* createUser();
	static IDepartment* createDepartment();

protected:
	static int getFactoryID() 
	{ 
		static int s_lastId = 0; 
		return ++s_lastId; 
	}

private:
	static std::string _s_dbNamePrefix;
};

void test();

} //namespace factory_abstract_pattern
using namespace factory_abstract_pattern;

#endif // _FACTORY_ABSTRACT_H

