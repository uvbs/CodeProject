////////////////////////////////////////////////////////////////////////////////////
// 
// @file: variable_system.h
// @author: by Mr.Chen
// @date: 2014/8/21	15:27
// @brief: 变量系统
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
	//设置整数类型值
	void		setIntValue(const string& szKey, int iValue);
	void		setIntValue(const char* szKey, int iValue);

	//得到整数类型值
	int			getIntValue(const string& szKey);
	int			getIntValue(const char* szKey);


	//设置浮点类型值
	void		setFloatValue(const string& szKey, float fValue);
	void		setFloatValue(const char* szKey, float fValue);

	//得到浮点类型值
	float		getFloatValue(const string& szKey);
	//得到浮点类型值
	float		getFloatValue(const char* szKey);

	//设置字符串类型值
	void		setStringValue(const string& szKey, const string& szValue);

	//得到字符串类型值
	string		getStringValue(const string& szKey);
	string		getStringValue(const char* szKey);

	//删除变量
	void		delVariable(const string& szKey);

private:
	//设置变量
	void		_setVariable(const string& szKey, const string& szValue);
	//得到变量
	string		_getVariable(const string& szKey);

private:
	std::map<std::string, std::string>	_variablesMap;
};

#endif // __VAR_SYSTEM_H_

