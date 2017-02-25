////////////////////////////////////////////////////////////////////////////////////
// 
// @file: tNode.h
// @author: by Mr.Chen
// @date: 2014/5/1	21:01
// @brief: 节点类，ref from CYOU.
//          
////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __TNODE_H_
#define __TNODE_H_

#include "tClass.h"
#include <string>
#include <list>
using std::string;
using std::list;

namespace reflect {

class tNode
{
public:
	typedef list<tNode*> ChildList;

	tNode();
	tNode(const char* szName);
	virtual ~tNode() { release(); }

	//返回子节点数目
	virtual int	getChildNum() const	{ return (int)_children.size(); }
	//返回节点名
	virtual const string&	 getNodeName() const	{ return _name; }
	//返回子节点列表
	virtual ChildList&	  getChildren()	{ return _children; }
	//返回父节点
	virtual tNode*	 getParent()	{ return _pParent; }
	//节点初始化
	virtual void	 initial()	 {}
	//逻辑轮循函数
	virtual void	 tick();

	//查找子节点,如果没有找到,返回NULL;
	virtual tNode* lookUpChild(const char* name);

	//添加子节点到该节点上
	virtual void addChild(tNode *pNode);

	//删除某个子节点,当该节点存在时返回TRUE,否则返回FALSE
	virtual bool eraseChild(tNode* pNode);

	//删除所有子节点
	virtual void eraseChildren();

	//释放自己所所拥有的资源
	virtual void release();

protected:
	//节点名
	string	 _name;
	//子节点列表
	ChildList	 _children;
	//父节点指针
	tNode* _pParent;

	friend struct tClass;
	WX_DECLARE_DYNAMIC(tNode);
};

} // namespace reflect
using namespace reflect;

#endif	// __TNODE_H_
