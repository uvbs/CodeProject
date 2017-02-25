#include "type.h"
#include "socketdef.h"
#include "test_allocator.h"
#include <stdio.h>

void test_allocator()
{
    //test allocator
	//test single_allocator
    single_allocator<int>::getSingleton().init(1024);
	for (int i = 0; i < 1024; ++i)
	{
		int* tmp_obj = single_allocator<int>::getSingleton().alloc();
		if (!tmp_obj)
		{
			printf("pool is empty!\n");
		}
	}
	int* tmp_obj = single_allocator<int>::getSingleton().alloc();
	if (!tmp_obj)
	{
		printf("pool is empty!\n");
	}

    //test aligned_allocator
	int* ptr = new int;
	printf("adress = 0x%X\n", ptr);

    //ÆðÊ¼µØÖ·Îª16×Ö½Ú¶ÔÆë
	aligned_allocator<int, 16> s;
	int* aligned_ptr = s.alloc(10);
	printf("adress = 0x%X\n", aligned_ptr);
	s.free(aligned_ptr);
}

