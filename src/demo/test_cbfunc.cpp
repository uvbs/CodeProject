////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_cbfunc.cpp
// @author: by Mr.Chen
// @date: 2015/4/21/ 23:51
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_cbfunc.h"

void test_cbfunc()
{
	//test type
	uint64 u64;
	uint32 u32;
	uint16 u16;
	uint8 u8;

	int64 i64;
	int32 i32;
	int16 i16;
	int8 i8;

	////注册事件对应的回调函数
	//TestCbfunc* pObj = new TestCbfunc;
	////REGISTER_FUNC(1, TestCbfunc::cb1, pObj);
 //   //REGISTER_OBJFUNC(2, TestCbfunc::cb2, pObj, pObj);
 //   pObj->registerEvent(1, &TestCbfunc::cb1);
 //   pObj->registerEvent(2, &TestCbfunc::cb2, pObj);
	//pObj->fireEvent(1);
	//pObj->fireEvent(2);

    TestCbfunc2* pObj2 = new TestCbfunc2;
    CbfuncHelper cb;
	int eventId1 = 1, eventId2 = 2;
    cb.registerEvent(eventId1, (void*)&TestCbfunc2::cb1);
    cb.registerEvent(eventId2, &TestCbfunc2::cb2, pObj2);
    cb.fireEvent(eventId1);
    cb.fireEvent(eventId2);
}

