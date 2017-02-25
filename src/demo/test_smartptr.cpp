////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_smartptr.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 1:27
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_smartptr.h"
#include "assert.h"
#include <stdio.h>

void test_shareptr()
{
	SharedPtr<int> sp(new int(10));                //一个指向整数的shared_ptr    
	Assert(sp.unique());                            //现在shared_ptr是指针的唯一持有者     
	SharedPtr<int> sp2 = sp;                       //第二个shared_ptr,拷贝构造函数     
	Assert(sp == sp2 && sp.useCount() == 2);       //两个shared_ptr相等,指向同一个对象,引用计数为2    
	*sp2 = 100;                                     //使用解引用操作符修改被指对象    
	Assert(*sp == 100);                             //另一个shared_ptr也同时被修改     
}

namespace sys_util
{
	template<>
	void SharedPtr<FILE>::destroy()
	{
		if (_pRep)
		{
			fclose(_pRep);
		}
	}
}

bool test_shareptr2()
{
    FILE* fp = ::fopen("./data/sharefileptr.txt", "w+");
    if (!fp) 
	{
		return false;
	}
    SharedPtr<FILE> obj(fp);
	return false;
}


void test_safeptr()
{
    //alloc integer array with 100 elements
    SafePtr<int> arr(10);
    arr[0] = arr[1] = 100;
    for (int i = 0; i < 10; i++)
    {
        if (i % 5 == 0 ) printf("\n");
        printf("%d ", arr[i]);
    }
    printf("\n");

    //SafePtr<int> arr2 = arr; //error
    SafePtr<int> arr3;
    //arr3 = arr;
}
