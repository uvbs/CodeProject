////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_datastruct.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 1:18
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "test_datastruct.h"
#include <iostream>
using std::cout;
using std::endl;

/*********************************************/
//test OderedLinearTable
void test_orderedLinearTable()
{
	OderedLinearTable orderObj;
	OderedLinearTable::SqList sl;
	orderObj.InitList(sl);
	for(int i=1;i<=10;i++)
	{
		orderObj.InsertList(sl,i,i);
	}
	orderObj.PutList(sl);
	int e;
	orderObj.DeleteList(sl,1,e);
	cout<<"第1个位置上的数:"<<e<<"已被删除"<<endl;
	orderObj.PutList(sl);
	int position=orderObj.LocateList(sl,5);
	if(position)
	{
		cout<<"查找的元素在第"<<position<<"个位置"<<endl;
	}
	else
	{
		cout<<"查找的元素不存在"<<endl;
	}
	OderedLinearTable::SqList list1,list2,list3;
	orderObj.InitList(list1);
	orderObj.InitList(list2);
	for(int i=1;i<=10;i++)
	{
		orderObj.InsertList(list1,i,i);
		orderObj.InsertList(list2,i,10+i);
	}
	orderObj.PutList(list1);
	orderObj.PutList(list2);
	//合并
	orderObj.MergeList(list1,list2,list3);
	orderObj.PutList(list3);
}
/*********************************************/
//test LinkedLinearTable
void test_linkedLinearTable()
{
	LinkedLinearTable linkObj;
	//LinkedLinearTable::LNode *list1;
	//linkObj.InitListFromEnd(list1,5);
	//linkObj.PutList(list1);

	//LinkedLinearTable::LNode *list2;
	//linkObj.InitListFromEnd(list2,5);
	//linkObj.PutList(list2);

	//int elem;
	//linkObj.GetListElem(list2,2,elem);
	//cout<<elem;

	//linkObj.InsertList(list2,4,100);
	//linkObj.PutList(list2);
	//int e;
	//if(linkObj.DeleteList(list2,4,e))
	//cout<<"元素"<<e<<"已被删除"<<endl;
	//else
	//cout<<"该元素不存在"<<endl;
	//linkObj.PutList(list2);

	//LinkedLinearTable::LNode *list3;
	//linkObj.MergeList(list1,list2,list3);
	//linkObj.PutList(list3);
	//list3 = linkObj.Reverse(list3);
	//linkObj.PutList(list3);

	LinkedLinearTable::LNode *list4;
	linkObj.InitListFromEnd(list4,5);
	//list4 = linkObj.DeleteSame(list4);
	linkObj.Delete(list4,2);
	linkObj.PutList(list4);
}
/*********************************************/
//test DoubleLinkedCircularTable
void test_doubleLinkedCircularTable()
{
	/*DoubleLinkedCircularTable dlinkObj;
	DoubleLinkedCircularTable::DLnode *head;
	printf("使用尾插法建立\n");
	head=dlinkObj.CreateFromEnd(-1);
	dlinkObj.PutList(head);
	printf("\n");*/

	/*
	head=Insert(head,5);
	Output(head);
	printf("\n");
	head=Delete(head,2);
	Output(head);
	printf("\n");*/

	/*head=dlinkObj.Move2(head);
	dlinkObj.PutList(head);
	printf("\n");*/

	FileTest obj;
	obj.test();
}
/*********************************************/
//test statck
void test_stack()
{
    //ComputeExpression();
    //Conversion(100,2);
    //Conversion(100,16);
    //Match();

    /*
    T=10
    各件体积为:{1,8,4,3,5,2}
    4组解:(1,4,3,2)
    (1,4,5)
    (8,2)
    (3,5,2)
    */
    int w[] = {1, 8, 4, 3, 5, 2};
    knapsack(w, 10, sizeof(w) / sizeof(w[0]));
    cout << endl;
    knapsack_v2(w, 10, sizeof(w) / sizeof(w[0]));
    cout << endl;

    //akm
    cout << "akm(3, 5) = " << akm(3, 5);
    cout << endl;
    cout << "akm(3, 5) = " << akm_v2(3, 5);
    cout << endl;

    //ackerman
    cout << "ackerman(3, 5, 3) = " << ackerman(3, 5, 3);
    cout << endl;
    cout << "ackerman(3, 5, 3) = " << ackerman_v2(3, 5, 3);
    cout << endl;

    //
    int n = 4;
    int *arr = new int[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = i + 1;
    }
    //permute(arr, 0, n);
    permute_v2(arr, n);
}
/*********************************************/
//test LinkedQueue
void test_linkedQueue()
{
	LinkedQueue<int> q;
	q.init();
	for (int i=0;i<5;++i)
	{
		q.enQueue(i+1);
	}
	q.output();
	int e;
	for (int i=0;i<4;++i)
	{
		q.deQueue(e);
		cout<<"队首元素: "<<e<<"被删除"<<endl;
		q.output();
	}
}
/*********************************************/
//test OrderedQueue
void test_orderedQueue()
{
	OrderedQueue<int> q;
	for (int i=0;i<5;++i)
	{
		q.enQueue(i+1);
	}
	q.output();
	int e;
	for (int i=0;i<4;++i)
	{
		q.deQueue(e);
		cout<<"队首元素: "<<e<<"被删除"<<endl;
		q.output();
	}
}
/*********************************************/
//test String
#ifndef CLK_TCK
#define CLK_TCK 1000
#endif
void test_string()
{
	String s("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");
	cout<<"s= "<<s<<endl;
	String t("00000001");
	cout<<"t= "<<t<<endl;

	double begin,end;
	int index;
	begin=(double)clock()/CLK_TCK;
	for(int i=0;i<10000;i++)
	{
		index = s.find(t,0);
	}
	cout<<index<<endl;
	end=(double)clock()/CLK_TCK;
	cout<<"Elapsed time: "<<(end-begin)<<"秒\n";
	///////////////////////////////////////////////
	begin=(double)clock()/CLK_TCK;
	for(int i=0;i<10000;i++)
	{
		index=s.find_kmp(t,0);
	}
	cout<<index<<endl;
	end=(double)clock()/CLK_TCK;
	cout<<"Elapsed time: "<<(end-begin)<<"秒\n";
}
/*********************************************/
//test array
#include "array.h"
void test_array()
{
	/////////////////////////////////////
	Array a1(1,10);
	int elem = 0;
	for(int i=0;i<10;i++)
	{
		a1.get(&elem,i);
		cout<<elem<<"\t";
	}
	cout<<endl;

	/////////////////////////////////////
	Array a2(2,3,4);
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
		{
			a2.get(&elem,i,j);
			cout<<elem<<"\t";
		}
		cout<<endl;
	}

	/////////////////////////////////////
	Array a3(3,2,2,2);
	for(int i=0;i<2;i++)
	{
		for(int j=0;j<2;j++)
			for(int k=0;k<2;k++)
			{
				a3.get(&elem,i,j,k);
				cout<<elem<<"\t";
			}
			cout<<endl;
	}

	/////////////////////////////////////
	Array a4(4,3,2,2,2);
	for(int i=0;i<3;i++)
		for(int j=0;j<2;j++)
			for(int k=0;k<2;k++)
				for(int m=0;m<2;m++)
				{
					a4.set(100,i,j,k,m);
				}
				cout<<endl;
				for(int i=0;i<3;i++)
				{
					for(int j=0;j<2;j++)
						for(int k=0;k<2;k++)
							for(int m=0;m<2;m++)
							{
								a4.get(&elem,i,j,k,m);
								cout<<elem<<"\t";
							}
							cout<<endl;
				}
}
/*********************************************/
//app30: test sMatrix
void test_matrix()
{
	SparseMatrix m1;
	SparseMatrix t1,t2;
	m1.mu=6;
	m1.nu=7;
	m1.tu=8;

	m1.data[1].i=1;
	m1.data[1].j=2;
	m1.data[1].e=12;

	m1.data[2].i=1;
	m1.data[2].j=3;
	m1.data[2].e=9;

	m1.data[3].i=3;
	m1.data[3].j=1;
	m1.data[3].e=-3;

	m1.data[4].i=3;
	m1.data[4].j=6;
	m1.data[4].e=14;

	m1.data[5].i=4;
	m1.data[5].j=3;
	m1.data[5].e=24;

	m1.data[6].i=5;
	m1.data[6].j=2;
	m1.data[6].e=18;

	m1.data[7].i=6;
	m1.data[7].j=1;
	m1.data[7].e=15;

	m1.data[8].i=6;
	m1.data[8].j=4;
	m1.data[8].e=-7;
	m1.print();
	double clk1,clk2;
	clk1=(double)clock()/CLK_TCK;
	if(m1.transpose(t1))
	{
		clk2=(double)clock()/CLK_TCK;
		cout<<"Elapsed time: "<<(clk2-clk1)<<"秒\n";
		t1.print();
	}
	clk1=(double)clock()/CLK_TCK;
	if(m1.transpose_fast(t2))
	{
		clk2=(double)clock()/CLK_TCK;
		cout<<"Elapsed time: "<<(clk2-clk1)<<"秒\n";
		t2.print();
	}

	SparseMatrix m2;
	SparseMatrix n2;
	m2.mu=3;
	m2.nu=4;
	m2.tu=4;

	m2.data[1].i=1;
	m2.data[1].j=1;
	m2.data[1].e=3;

	m2.data[2].i=1;
	m2.data[2].j=4;
	m2.data[2].e=5;

	m2.data[3].i=2;
	m2.data[3].j=2;
	m2.data[3].e=-1;

	m2.data[4].i=3;
	m2.data[4].j=1;
	m2.data[4].e=2;

	m2.rpos[1]=1;
	m2.rpos[2]=3;
	m2.rpos[3]=4;

	n2.mu=4;
	n2.nu=2;
	n2.tu=4;

	n2.data[1].i=1;
	n2.data[1].j=2;
	n2.data[1].e=2;

	n2.data[2].i=2;
	n2.data[2].j=1;
	n2.data[2].e=1;

	n2.data[3].i=3;
	n2.data[3].j=1;
	n2.data[3].e=-2;

	n2.data[4].i=3;
	n2.data[4].j=2;
	n2.data[4].e=4;

	n2.rpos[1]=1;
	n2.rpos[2]=2;
	n2.rpos[3]=3;
	n2.rpos[4]=5;

	m2.print();
	n2.print();
	SparseMatrix t3;
	m2.multiple(n2,t3);
	t3.print();
}
/*********************************************/

void test_datastruct()
{
	test_orderedLinearTable();
	//test_linkedLinearTable();
	//test_doubleLinkedCircularTable();
	test_stack();
	test_linkedQueue();
	test_orderedQueue();
	test_string();
	test_array();
	test_matrix();
}

