////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_designpattern.h
// @author: by Mr.Chen
// @date: 2015/4/23/ 0:05
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "type.h"
#include <iostream>
using std::endl;
using std::cout;
#include "factory.h"
#include "factory_method.h"
#include "factory_abstract.h"
#include "singleton.h"
#include "strategy.h"
#include "observer.h"
#include "tpl_method.h"
#include "work.h"

//单例模式测试类
class TestSingleton : public Singleton<TestSingleton>
{
public:
	void test() 
	{
		cout << "test singleton and MLock." << endl;
	}
};

void test_pattern();
