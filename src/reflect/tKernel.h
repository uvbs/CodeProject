////////////////////////////////////////////////////////////////////////////////////
// 
// @file: tKernel.h
// @author: by Mr.Chen
// @date: 2014/5/1	22:30
// @brief: ���ݺ��ģ�ref from CYOU.
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
	//		���������
	//--------------------------------------------------
public:
	//��ĳ����ע�ᵽȫ�����������
	void registerClass(tClass* pNewClass);
	//����������Ƿ���м̳й�ϵ
	bool isKindOf(const tClass* pThisClass, const tClass* pBaseClass) const;
	//�����ַ�����ʽ����ȡ��ĳ������
	tClass* openClass(const char* szClassName);

protected:
	//ȫ�ֶ����������
	map<string, tClass*> _classesMap;
	//��������е�һ����
	tClass* _pFirstClass;
	//���ڵ�
	tNode _root;

	//--------------------------------------------------
	//		�ڵ�������
	//--------------------------------------------------
public:
	/**
		�´���һ���ڵ���뵽�ڵ����ϣ����нڵ�Ĵ�����Ӧ��ͨ������������������
			@param
				szClassName �������Ľڵ���������������Ѿ�ע���
				szPosition  �������Ľڵ����ڵĸ��ڵ��·��
				pParentNode	�������Ľڵ����ڵĸ��ڵ��ָ��
				szNodeName	�������Ľڵ������

		�ɹ������´����Ľڵ㣬���ʧ��,����NULL	
	*/
	///�����ַ�����ʽ·��������
	virtual tNode*	newNode(const char* szClassName, const char* szPosition, const char* szNodeName);
	///���ݸ��ڵ�ָ����ʽ������
	virtual tNode*	newNode(const char* szClassName, tNode* pParentNode, const char* szNodeName);
	///����·����ȡ�ýڵ㣬���� pNode = GetNode("scene\\object")
	virtual tNode*	getNode(const char* szPathName);

public:
	tKernel();
	virtual ~tKernel();
};

} // namespace reflect
using namespace reflect;

#endif	// __TKERNEL_H_
