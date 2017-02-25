////////////////////////////////////////////////////////////////////////////////////
// 
// @file: tClass.h
// @author: by Mr.Chen
// @date: 2014/5/1	21:49
// @brief: �ڵ�����Ϣ�����࣬ref from CYOU.
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
	char* szClassName;						//!< Node������
	int	nObjectSize;						//!< Node��Ĵ�С
	tNode* (STDCALL* pfnCreateObject)();	//!< ����ָ�룬��������һ��INode��ʵ��
	tClass* pBaseClass;						//!< ���̳еĻ���
	tClass* pNextClass;						//!< ��һ����
	tNode* createObject(const char* szName);//!< ��������һ��ʵ���ĺ���
};

	//��������ȡ�ö�����
#define GETCLASS(className)  (&className::m_class##className)

	//�ඨ��������
#define WX_DECLARE_DYNAMIC(className) \
public: \
	static tClass				m_class##className; \
	virtual const tClass*		getClass() const; \
	static tNode* STDCALL		createObject();

	//�ඨ��ʵ�ֺ�
#define WX_IMPLEMENT_DYNAMIC(className, baseClass) \
	static char sz##className[] = #className; \
	tClass className::m_class##className = \
	{ sz##className, sizeof(className), className::createObject, baseClass, NULL }; \
	const tClass* className::getClass() const \
	{ return &className::m_class##className; } \
	tNode* className::createObject() \
	{ return new className; } 


	//�������ඨ��ʵ�ֺ�
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
