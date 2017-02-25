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

//查找子节点,如果没有找到,返回NULL;
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

//添加子节点到该节点上
void tNode::addChild(tNode *pNode)
{
	if (!pNode) 
	{
		return;
	}
	pNode->_pParent = this;
	_children.push_back(pNode);
}

//删除某个子节点,当该节点存在时返回TRUE,否则返回FALSE
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

//删除所有子节点
void tNode::eraseChildren()
{
	if (_children.empty()) 
	{
		return ;
	}
	_children.clear();
}

//释放自己所所拥有的资源
void tNode::release()
{
	if (_children.empty()) 
	{
		return;
	}
	//释放所有子节点
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
