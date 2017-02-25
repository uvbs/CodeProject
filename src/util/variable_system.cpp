#include "variable_system.h"
#include "string_util.h"
#include "assert.h"

WX_IMPLEMENT_SINGLEON(CVariableSystem);

//设置整数类型值
void CVariableSystem::setIntValue(const string& szKey, int iValue)
{
	__ENTER_FUNCTION

	char temp[10]={0};
	str_util::int32ToStr(iValue,temp,10);
	string szValue = temp;
	_setVariable(szKey, szValue);

	__LEAVE_FUNCTION
}

void CVariableSystem::setIntValue(const char* szKey, int iValue)
{
	string key = szKey;
	setIntValue(key, iValue);
}

//得到整数类型值
int CVariableSystem::getIntValue(const string& szKey)
{
	return str_util::strToInt(_getVariable(szKey).c_str());
}

int CVariableSystem::getIntValue(const char* szKey)
{
	string key = szKey;
	return getIntValue(key);
}

//设置浮点类型值
void CVariableSystem::setFloatValue(const string& szKey, float fValue)
{
	__ENTER_FUNCTION

	char temp[10]={0};
	str_util::floatToStr(fValue,temp);
	string szValue = temp;
	_setVariable(szKey, szValue);

	__LEAVE_FUNCTION
}

void CVariableSystem::setFloatValue( const char* szKey, float fValue )
{
	string key = szKey;
	setFloatValue(key, fValue);
}

//得到浮点类型值
float CVariableSystem::getFloatValue(const string& szKey)
{
	return strToFloat(_getVariable(szKey).c_str());
}

float CVariableSystem::getFloatValue( const char* szKey )
{
	string key = szKey;
	return strToFloat(_getVariable(key).c_str());

}

//设置字符串类型值
void CVariableSystem::setStringValue(const string& szKey, const string& szValue)
{
	_setVariable(szKey, szValue);
}

//得到字符串类型值
string CVariableSystem::getStringValue(const string& szKey)
{
	return _getVariable(szKey);
}

string CVariableSystem::getStringValue(const char* szKey)
{
	string key = szKey;
	return _getVariable(key);
}

//删除变量
void CVariableSystem::delVariable(const string& szKey)
{
	std::map<string, string>::iterator it;
	it = _variablesMap.find(szKey);

	if(it != _variablesMap.end())
		_variablesMap.erase(it);
}

//设置变量
void CVariableSystem::_setVariable(const string& szKey, const string& szValue)
{
	__ENTER_FUNCTION
	_variablesMap[szKey] = szValue;
	__LEAVE_FUNCTION
}

//得到变量
string CVariableSystem::_getVariable(const string& szKey)
{
	std::map<string, string>::iterator it;
	it = _variablesMap.find(szKey);
	if(it != _variablesMap.end())
		return it->second;
	return "";
}
