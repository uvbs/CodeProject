////////////////////////////////////////////////////////////////////////////////
//	allocator
//	2015.05.02 Mr.chen
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

#include <new>	//placement new
#include <stdlib.h> //malloc()
#include "assert.h"
#include "stack.h"

namespace algorithm 
{
////////////////////////////////////////////////////////////////////////////////
//	DefaultAllocator
////////////////////////////////////////////////////////////////////////////////
template<typename T>
class default_allocator
{
public:
	static T* alloc()					{ return ::new T(); }
	static  void	free(T* const pElem)	{ ::delete pElem;	}
};


////////////////////////////////////////////////////////////////////////////////
//	single_allocator
//	仅用于单个对象的分配
////////////////////////////////////////////////////////////////////////////////
template<typename T>
class single_allocator
{
public:
	static single_allocator<T>& getSingleton() { static single_allocator<T> _instance; return _instance; }
	static void	init(int maxSize);
	static void	release();
	static bool	isFull() { return _s_allocCount == 0 && _s_free->length() == 0; }

	static T*	alloc();
	static void	free(T* const pElem);

private:
	static T* _s_memory;
	static Stack<int>* _s_free;
	static int _s_allocCount;
	static bool _s_hasInit;
};

template<typename T>
T* single_allocator<T>::_s_memory = NULL;

template<typename T>
Stack<int>*	single_allocator<T>::_s_free;

template<typename T>
int single_allocator<T>::_s_allocCount;

template<typename T>
bool single_allocator<T>::_s_hasInit;

template<typename T>
void single_allocator<T>::init(const int maxSize)
{
	if (_s_hasInit)
	{
		Assert(0);
		return;
	}
	//为了规避init时对T的构造函数的调用，这里使用malloc
	//后来为了方便new_tracer能够记录single_allocator分配的内存，这里又改成了new byte[]
	_s_memory = static_cast<T*>(static_cast<void*>(new byte[maxSize * sizeof(T)]));
	Assert(_s_memory);
	_s_free = new Stack<int>(maxSize);
	Assert(_s_free);
	_s_allocCount = maxSize; 
	_s_hasInit = true;
}

template<typename T>
void single_allocator<T>::release()
{
	_s_hasInit = false;
	delete[] (byte*)_s_memory;
	delete _s_free;
	//::free(m_memory);
}

template<typename T>
T* single_allocator<T>::alloc()
{
	if (!_s_hasInit)
	{
		Assert(0);
		return NULL;
	}
	if (_s_allocCount > 0)
	{
		--_s_allocCount;
		T* p = new (&_s_memory[_s_allocCount]) T;
		return p;
	}
	else if (_s_free->length() > 0)
	{
		int index = _s_free->pop();
 		T* p = new (&_s_memory[index]) T;
 		return p;
	}
	else
	{
		//throw(1);	//TODO 定义对应异常
		//Assert(0);
		return NULL;
	}
}

template<typename T>
void single_allocator<T>::free(T* const pElem)
{
	if (!_s_hasInit)
	{
		Assert(0);
		return;
	}
	if (NULL != pElem)
	{
		pElem->~T();
		int index = pElem - &(_s_memory[0]);
		_s_free->push(index);
	}
}


////////////////////////////////////////////////////////////////////////////////
//	single_allocator
//	仅用于单个对象的分配
////////////////////////////////////////////////////////////////////////////////
template<class T, int alignByte = 16>
class aligned_allocator
{
public:
	static T* alloc(size_t size)
	{
		char *ptr, *ptr2, *aligned_ptr;
		ptr = new char[size * sizeof(T) + alignByte + sizeof(int)];
		ptr2 = ptr + sizeof(int);
		aligned_ptr = ptr2 + (alignByte - ((size_t)ptr2 & (alignByte - 1) ));
		ptr2 = aligned_ptr - sizeof(int);
		*((int*)ptr2) = (int)(aligned_ptr - ptr);
		return(T*)(aligned_ptr);
	}
	static void free(T* ptr)
	{
		int *ptr2 = (int*)ptr - 1;
		char* const m = (char*)ptr - *ptr2;
		delete[] m;
	}
};

} //namespace algorithm
using namespace algorithm;

#endif // _ALLOCATOR_H


