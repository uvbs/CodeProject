////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_designpattern.cpp
// @author: by Mr.Chen
// @date: 2015/4/23/ 0:08
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_designpattern.h"

//test singleton and MLock
WX_IMPLEMENT_SINGLEON(TestSingleton)

void test_pattern()
{
	//单例模式
	TestSingleton::getSinglePtr()->test();
	//策略模式
	strategy_pattern::test();
	//工厂模式
	factory_pattern::test();
	factory_method_pattern::test();
	factory_abstract_pattern::test();
	//观察者模式
	observer_pattern::test();
	//模板方法模式
	tpl_method_pattern::test();
	//状态模式
	state_pattern::test();
}
