////////////////////////////////////////////////////////////////////////////////////
// 
// @file: 1-linearTable.h
// @author: by Mr.Chen
// @date: 2014/9/14/ 9:34
// @brief: 线性表（顺序+链式）
//			   双循环链表（链式）
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __LINEAR_TABLE_H_
#define __LINEAR_TABLE_H_

#include <malloc.h>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

//查找元素
bool Comp(int a,int b);
//定义函数指针类型
typedef bool (*pFunction)(int,int);

//线性表的顺序实现
class OderedLinearTable
{
public:
	static const int LIST_INIT_SIZE=100;
	static const int LISTINCREMENT=10;

	typedef struct //顺序表结构
	{
		int *elem;
		int length;//当前长度
		int listsize;//当前分配的存储容量
	}SqList;

	bool InitList(SqList &list);//初始化顺序表
	void PutList(SqList &list);//输出顺序表
	bool InsertList(SqList &list,int i,int e);//插入元素
	bool DeleteList(SqList &list,int i,int &e);//删除元素
	int LocateList(SqList &list,int e,pFunction pFun=Comp);//查找元素
	void MergeList(SqList list1,SqList list2,SqList &listResult);//顺序表的合并
};


//线性表的链式实现(带表头节点)
class LinkedLinearTable
{
public:
	typedef struct LNode
	{
		int data;
		struct LNode *next;
	}LNode;

	void InitListFromBegin(LNode * &list,int n);//前插法
	void InitListFromEnd(LNode *&list,int n);//后插法
	void PutList(LNode *list);
	bool GetListElem(LNode *list,int i,int &e);//得到第i个位置上的元素
	bool InsertList(LNode * list,int i,int e);//在第i个位置前边插入元素e
	bool DeleteList(LNode * list,int i,int &e);//删除第i个元素
	void MergeList(LNode *&listA,LNode *&listB,LNode *&listC);//合并
	LNode * Reverse(LNode *list); //逆置
	LNode * DeleteSame(LNode *h); //把重复值删去
	void Delete(LNode *h,int x);
};

//双向循环链表的链式实现(带表头节点)
class DoubleLinkedCircularTable
{
public:
	typedef struct node
	{
		int data;
		struct node *prior;
		struct node *next;
	}DLnode;

	//用前插法建立带表头节点的双向循环链表
	DLnode* CreateFromBegin(int tag);
	//用后插法建立带表头节点的双向循环链表
	DLnode* CreateFromEnd(int tag);
	//插入值为x的节点
	DLnode* Insert(DLnode *h,int x);
	//删除所有值为x的节点
	DLnode* Delete(DLnode *h,int x);
	//把负数节点放到正数节点前边
	DLnode* Move(DLnode *h);
	DLnode* Move2(DLnode *h);
	//打印
	void PutList(DLnode *h);

};

//线性表的运用
class FileTest
{
public:
	struct Line {
		char text[81];
		int num;                /*行号*/
		struct Line *next;      /*指向下一个输入项目的指针 */
		struct Line *prior;     /*指向前一个项目的指针 */
	};

public:
	FileTest();
	~FileTest();
	struct Line* find(int);
	struct Line* dls_store(struct Line *);
	void patchup(int,int);
	void delete_text();
	void list();
	
	void save(char *);
	void load(char *);
	int menu_select();
	int enter(int linenum);
	int test();

private:
	struct Line* m_start;      /*指向表中第一个项目的指针 */
	struct Line* m_last;       /*指向表中作后一个项目的指针 */
};

#endif
