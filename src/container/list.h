////////////////////////////////////////////////////////////////////////////////
//	list
//	2010.05.02 caolei
////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef _LIST_H
#define _LIST_H

#include "type.h"
#include "allocator.h"

namespace container {
////////////////////////////////////////////////////////////////////////////////
//	DefaultAllocator
////////////////////////////////////////////////////////////////////////////////
template<typename T>
class DefaultAllocator
{
public:
	static T* alloc()					{ return ::new T(); }
	static void free(T* const pElem)	{ ::delete pElem;}
};

////////////////////////////////////////////////////////////////////////////////
//	class list_node
////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct ListNode
{
public:
	T elem;
	ListNode* next;
	ListNode* prev;
};

////////////////////////////////////////////////////////////////////////////////
//	class list
////////////////////////////////////////////////////////////////////////////////
template<typename T, typename Alloc = DefaultAllocator<ListNode<T> > >
class List
{
public:
	typedef ListNode<T> Node;
	class	Iterator;

public:
	List();
	virtual ~List();
	List(const List& other) { *this = other; };
	//void SetAllocator(IAllocator<Node>& allocator) { m_pAllocator = &allocator; }

	//stl兼容接口
	void		push_back	(const T& elem) 				{ _insert(_pTail, elem); }
	void		push_front	(const T& elem) 				{ _insert(NULL, elem); }
	void		pop_back	()									{ _remove(_pTail); }
	void		pop_front	()									{ _remove(_pHead); }
	void		remove		(const T& elem) 				{ _remove(_find(elem)); }
	Iterator	insert		(Iterator _where, const T& elem)	{ return Iterator(_insert(_where.p, elem)); };
	Iterator	erase		(Iterator _where)					{ return Iterator(_remove(_where.p)); }
	void		clear		();
	bool		empty	() const								{ return _pHead = NULL; }
	int			size		() const								{ return _size; }

	Iterator	begin	() const	{ return Iterator(_pHead);	}
	Iterator	end	() const	{ return Iterator(NULL);	}
	Iterator	rbegin() const	{ return Iterator(_pTail);	}
	Iterator	rend	() const	{ return Iterator(NULL);	}

	T&			front	()				{ return _pHead->elem; }
	const T&	front	() const	{ return _pHead->elem; }
	T&			back	()				{ return _pTail->elem; }
	const T&	back	() const	{ return _pTail->elem; }

	//stl中没有的接口
	void		insert_before	(Iterator _where, const T& elem);
	T&			push_back_fast()				{ Node* p = _insert(_pTail); return p->elem; }
	Iterator	find(const T& elem)			{ return Iterator(_find(elem)); };
	Iterator	find_last(const T& elem)	{ return Iterator(_findLast(elem)); }

	//List层面的深拷贝，list_node中的list_node::elem内如果包含指针，不在该拷贝过程中。因此对于list_node::elem是浅拷贝
	List& operator = (const List& other);

public:
	class Iterator
	{
	public:
		Node* p;
		
		Iterator() { p = NULL; }
		Iterator(Node* pNode) { p = pNode; }
		Iterator&	operator ++ () { MyAssert(NULL != p); p = p->next; return *this; }
		Iterator&	operator ++ (int) { MyAssert(NULL != p); p = p->next; return *this; }
		Iterator&	operator -- () { MyAssert(NULL != p); p = p->prev; return *this; }
		Iterator&	operator -- (int) { MyAssert(NULL != p); p = p->prev; return *this; }
		bool		operator == (const Iterator& other) const { return this->p == other.p; }
		bool		operator != (const Iterator& other) const { return this->p != other.p; }
		T&			operator * () { MyAssert(NULL != p); return p->elem; }
		const T&	operator * () const { MyAssert(NULL != p); return p->elem; }
	};

private:
	Node*	_insert		(Node* pPosition, const T& elem);
	Node*	_insert		(Node* pPosition);
	Node*	_find			(const T& elem) const;
	Node*	_findLast	(const T& elem) const;
	Node*	_remove		(const T& elem);
	Node*	_remove		(Node* pElem);

private:
	Node* _pHead;
	Node* _pTail;
	int	 _size;
	//IAllocator<Node>* m_pAllocator;
};

template<typename T, typename Alloc>
List<T, Alloc>::List()
{
	_pHead	= NULL;
	_pTail	= NULL;
	//m_pAllocator = &DefaultAllocator< Node >::Singleton();
	_size = 0;
}

template<typename T, typename Alloc>
List<T, Alloc>::~List()
{
	clear();
}

template<typename T, typename Alloc>
void List<T, Alloc>::insert_before(Iterator _where, const T& elem) 
{ 
	Node* pPosition = _where.p;
	if (NULL != pPosition->prev)
	{ 
		//pPosition不是头结点，直接插入即可
		_insert(pPosition->prev, elem); 
	} 
	else
	{
		//pPosition是头结点
		Node* pNewNode = Alloc::alloc();
		pNewNode->elem = elem;
		pNewNode->prev = NULL;
		pNewNode->next = pPosition;
		pPosition->prev = pNewNode;
		_pHead = pNewNode;
	}
}

template<typename T, typename Alloc>
void List<T, Alloc>::clear()
{
	Node* removeNode = _pHead;
	while (removeNode)
	{
		Node* next = removeNode->next;
		_remove(removeNode);
		removeNode = next;
	}
	_pHead = NULL;
	_pTail = NULL;
	_size = 0;
}

//List层面的深拷贝，list_node中的list_node::elem内如果包含指针，不在该拷贝过程中。
//因此对于list_node::elem是浅拷贝
template<typename T, typename Alloc>
List<T, Alloc>& List<T, Alloc>::operator = (const List<T, Alloc>& other)
{
	clear();
	Node* p = other._pHead;
	while (p)
	{
		push_back(p->elem);
		p = p->next;
	}
	return *this;
}

//将elem元素插入到pPosition后面
template<typename T, typename Alloc>
ListNode<T>* List<T, Alloc>::_insert(Node* pPosition, const T& elem)
{
	Node* pNewNode = _insert(pPosition);
	pNewNode->elem = elem;
	return pNewNode;
}

//将elem元素插入到pPosition后面
template<typename T, typename Alloc>
ListNode<T>* List<T, Alloc>::_insert(Node* pPosition)
{
	Node* pNewNode = Alloc::alloc();
	if (NULL == pPosition)
	{
		pNewNode->next = _pHead;
		pNewNode->prev = NULL;
		if (NULL != _pHead)
		{
			_pHead->prev = pNewNode;
		}
	}
	else
	{	
		pNewNode->prev = pPosition;
		pNewNode->next = pPosition->next;
		if (pPosition->next)
		{
			pPosition->next->prev = pNewNode;
		}
		pPosition->next = pNewNode;
	}
	if (NULL == pNewNode->prev)
	{
		_pHead = pNewNode;
	}
	if (NULL == pNewNode->next)
	{
		_pTail = pNewNode;
	}

	++_size;
	return pNewNode;
}

template<typename T, typename Alloc>
ListNode<T>* List<T, Alloc>::_find(const T& elem) const
{
	Node* pFind = _pHead;
	while(NULL != pFind)
	{
		if (pFind->elem == elem)
		{
			return pFind;
		}
		pFind = pFind->next;
	}
	return NULL;
}

template<typename T, typename Alloc>
ListNode<T>* List<T, Alloc>::_findLast(const T& elem) const
{
	Node* pFind = _pTail;
	while(NULL != pFind)
	{
		if (pFind->elem == elem)
		{
			return pFind;
		}
		pFind = pFind->prev;
	}
	return NULL;
}

template<typename T, typename Alloc>
ListNode<T>* List<T, Alloc>::_remove(const T& elem)
{
	return _remove(_find(elem));
}

template<typename T, typename Alloc>
ListNode<T>* List<T, Alloc>::_remove(Node* pElem)
{
	if (NULL == pElem)
	{
		return NULL;
	}
	Node* pNext = pElem->next;
	if (_pHead == pElem)
	{
		_pHead = pElem->next;
	}
	if (_pTail == pElem)
	{
		_pTail = pElem->prev;
	}
	if (NULL != pElem->prev)
	{
		pElem->prev->next = pElem->next;
	}
	if (NULL != pElem->next)
	{
		pElem->next->prev = pElem->prev;
	}
	Alloc::free(pElem);
	--_size;
	return pNext;
}

} //namespace container
using namespace container;

#endif // _LIST_H
