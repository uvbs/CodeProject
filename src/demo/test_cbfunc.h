////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_cbfunc.h
// @author: by Mr.Chen
// @date: 2015/4/21/ 23:49
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef _TEST_CBFUNC_H
#define _TEST_CBFUNC_H

#include "type.h"
#include "cbfunc.h"
#include <iostream>
using std::cout;
using std::endl;

class TestCbfunc : public CbfuncHelper
{
public:
	static bool cb1(const EventArgs&)
	{
		cout<<"in function cb1."<<endl;
		return true;
	}
	bool cb2(const EventArgs&)
	{
		cout<<"in function cb2."<<endl;
		return true;
	}
};


class TestCbfunc2
{
public:
    static bool cb1(const EventArgs&)
    {
        cout<<"in function cb1."<<endl;
        return true;
    }
    bool cb2(const EventArgs&)
    {
        cout<<"in function cb2."<<endl;
        return true;
    }
};

void test_cbfunc();

#endif  //_TEST_CBFUNC_H

