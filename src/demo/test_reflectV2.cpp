////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_reflectV2.cpp
// @author: by Mr.Chen
// @date: 2015/7/15	15:22
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_reflectV2.h"

using std::cout;
using std::endl;

REFLECT_IMPLEMENT(CTest);
PROPERTY_IMPLEMENT(CTest, int, _id);
PROPERTY_IMPLEMENT(CTest, char*, _name);

// 实现
void CTest::set__id(int value)
{
    _id=value;
}
int CTest::get__id()
{
    return _id;
}
void CTest::set__name(char* value)
{
    strcpy(_name,value);
}
char* CTest::get__name()
{
    return _name;
}

// -----------------------------------------------------------------------------------------------------

void test_reflectV2()
{
// -----------------------------------------------------------------------------------------------------
    CTest t;
    CPropertyInfo<CTest,int>* ID = findProperty<CTest,int>("_id");
    CPropertyInfo<CTest,char*>* Name = PROPERTY(CTest, _name);
    //修改属性
    SET_PROP(&t,ID,1234);
    setProperty(&t,Name,(char*)"hello"); 
    //读取属性
    cout << getProperty(&t,ID) << endl<<GET_PROP(&t,Name) << endl;
    //枚举所有属性，和属性类型
    CPropertyBase* Item = NULL;
    for (Item = CTest::lpFirstProperty; Item; Item = Item->lpNext)
    {
        cout << Item->szType << " " << Item->szName << endl;
    }

// -----------------------------------------------------------------------------------------------------
	GameObject gameObject;
	gameObject.set("_itest", 1);
	gameObject.set("_ftest", 2.f);
	gameObject.set("_strtest", "hello world.");
	gameObject.set("_btest", false);

	cout << gameObject.getInt("_itest") << endl;
	cout << gameObject.getFloat("_ftest") << endl;
	cout << gameObject.getString("_strtest") << endl;
	cout << gameObject.getBool("_btest") << endl;

}
