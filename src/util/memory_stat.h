////////////////////////////////////////////////////////////////////////////////////
// 
// @file: memory_statistics.h
// @author: by Mr.Chen
// @date: 2015/4/16	17:26
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __MEMORY_STATISTIC_H_
#define __MEMORY_STATISTIC_H_

#include <stdlib.h>
#include <iostream>
#include <list>

using std::list;
using std::endl;
using std::cout;
using std::bad_alloc;

// �ڴ�ڵ�
class MemNode
{
public:
    MemNode();
    ~MemNode() {}
    MemNode(void* Buffer, size_t s, char* File, int Line);
    MemNode(const MemNode& Temp);

public:
    void*	_object;		//�ڴ������õ�ָ��
    size_t _size;			//�ڴ����Ĵ�С
    char*	_file;	//�ڴ������������ļ�λ��
    int		_lineno;		//�ڴ������������к�
};

// �ڴ�ڵ���
class NodeList
{
public:
    bool empty();
    void add(const MemNode& Temp);
    void remove(void* Object);
    void check();

public:
    list<MemNode> _list;
};

// typedef
typedef list<MemNode>::iterator NodeListIter;


// ����
extern void* operator new(size_t Size, char* FileName, int LineNum);
extern void* operator new[](size_t Size, char* FileName, int LineNum);
extern void operator delete(void* Object);
extern void operator delete[](void* Object);

//
extern void check();
extern NodeList g_memNodeList;

#define new new(__FILE__, __LINE__ )

//#if defined (_WIN32)
//#ifdef _DEBUG 
//#define DEBUG_CLIENTBLOCK  new( _CLIENT_BLOCK, __FILE__, __LINE__) 
//#else
//#define DEBUG_CLIENTBLOCK
//#endif
//
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h> 
//#ifdef _DEBUG 
//#define new DEBUG_CLIENTBLOCK 
//#endif
//#endif

#endif

