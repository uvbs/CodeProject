#include "tNode.h"
#include "macrodef.h"

namespace reflect {

WX_IMPLEMENT_DYNAMIC(tNode, NULL);

tNode::tNode() : _pParent(NULL)
{}

tNode::tNode(const char* name)
{
	_name = name;	
}

//�����ӽڵ�,���û���ҵ�,����NULL;
tNode*	tNode::lookUpChild(const char* name)
{
	register ChildList::iterator it;
	for (it = _children.begin(); it != _children.end(); it++)
	{
		if ((*it)->getNodeName() == name) 
		{
			return (tNode*)*it;
		}
	}
	return NULL;
}

//����ӽڵ㵽�ýڵ���
void tNode::addChild(tNode *pNode)
{
	if (!pNode) 
	{
		return;
	}
	pNode->_pParent = this;
	_children.push_back(pNode);
}

//ɾ��ĳ���ӽڵ�,���ýڵ����ʱ����TRUE,���򷵻�FALSE
bool tNode::eraseChild(tNode* pNode)
{
	if (_children.empty()) 
	{
		return false;
	}
	register ChildList::iterator it;
	for (it = _children.begin(); it != _children.end(); it++)
	{
		if (pNode == *it)
		{
			_children.erase(it);
			return true;
		}
	}
	return false;
}

//ɾ�������ӽڵ�
void tNode::eraseChildren()
{
	if (_children.empty()) 
	{
		return ;
	}
	_children.clear();
}

//�ͷ��Լ�����ӵ�е���Դ
void tNode::release()
{
	if (_children.empty()) 
	{
		return;
	}
	//�ͷ������ӽڵ�
	register ChildList::iterator it;
	for (it = _children.begin(); it != _children.end(); it++)
	{
		(*it)->release();
		SAFE_DELETE(*it);
	}
	_children.clear();
}

void tNode::tick()
{
	if (_children.empty()) 
	{
		return;
	}
	register ChildList::iterator it = _children.end(), itNext;
	it--;

	do
	{
		itNext = it;
		if (itNext == _children.begin()) 
		{
			itNext = _children.end();
		}
		else 
		{
			itNext--;
		}
		((tNode*)(*it))->tick();

		if (itNext == _children.end()) 
		{
			break;
		}
		it = itNext;

	}while(true);
}

} // namespace reflect
