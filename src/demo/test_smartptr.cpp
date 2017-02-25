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
	SharedPtr<int> sp(new int(10));                //һ��ָ��������shared_ptr    
	Assert(sp.unique());                            //����shared_ptr��ָ���Ψһ������     
	SharedPtr<int> sp2 = sp;                       //�ڶ���shared_ptr,�������캯��     
	Assert(sp == sp2 && sp.useCount() == 2);       //����shared_ptr���,ָ��ͬһ������,���ü���Ϊ2    
	*sp2 = 100;                                     //ʹ�ý����ò������޸ı�ָ����    
	Assert(*sp == 100);                             //��һ��shared_ptrҲͬʱ���޸�     
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
