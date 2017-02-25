////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_bitset.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 0:51
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_bitset.h"
#include <iostream>
using std::cout;
using std::endl;

void test_bitset()
{
	BitSet<8> bitset;
	bitset.setBit(0);
	for (int i=0;i<8;++i)
	{
		if (bitset.isSetBit(i))
		{
			cout <<"bit ["<<i<<"] is set"<<endl;
		}
	}
}

void test_bitflags()
{
    BitFlags<uint32> flag(1);
    bool b = flag.isSetBit(0);

    BitFlags<uint64> flag2(2);
    b = flag2.isSetBit(1);
    b = flag2.isSetBit(0);
}
