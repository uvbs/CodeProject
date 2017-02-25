////////////////////////////////////////////////////////////////////////////////////
// 
// @file: queue.h
// @author: by Mr.Chen
// @date: 2014/9/19	11:08
// @brief: 队列的链式、顺序实现
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __QUEUE_H_
#define __QUEUE_H_

#include <stdio.h>
#include <iostream>
using std::cout;
using std::endl;

namespace container {

/////////////////链式队列的实现//////////////////////

//队列节点
template<class ElemType>
struct QNode
{
	ElemType data;
	struct QNode *next;
};

//链式队列
template<class ElemType>
class LinkedQueue
{
public:
	bool init()
	{
		m_front = m_rear = new QNode<ElemType>;
		if(!m_front)
			return false;
		m_front->next=NULL;
		return true;
	}

	bool destroy()
	{
		while(m_front)
		{
			m_rear = m_front->next;
			delete m_front;
			m_front = m_rear;
		}
		return true;
	}

	bool enQueue(ElemType e)
	{
		QNode<ElemType> *p=new QNode<ElemType>;
		if(!p)
			return false;
		p->data=e;
		p->next=NULL;
		m_rear->next=p;
		m_rear=p;
		return true;
	}

	bool deQueue(ElemType &e)
	{
		if(m_front==m_rear)
			return false;
		QNode<ElemType> *p=m_front->next;
		m_front->next=p->next;
		e=p->data;
		if(m_rear==p)
			m_rear=m_front;
		delete p;
		return true;
	}

	void output()
	{
		QNode<ElemType> *p=m_front->next;
		while(p)
		{
			cout<<p->data<<' ';
			p=p->next;
		}	
		cout<<endl;
	}

private:
	QNode<ElemType>* m_front;
	QNode<ElemType>* m_rear;
};


/////////////////顺序队列的实现//////////////////////
template <class ElemType>
class OrderedQueue
{
public:
	OrderedQueue():MAXSIZE(100)
	{
		base=new ElemType[MAXSIZE];
		if(!base)
			cout<<"分配空间失败"<<endl;
		front=rear=0;
	}

	int length() {return (rear-front+MAXSIZE)%MAXSIZE;}

	bool enQueue(ElemType e)
	{
		if((rear+1)%MAXSIZE==front)
		{
			cout<<"队列已满"<<endl;
			return false;
		}
		base[rear]=e;
		rear=(rear+1)%MAXSIZE;
		cout<<"元素"<<e<<"已入队列"<<endl;
		return true;
	}

	bool deQueue(ElemType& e)
	{
		if(front==rear)
		{
			cout<<"队列已为空"<<endl;
			return false;
		}
		e=base[front];
		front=(front+1)%MAXSIZE;
		cout<<"元素"<<e<<"已出队"<<endl;
		return true;
	}

	void output()
	{
		if(front==rear)
		{
			cout<<"队列已为空"<<endl;
			return;
		}
		for (int i=front;i<rear;++i)
		{
			cout<<base[i]<<' ';
		}
		cout<<endl;
	}

private:
	int front;
	int rear;
	ElemType *base;
	const int MAXSIZE;
};

} //namespace container
using namespace container;

#endif //__QUEUE_H_


