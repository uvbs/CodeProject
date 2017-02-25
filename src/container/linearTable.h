////////////////////////////////////////////////////////////////////////////////////
// 
// @file: 1-linearTable.h
// @author: by Mr.Chen
// @date: 2014/9/14/ 9:34
// @brief: ���Ա�˳��+��ʽ��
//			   ˫ѭ��������ʽ��
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __LINEAR_TABLE_H_
#define __LINEAR_TABLE_H_

#include <malloc.h>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

//����Ԫ��
bool Comp(int a,int b);
//���庯��ָ������
typedef bool (*pFunction)(int,int);

//���Ա��˳��ʵ��
class OderedLinearTable
{
public:
	static const int LIST_INIT_SIZE=100;
	static const int LISTINCREMENT=10;

	typedef struct //˳���ṹ
	{
		int *elem;
		int length;//��ǰ����
		int listsize;//��ǰ����Ĵ洢����
	}SqList;

	bool InitList(SqList &list);//��ʼ��˳���
	void PutList(SqList &list);//���˳���
	bool InsertList(SqList &list,int i,int e);//����Ԫ��
	bool DeleteList(SqList &list,int i,int &e);//ɾ��Ԫ��
	int LocateList(SqList &list,int e,pFunction pFun=Comp);//����Ԫ��
	void MergeList(SqList list1,SqList list2,SqList &listResult);//˳���ĺϲ�
};


//���Ա����ʽʵ��(����ͷ�ڵ�)
class LinkedLinearTable
{
public:
	typedef struct LNode
	{
		int data;
		struct LNode *next;
	}LNode;

	void InitListFromBegin(LNode * &list,int n);//ǰ�巨
	void InitListFromEnd(LNode *&list,int n);//��巨
	void PutList(LNode *list);
	bool GetListElem(LNode *list,int i,int &e);//�õ���i��λ���ϵ�Ԫ��
	bool InsertList(LNode * list,int i,int e);//�ڵ�i��λ��ǰ�߲���Ԫ��e
	bool DeleteList(LNode * list,int i,int &e);//ɾ����i��Ԫ��
	void MergeList(LNode *&listA,LNode *&listB,LNode *&listC);//�ϲ�
	LNode * Reverse(LNode *list); //����
	LNode * DeleteSame(LNode *h); //���ظ�ֵɾȥ
	void Delete(LNode *h,int x);
};

//˫��ѭ���������ʽʵ��(����ͷ�ڵ�)
class DoubleLinkedCircularTable
{
public:
	typedef struct node
	{
		int data;
		struct node *prior;
		struct node *next;
	}DLnode;

	//��ǰ�巨��������ͷ�ڵ��˫��ѭ������
	DLnode* CreateFromBegin(int tag);
	//�ú�巨��������ͷ�ڵ��˫��ѭ������
	DLnode* CreateFromEnd(int tag);
	//����ֵΪx�Ľڵ�
	DLnode* Insert(DLnode *h,int x);
	//ɾ������ֵΪx�Ľڵ�
	DLnode* Delete(DLnode *h,int x);
	//�Ѹ����ڵ�ŵ������ڵ�ǰ��
	DLnode* Move(DLnode *h);
	DLnode* Move2(DLnode *h);
	//��ӡ
	void PutList(DLnode *h);

};

//���Ա������
class FileTest
{
public:
	struct Line {
		char text[81];
		int num;                /*�к�*/
		struct Line *next;      /*ָ����һ��������Ŀ��ָ�� */
		struct Line *prior;     /*ָ��ǰһ����Ŀ��ָ�� */
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
	struct Line* m_start;      /*ָ����е�һ����Ŀ��ָ�� */
	struct Line* m_last;       /*ָ���������һ����Ŀ��ָ�� */
};

#endif
