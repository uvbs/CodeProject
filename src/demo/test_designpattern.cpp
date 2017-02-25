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
	//����ģʽ
	TestSingleton::getSinglePtr()->test();
	//����ģʽ
	strategy_pattern::test();
	//����ģʽ
	factory_pattern::test();
	factory_method_pattern::test();
	factory_abstract_pattern::test();
	//�۲���ģʽ
	observer_pattern::test();
	//ģ�巽��ģʽ
	tpl_method_pattern::test();
	//״̬ģʽ
	state_pattern::test();
}
