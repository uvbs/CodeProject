/////////////////////////////////////////////
#if defined _USE_MEM_STATIS_
/////////////////////////////////////////////

#include "memory_statistics.h"
#include <stdio.h>
#undef new

MemNode::MemNode()
{
    _object = 0;
    _file = 0;
}

MemNode::MemNode(void* Buffer, size_t s, char* File, int Line)
{
    _object = Buffer;
    _size = s;
    _file = File;
    _lineno = Line;
}

MemNode::MemNode(const MemNode& Temp)
{
    _file = Temp._file;
    _lineno = Temp._lineno;
    _object = Temp._object;
    _size = Temp._size;
}

bool NodeList::empty()
{
    if (_list.size() > 0)
    {
        return false;
    }
    else 
    {
        return true;
    }
}

void NodeList::add(const MemNode& Temp)
{
    _list.push_back(Temp);
}

void NodeList::remove(void* pObject)
{
    NodeListIter iter = _list.begin();
    NodeListIter endIter = _list.end();
    for (; iter != endIter; ++iter)
    {
	    if (iter->_object == pObject)
	    {
		    _list.erase(iter);
		    break;
	    }
    }
}

void NodeList::check()
{
    if (!empty())
    {
        cout << "´æÔÚÄÚ´æÐ¹Â©" << endl;
        cout << "Line\tSize\tPath" << endl;
	    NodeListIter iter = _list.begin();
	    NodeListIter endIter = _list.end();
	    char s[200] = {0};
	    for (; iter != endIter; ++iter)
	    {
		    char Line[20];
		    itoa(iter->_lineno, Line, 10);
		    cout << Line;
		    cout << "\t";
		    memset(s, 0, sizeof(s));
		    itoa(iter->_size, s, 10);
		    cout << s;
		    cout << "\t";
		    cout << iter->_file;
		    cout << "\t";
	    }
    }
    else
    {
	    cout << "²»´æÔÚÄÚ´æÐ¹Â©";
    }
}

NodeList g_memNodeList;

void* operator new(size_t size, char* file, int lineno)
{
    if (size == (size_t)0)
    {
        size=1;
    }
    void* result = ::operator new(size);
    if (!result)
    {
        throw bad_alloc();
    }
    else
    {
#if defined _USE_MEM_STATIS_
        MemNode tmp(result, size, file, lineno);
        g_memNodeList.add(tmp);
#endif
    }
    return result;
}

void* operator new[](size_t size, char* file, int lineno)
{
   if (size==(size_t)0)
   {
       size=1;
   }
   void* result=::operator new[](size);
   if (!result)
   {
       throw bad_alloc();
   }
   else
   {
#if defined _USE_MEM_STATIS_
        MemNode tmp(result, size, file, lineno);
        g_memNodeList.add(tmp);
#endif
    }
    return result;
}

void operator delete(void* object)
{
    if (!object)
    {
        return;
    }
#if defined _USE_MEM_STATIS_
    g_memNodeList.remove(object);
#endif
    //::delete(Object);
    free(object);
}

void operator delete[](void* Object)
{
    if (!Object) return;
#if defined _USE_MEM_STATIS_
    g_memNodeList.remove(Object);
#endif
    //::operator delete[](Object);
    free(Object);
}

void check()
{
    g_memNodeList.check();
}

/////////////////////////////////////////////
#endif  //end of file
/////////////////////////////////////////////