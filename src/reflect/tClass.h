////////////////////////////////////////////////////////////////////////////////////
// 
// @file: tClass.h
// @author: by Mr.Chen
// @date: 2014/5/1	21:49
// @brief: 节点类信息定义类，ref from CYOU.
//          
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __TCLASS_H_
#define __TCLASS_H_

namespace reflect {

#if defined (_WIN32)
#define STDCALL __stdcall
#elif defined(_LINUX64)
#define STDCALL
#endif

class tNode;
struct tClass
{
	char* szClassName;						//!< Node类名称
	int	nObjectSize;						//!< Node类的大小
	tNode* (STDCALL* pfnCreateObject)();	//!< 函数指针，用于生成一个INode类实例
	tClass* pBaseClass;						//!< 所继承的基类
	tClass* pNextClass;						//!< 下一个类
	tNode* createObject(const char* szName);//!< 用于生成一个实例的函数
};

	//根据类名取得定义类
#define GETCLASS(className)  (&className::m_class##className)

	//类定义声明宏
#define WX_DECLARE_DYNAMIC(className) \
public: \
	static tClass				m_class##className; \
	virtual const tClass*		getClass() const; \
	static tNode* STDCALL		createObject();

	//类定义实现宏
#define WX_IMPLEMENT_DYNAMIC(className, baseClass) \
	static char sz##className[] = #className; \
	tClass className::m_class##className = \
	{ sz##className, sizeof(className), className::createObject, baseClass, NULL }; \
	const tClass* className::getClass() const \
	{ return &className::m_class##className; } \
	tNode* className::createObject() \
	{ return new className; } 


	//纯虚类类定义实现宏
#define WX_IMPLEMENT_DYNAMIC_VIRTUAL(className, baseClass) \
	static char sz##className[] = #className; \
	tClass className::m_class##className = \
	{ sz##className, sizeof(className), className::createObject, baseClass, NULL }; \
	const tClass* className::getClass() const \
	{ return &className::m_class##className; } \
	tNode* className::createObject() \
	{ throw("Pure virtual class"); return NULL; } 

} // namespace reflect
using namespace reflect;

#endif	// __TCLASS_H_
