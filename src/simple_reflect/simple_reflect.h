////////////////////////////////////////////////////////////////////////////////////
// 
// @file: simple_reflect.h
// @author: by Mr.Chen
// @date: 2015/7/15	15:12
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef APP_GENSOFT_P2P_CLIENT_LIVE_CODEPRJ_REFLECT_V2_SIMPLEREFLECT_H
#define APP_GENSOFT_P2P_CLIENT_LIVE_CODEPRJ_REFLECT_V2_SIMPLEREFLECT_H

#include "type.h"
#include <string.h>

typedef struct tagCPropertyBase
{
	const char* szName;
	const char* szType;
	tagCPropertyBase *lpNext;
}CPropertyBase;

template<typename C,typename T>
struct CPropertyInfo
{
	const char* szName;
	const char* szType;
	CPropertyBase *lpNext;
	void (C::*set)(T);
	T (C::*get)();
	CPropertyInfo(const char* szName,const char* szType,void (C::*Setter)(T),T (C::*Getter)())
	{
		this->szName = szName;
		this->szType = szType;
		this->set = Setter;
		this->get = Getter;
		this->lpNext = NULL;
		//初始化含有属性类的链表头节点
		if (C::lpFirstProperty == NULL)
		{
			C::lpFirstProperty = (CPropertyBase*)this;
		}
		//将当前属性信息添加到链表结尾
		if (C::lpLastProperty != NULL)
		{
			C::lpLastProperty->lpNext = (CPropertyBase*)this;
		} 
		C::lpLastProperty=(CPropertyBase*)this; 
	}
};

// 用来确定链表头尾：
#define REFLECT_DECLARE() \
    static CPropertyBase* lpFirstProperty; \
    static CPropertyBase* lpLastProperty; 

// 初始化链表头尾：
#define REFLECT_IMPLEMENT(C) \
    CPropertyBase* C::lpFirstProperty = NULL; \
    CPropertyBase* C::lpLastProperty = NULL;

// 定义属性元数据：
#define PROPERTY_DECLARE(C,T,P) \
    static struct CPropertyInfo<C,T> Prop_##P; \
    void set_##P(T); \
    T get_##P();

// 初始化属性元数据：
#define PROPERTY_IMPLEMENT(C,T,P) \
    struct CPropertyInfo<C,T> C::Prop_##P(#P, #T, &C::set_##P, &C::get_##P);

// 访问属性元数据：
#define PROPERTY(C,P) &(C::Prop_##P)
// 修改属性：
#define SET_PROP(O,P,V) (O->*P->set)(V)
// 读取属性：
#define GET_PROP(O,P) (O->*P->get)()

// 根据属性名查找属性：
template<typename C,typename T>
CPropertyInfo<C,T>* findProperty(const char* szName)
{
	CPropertyBase* item = C::lpFirstProperty;
	while (item)
	{
		if (!::strcmp(item->szName,szName))
		{
			return reinterpret_cast<CPropertyInfo<C,T>*>(item);
		}
		item = item->lpNext;
	}
	return NULL;
}

// 修改属性：
template<typename C,typename T>
inline void setProperty(C*object, CPropertyInfo<C,T>* prop, T value)
{
	if (object && prop->set)
	{
		(object->*prop->set)(value);
	}
}

// 读取属性：
template<typename C,typename T>
inline T getProperty(C*object, CPropertyInfo<C,T>* prop)
{
	if (object && prop->get)
	{
		return (object->*prop->get)();
	}
	return (T)0;
}

#endif //_SIMPLE_REFLECT_H
