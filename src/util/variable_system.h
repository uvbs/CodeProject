////////////////////////////////////////////////////////////////////////////////////
// 
// @file: variable_system.h
// @author: by Mr.Chen
// @date: 2014/8/21	15:27
// @brief: ����ϵͳ
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __VAR_SYSTEM_H_
#define __VAR_SYSTEM_H_

#include <string>
#include <map>
using std::string;
using std::map;
#include "singleton.h"

class CVariableSystem : public Singleton<CVariableSystem>
{
public:
	CVariableSystem() {_variablesMap.clear();}
	~CVariableSystem() {_variablesMap.clear();}

public:
	//������������ֵ
	void		setIntValue(const string& szKey, int iValue);
	void		setIntValue(const char* szKey, int iValue);

	//�õ���������ֵ
	int			getIntValue(const string& szKey);
	int			getIntValue(const char* szKey);


	//���ø�������ֵ
	void		setFloatValue(const string& szKey, float fValue);
	void		setFloatValue(const char* szKey, float fValue);

	//�õ���������ֵ
	float		getFloatValue(const string& szKey);
	//�õ���������ֵ
	float		getFloatValue(const char* szKey);

	//�����ַ�������ֵ
	void		setStringValue(const string& szKey, const string& szValue);

	//�õ��ַ�������ֵ
	string		getStringValue(const string& szKey);
	string		getStringValue(const char* szKey);

	//ɾ������
	void		delVariable(const string& szKey);

private:
	//���ñ���
	void		_setVariable(const string& szKey, const string& szValue);
	//�õ�����
	string		_getVariable(const string& szKey);

private:
	std::map<std::string, std::string>	_variablesMap;
};

#endif // __VAR_SYSTEM_H_

