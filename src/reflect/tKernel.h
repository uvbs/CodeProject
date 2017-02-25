////////////////////////////////////////////////////////////////////////////////////
// 
// @file: tKernel.h
// @author: by Mr.Chen
// @date: 2014/5/1	22:30
// @brief: 数据核心，ref from CYOU.
//          
////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __TKERNEL_H_
#define __TKERNEL_H_

#include "tNode.h"
#include "singleton.h"
#include <string>
#include <map>
using std::string;
using std::map;

namespace reflect {

class tKernel : public Singleton<tKernel>
{
	//--------------------------------------------------
	//		定义类相关
	//--------------------------------------------------
public:
	//将某个类注册到全局类检索表函数
	void registerClass(tClass* pNewClass);
	//检查两个类是否具有继承关系
	bool isKindOf(const tClass* pThisClass, const tClass* pBaseClass) const;
	//根据字符串形式类名取得某定义类
	tClass* openClass(const char* szClassName);

protected:
	//全局定义类检索表
	map<string, tClass*> _classesMap;
	//类检索表中第一个类
	tClass* _pFirstClass;
	//根节点
	tNode _root;

	//--------------------------------------------------
	//		节点管理相关
	//--------------------------------------------------
public:
	/**
		新创建一个节点加入到节点树上，所有节点的创建都应该通过这两个函数来创建
			@param
				szClassName 所创建的节点类名，该类必须已经注册过
				szPosition  所创建的节点所在的父节点的路径
				pParentNode	所创建的节点所在的父节点的指针
				szNodeName	所创建的节点的名称

		成功返回新创建的节点，如果失败,返回NULL	
	*/
	///根据字符串形式路径来创建
	virtual tNode*	newNode(const char* szClassName, const char* szPosition, const char* szNodeName);
	///根据父节点指针形式来创建
	virtual tNode*	newNode(const char* szClassName, tNode* pParentNode, const char* szNodeName);
	///根据路径名取得节点，例如 pNode = GetNode("scene\\object")
	virtual tNode*	getNode(const char* szPathName);

public:
	tKernel();
	virtual ~tKernel();
};

} // namespace reflect
using namespace reflect;

#endif	// __TKERNEL_H_
