#include "ini_reader.h"
#include "assert.h"
#include "string_util.h"
#include "macrodef.h"
#include <stdio.h>

#if defined(_LINUX64)
#include <string.h>          // memset
#endif

namespace file_parser {

IniReader::IniReader()
{
	__ENTER_FUNCTION
	_filename[0] = 0;
	_dataLen = 0;
	_pData = NULL;
	_pSectIndexArr = NULL;
	_sectCount = 0;
	__LEAVE_FUNCTION
}

IniReader::IniReader(const char *pFileName)
{
	__ENTER_FUNCTION
	memset(_filename, 0, sizeof(_filename));
	str_util::strncpy(_filename, pFileName, sizeof(_filename) - 1);
	_dataLen = 0;
	_pData = NULL;
	_pSectIndexArr = NULL;
	_sectCount = 0;
	bool ret = open(pFileName);
	if (!ret)
	{
		Assert(0);
	}
	__LEAVE_FUNCTION
}

IniReader::~IniReader()
{
	__ENTER_FUNCTION
	SAFE_DELETE_ARRAY(_pData);
	SAFE_DELETE_ARRAY(_pSectIndexArr);
	__LEAVE_FUNCTION
}

bool IniReader::open(const char* pFileName)
{
	__ENTER_FUNCTION

	//判断文件名是否有效
	Assert(pFileName != NULL);
	if (_filename[0] == '\0')
	{
		str_util::strncpy(_filename, pFileName, sizeof(_filename)-1);
	}
	
	//open file
	FILE*	 fp = ::fopen(_filename, "r+b");
	if (fp == NULL)
	{
		AssertSpecial(0, "Can't Open File!, FileName = %s ", _filename);
	}
	//get file length
	::fseek(fp, 0, SEEK_END);
	_dataLen = (int)::ftell(fp);

	//read file
	::fseek(fp, 0, SEEK_SET);
	_pData = new char[_dataLen + 1];
	Assert(_pData);
	::fread(_pData, 1, _dataLen, fp);
	_pData[_dataLen] = '\0';

	//close file
	::fclose(fp);
	fp = NULL;

	uint16 uFlag = 0;
	memcpy(&uFlag, _pData, sizeof(uFlag));
	if((uFlag == 0xFFFE) || (uFlag == 0xFEFF))
	{
		AssertSpecial(0, "Can't Read Unicode File!, FileName = %s ", _filename);
	}

	//初始化段起始位置索引
	_initSectIndex();

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool IniReader::save()
{
	__ENTER_FUNCTION
	Assert(_filename[0] != '\0');

	FILE* pFile = ::fopen(_filename, "wb");
	Assert(pFile!=NULL);
	
	::fseek(pFile, 0, SEEK_SET);
	::fwrite(_pData, _dataLen, 1, pFile);
	::fclose(pFile);

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool IniReader::readString(const char* pSect, const char* pKey, char* pOut, int len) const
{
	__ENTER_FUNCTION
	Assert(pSect != NULL);
	Assert(pKey != NULL);

	//找到段的起始偏移
	int pos = _findSect(pSect);
	if (pos == -1)
	{
		return false;
	}

	//跳到下一行（键值对行）
	pos = _findNextLineBegin(pos);
	if (pos == -1)
	{
		return false;
	}

	//查找键,没找到
	pos = _findKey(pos, pKey);
	if (pos == -1)
	{
		return false;
	}

	//获取值
	return _getValue(pos, pOut, len);
	__LEAVE_FUNCTION
	return false;
}

int IniReader::readInt(const char* pSect, const char* pKey) const
{
	__ENTER_FUNCTION

	char buffer[INI_VALUE_LEN] = {0};
	memset(buffer, 0, sizeof(buffer));
	if (!readString(pSect, pKey, buffer, INI_VALUE_LEN))
	{
		return -1;
	}
	if (!str_util::isInteger(buffer))
	{
		return -1;
	}
	return str_util::strToInt(buffer);

	__LEAVE_FUNCTION
	return -1;
}

float IniReader::readFloat(const char* pSect, const char* pKey) const
{
	__ENTER_FUNCTION

	char buffer[INI_VALUE_LEN] = {0};
	memset(buffer, 0, sizeof(buffer));
	if (!readString(pSect, pKey, buffer, INI_VALUE_LEN))
	{
		return -1;
	}
	if (!str_util::isFloat(buffer))
	{
		return -1;
	}
	return str_util::strToFloat(buffer);

	__LEAVE_FUNCTION
	return -1;
}

bool IniReader::writeInt(const char* pSect, const char* pKey, int iValue)
{
	__ENTER_FUNCTION

	char buffer[INI_KEY_LEN+INI_VALUE_LEN];
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%d", iValue);
	return writeString(pSect, pKey, buffer);

	__LEAVE_FUNCTION
	return false;
}

bool IniReader::writeFloat(const char* pSect, const char* pKey, float fValue)
{
	__ENTER_FUNCTION

	char buffer[INI_KEY_LEN+INI_VALUE_LEN];
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%f", fValue);
	return writeString(pSect, pKey, buffer);

	__LEAVE_FUNCTION
	return false;
}

bool IniReader::writeString(const char* pSect, const char* pKey, char* pValue)
{
	__ENTER_FUNCTION

	Assert(pSect != NULL);
	Assert(pKey != NULL);
	Assert(pValue != NULL);

	//查找段
	int pos = _findSect(pSect);
	//不存在则新建
	if (pos == -1)
	{
		addSect(pSect);
		pos = _findSect(pSect);
		Assert(pos != -1);
	}

	//跳到下一行（键值对行）
	pos = _findNextLineBegin(pos);
	if (pos == -1)
	{
		return addKeyValuePair(pSect, pKey, pValue);
	}

	//查找键,没找到则添加
	pos = _findKey(pos, pKey);
	if (pos == -1)
	{
		return addKeyValuePair(pSect, pKey, pValue);
	}
	//找到了修改
	return modifyKeyValuePair(pSect, pKey, pValue);

	__LEAVE_FUNCTION
	return false;
}

bool IniReader::addKeyValuePair(const char* pSect, const char* pKey, const char* pValue)
{
	__ENTER_FUNCTION

	int pos1 = _findSect(pSect);
	if (pos1 == -1)
	{
		return false;
	}
	
	int pos2 = _findKey(pos1, pKey);
	if (pos2 != -1)
	{
		return false;
	}
	
	//插入位置
	int insertPos = -1;
	int index = _getIndexByPos(pos1);
	if (index < _sectCount - 1)
	{
		insertPos = _pSectIndexArr[index + 1] - 1;
	}
	else
	{
		insertPos = _dataLen;
	}
	
	char KeyValuePair[INI_KEY_LEN + INI_VALUE_LEN];
	memset(KeyValuePair, 0, sizeof(KeyValuePair));

	int KeyLen = str_util::strlen(pKey);
	int ValueLen = str_util::strlen(pValue);

	//添加换行
	KeyValuePair[0] = '\r';
	KeyValuePair[1] = '\n';

	str_util::strncpy(&KeyValuePair[2], pKey, KeyLen);
	KeyValuePair[KeyLen+2] = '=';
	str_util::strncpy(&KeyValuePair[KeyLen + 3], pValue, ValueLen);

	int IncSize = 2 + KeyLen + 1 + ValueLen;
	
	char* pNewData = new char[_dataLen+IncSize];
	Assert(pNewData!=NULL);

	//拷贝原来的数据的前半
	if (insertPos > 0)
	{
		memcpy(pNewData, _pData, insertPos);
	}
	
	//插入键值对
	memcpy(&pNewData[insertPos], KeyValuePair, IncSize);
	
	//拷贝原数据的后半
	if (_dataLen - insertPos > 0)
	{
		memcpy(&pNewData[insertPos + IncSize], &pNewData[insertPos], _dataLen - insertPos);
	}

	//干掉原数据
	SAFE_DELETE_ARRAY(_pData);
	_pData = pNewData;
	_dataLen += IncSize;

	//重构段起始位置索引
	_initSectIndex();

	return  true;
	__LEAVE_FUNCTION
	return false;
}

bool IniReader::modifyKeyValuePair(const char* pSect, const char* pKey, const char* pValue)
{
	__ENTER_FUNCTION

	//查找段
	int pos = _findSect(pSect);
	Assert(pos != -1);

	//跳到下一行（键值对行）
	pos = _findNextLineBegin(pos);
	Assert(pos != -1);

	//查找键,没找到则添加
	pos = _findKey(pos, pKey);
	Assert(pos != -1);

	int oldSize = -1;
	int endPos = -1;
	for (int i = pos; i < _dataLen; ++i)
	{
		if (_pData[i] == '\n')
		{
			oldSize = i - pos - 1;
			endPos = i - 1;
			break;
		}

		//最后一行
		if (i == _dataLen - 1)
		{
			oldSize = _dataLen - pos;
			endPos = _dataLen;
		}
	}

	char keyValuePair[INI_KEY_LEN + INI_VALUE_LEN];
	memset(keyValuePair, 0, sizeof(keyValuePair));

	int keyLen = str_util::strlen(pKey);
	int valueLen = str_util::strlen(pValue);

	str_util::strncpy(&keyValuePair[0], pKey, keyLen);
	keyValuePair[keyLen] = '=';
	str_util::strncpy(&keyValuePair[keyLen + 1], pValue, valueLen);

	int newSize = keyLen + 1 + valueLen;
	int incSize = newSize - oldSize;

	char* pNewData = new char[_dataLen + incSize];
	Assert(pNewData != NULL);

	//拷贝原来的数据的前半
	if (pos > 0)
	{
		memcpy(pNewData, _pData, pos);
	}

	//插入新键值对
	memcpy(&pNewData[pos], keyValuePair, newSize);

	//拷贝原数据的后半
	if (_dataLen - endPos > 0)
	{
		memcpy(&pNewData[pos+newSize], &_pData[endPos], _dataLen - endPos);
	}

	//干掉原数据
	SAFE_DELETE_ARRAY(_pData);
	_pData = pNewData;
	_dataLen += incSize;

	//重构段起始位置索引
	_initSectIndex();

	return true;
	__LEAVE_FUNCTION
	return false;
}

int IniReader::_getIndexByPos(int Pos) const
{
	__ENTER_FUNCTION

	for (int i = 0; i < _sectCount; ++i)
	{
		if (_pSectIndexArr[i] == Pos)
		{
			return i;
		}
	}

	return -1;
	__LEAVE_FUNCTION
	return -1;
}

int IniReader::_findSect(const char* pSect) const
{
	__ENTER_FUNCTION
	Assert(pSect!=NULL);

	char buffer[INI_VALUE_LEN] = {0};
	memset(buffer, 0, sizeof(buffer));
	for (int i = 0; i < _sectCount; ++i)
	{
		int pos = _pSectIndexArr[i];
		while (_pData[pos] != ']' && pos<_dataLen)
		{
			++pos;
		}
		memset(buffer, 0, sizeof(buffer));
		str_util::strncpy(buffer, &_pData[_pSectIndexArr[i]], pos - _pSectIndexArr[i]);
		str_util::trim(buffer);
		if (str_util::strcmp(buffer, pSect) == 0)
		{
			return _pSectIndexArr[i];
		}
	}

	//没找到
	return -1;
	__LEAVE_FUNCTION
	return -1;
}

int IniReader::_getLine(char* pBuff, int bufferLen, int beginPos) const
{
	__ENTER_FUNCTION
	Assert(pBuff != 0);
	Assert(bufferLen > 0);
	Assert(beginPos >= 0 && beginPos < _dataLen);

	int index = 0;
	while (beginPos < _dataLen && index < bufferLen - 1)
	{
		if (_pData[beginPos]=='\n' || _pData[beginPos]=='\r')
		{
			break;
		}
		else
		{
			pBuff[index++] = _pData[beginPos++];
		}
	}

	pBuff[index] = '\0';
	
	//跳过换行
	while (beginPos<_dataLen)
	{
		if (_pData[beginPos] == '\r' || _pData[beginPos] == '\n')
		{
			++beginPos;
		}
		else
		{
			return beginPos;
		}
	}

	return _dataLen;
	__LEAVE_FUNCTION
	return -1;
}

int IniReader::_findKey(int begin, const char* pKey) const
{
	__ENTER_FUNCTION
	Assert(pKey != NULL);

	char lineBuffer[INI_LINE_LEN] = {0};
	memset(lineBuffer, 0, sizeof(lineBuffer));
	char keyBuffer[INI_KEY_LEN] = {0};
	memset(keyBuffer, 0, sizeof(keyBuffer));	

	while (begin < _dataLen)
	{
		//获取一行
		memset(lineBuffer, 0, sizeof(lineBuffer));
		int nextLineBegin = _getLine(lineBuffer, INI_LINE_LEN, begin);
		
		int lineLen = (int)str_util::strlen(lineBuffer);
		//跳过空行
		if (lineLen == 0)
		{
			begin = nextLineBegin;
			continue;
		}

		//跳过注释行
		if (lineBuffer[0] == '#')
		{
			begin = nextLineBegin;
			continue;
		}
		
		memset(keyBuffer, 0, sizeof(keyBuffer));
		for (int i = 0; i < lineLen; ++i)
		{
			//查找到下一段了，失败
			if (lineBuffer[i] == '[')
			{
				if (i == 0 || lineBuffer[i - 1]=='\n')
				{
					return -1;
				}		 
			}
			if (lineBuffer[i] != '=')
			{
				keyBuffer[i] = lineBuffer[i];
			}
			else
			{
				//删除空格
				str_util::trim(keyBuffer);
				//找到Key
				if (str_util::strcmp(keyBuffer, pKey) == 0)
				{
					//返回Key所在行的起始位置
					return begin;
				}
				else
				{
					break;
				}
			}
		}

		begin = nextLineBegin;
	}

	return -1;
	__LEAVE_FUNCTION
	return -1;
}

int IniReader::_findNextLineBegin(int pos) const
{
	__ENTER_FUNCTION

	for (int i = pos; i < _dataLen; ++i)
	{
		if (_pData[i] == '\n')
		{
			return ++i;
		}
	}
	return -1;
	__LEAVE_FUNCTION
	return -1;
}

void IniReader::addSect(const char* pSect)
{
	__ENTER_FUNCTION

	//检测
	if (NULL == pSect)
	{
		AssertEx(0, "sect error!");
	}
	if (str_util::strlen(pSect) > INI_SECT_LEN - 8)
	{
		AssertEx(0, "sect too long!");
		return;
	}
	//段存在则直接返回
	if (_findSect(pSect) != -1)
	{
		return;
	}
	
	char sectBuffer[INI_SECT_LEN] = {0};
	memset(sectBuffer, 0, sizeof(sectBuffer));
	sectBuffer[0] = '[';
	str_util::strncpy(&sectBuffer[1], pSect, INI_SECT_LEN - 2);
	sectBuffer[str_util::strlen(pSect) + 1] = ']';
	
	int incLen = (int)str_util::strlen(sectBuffer);
	char* pNewData = NULL;
	//文件结尾没有空行则添加空行
	if (_pData[_dataLen - 1] != '\n')
	{
		pNewData = new char[_dataLen + incLen + 2];
		Assert(pNewData != NULL);
		
		//拷贝原来的数据
		memcpy(pNewData, _pData, _dataLen);
		
		//添加回车换行
		pNewData[_dataLen] = '\r';
		pNewData[_dataLen + 1] = '\n';

		//添加新段
		memcpy(&pNewData[_dataLen + 2], sectBuffer, incLen);

		//修改文件长度
		_dataLen += incLen + 2;
	}
	else
	{
		pNewData = new char[_dataLen + incLen + 2];
		Assert(pNewData != NULL);

		//拷贝原来的数据
		memcpy(pNewData, _pData, _dataLen);

		//添加新段
		memcpy(&pNewData[_dataLen], sectBuffer, incLen);

		//修改文件长度
		_dataLen += incLen;
	}

	//干掉原来的内存块
	SAFE_DELETE_ARRAY(_pData);
	_pData = pNewData;

	//重新构建段索引
	_initSectIndex();
	__LEAVE_FUNCTION
}

bool IniReader::_getValue(int begin, char* pValue, int len) const
{
	__ENTER_FUNCTION
	Assert(begin>0 && begin<_dataLen);
	Assert(pValue != NULL);

	bool flag = false;
	memset(pValue, 0, sizeof(char) * len);
	int os = 0;

	for (int i = begin; i < _dataLen; ++i)
	{
		//遍历到等号，置标记，后面的内容要缓存
		if (_pData[i] == '=')
		{
			flag = true;
			continue;
		}

		//遍历到换行符跳出
		if (_pData[i] == '\n' || _pData[i] == '\r' || _pData[i] == ';')
		{
			break;
		}
		if (flag)
		{
			//缓存
			pValue[os] = _pData[i];
			++os;
		}
	}

	//去除空格
	str_util::trim(pValue);
	return str_util::strlen(pValue)>0;

	__LEAVE_FUNCTION
	return false;
}

void IniReader::_initSectIndex()
{
	__ENTER_FUNCTION
	Assert(_dataLen>0);
	
	//计算段数目
	_sectCount = 0;
	for (int i = 0; i < _dataLen; ++i)
	{
		if (_pData[i] == '[')
		{
			if (i == 0 || _pData[i - 1]=='\n')
			{
				++_sectCount;
			}
		}
	}

	if (_sectCount > 0)
	{
		//存在则干掉
		if (_pSectIndexArr != NULL)
		{
			SAFE_DELETE_ARRAY(_pSectIndexArr);
		}

		//分配索引数组空间
		_pSectIndexArr = new int[_sectCount];
		int pos = 0;
		//初始化
		for (int i = 0; i < _dataLen; ++i)
		{
			if (_pData[i] == '[')
			{
				if (i == 0 || _pData[i-1] == '\n')
				{
					_pSectIndexArr[pos] = i + 1;
					++pos;
				}
			}
		}
	}
	__LEAVE_FUNCTION
}

} //namespace file_parser
