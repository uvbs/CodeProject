////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_varsystem.cpp
// @author: by Mr.Chen
// @date: 2015/4/23/ 0:25
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_varsystem.h"

void test_varsystem()
{
	CVariableSystem::getSinglePtr()->setIntValue("money", 12345);
	int money = CVariableSystem::getSinglePtr()->getIntValue("money");
	string strKey = "IP";
	string strValue = "192.168.1.189";
	CVariableSystem::getSinglePtr()->setStringValue(strKey, strValue);
	strValue = CVariableSystem::getSinglePtr()->getStringValue(strKey);
}


