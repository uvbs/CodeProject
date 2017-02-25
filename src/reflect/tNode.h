////////////////////////////////////////////////////////////////////////////////////
// 
// @file: tNode.h
// @author: by Mr.Chen
// @date: 2014/5/1	21:01
// @brief: �ڵ��࣬ref from CYOU.
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

	//�����ӽڵ���Ŀ
	virtual int	getChildNum() const	{ return (int)_children.size(); }
	//���ؽڵ���
	virtual const string&	 getNodeName() const	{ return _name; }
	//�����ӽڵ��б�
	virtual ChildList&	  getChildren()	{ return _children; }
	//���ظ��ڵ�
	virtual tNode*	 getParent()	{ return _pParent; }
	//�ڵ��ʼ��
	virtual void	 initial()	 {}
	//�߼���ѭ����
	virtual void	 tick();

	//�����ӽڵ�,���û���ҵ�,����NULL;
	virtual tNode* lookUpChild(const char* name);

	//����ӽڵ㵽�ýڵ���
	virtual void addChild(tNode *pNode);

	//ɾ��ĳ���ӽڵ�,���ýڵ����ʱ����TRUE,���򷵻�FALSE
	virtual bool eraseChild(tNode* pNode);

	//ɾ�������ӽڵ�
	virtual void eraseChildren();

	//�ͷ��Լ�����ӵ�е���Դ
	virtual void release();

protected:
	//�ڵ���
	string	 _name;
	//�ӽڵ��б�
	ChildList	 _children;
	//���ڵ�ָ��
	tNode* _pParent;

	friend struct tClass;
	WX_DECLARE_DYNAMIC(tNode);
};

} // namespace reflect
using namespace reflect;

#endif	// __TNODE_H_
