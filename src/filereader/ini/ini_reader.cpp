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

	//�ж��ļ����Ƿ���Ч
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

	//��ʼ������ʼλ������
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

	//�ҵ��ε���ʼƫ��
	int pos = _findSect(pSect);
	if (pos == -1)
	{
		return false;
	}

	//������һ�У���ֵ���У�
	pos = _findNextLineBegin(pos);
	if (pos == -1)
	{
		return false;
	}

	//���Ҽ�,û�ҵ�
	pos = _findKey(pos, pKey);
	if (pos == -1)
	{
		return false;
	}

	//��ȡֵ
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

	//���Ҷ�
	int pos = _findSect(pSect);
	//���������½�
	if (pos == -1)
	{
		addSect(pSect);
		pos = _findSect(pSect);
		Assert(pos != -1);
	}

	//������һ�У���ֵ���У�
	pos = _findNextLineBegin(pos);
	if (pos == -1)
	{
		return addKeyValuePair(pSect, pKey, pValue);
	}

	//���Ҽ�,û�ҵ������
	pos = _findKey(pos, pKey);
	if (pos == -1)
	{
		return addKeyValuePair(pSect, pKey, pValue);
	}
	//�ҵ����޸�
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
	
	//����λ��
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

	//��ӻ���
	KeyValuePair[0] = '\r';
	KeyValuePair[1] = '\n';

	str_util::strncpy(&KeyValuePair[2], pKey, KeyLen);
	KeyValuePair[KeyLen+2] = '=';
	str_util::strncpy(&KeyValuePair[KeyLen + 3], pValue, ValueLen);

	int IncSize = 2 + KeyLen + 1 + ValueLen;
	
	char* pNewData = new char[_dataLen+IncSize];
	Assert(pNewData!=NULL);

	//����ԭ�������ݵ�ǰ��
	if (insertPos > 0)
	{
		memcpy(pNewData, _pData, insertPos);
	}
	
	//�����ֵ��
	memcpy(&pNewData[insertPos], KeyValuePair, IncSize);
	
	//����ԭ���ݵĺ��
	if (_dataLen - insertPos > 0)
	{
		memcpy(&pNewData[insertPos + IncSize], &pNewData[insertPos], _dataLen - insertPos);
	}

	//�ɵ�ԭ����
	SAFE_DELETE_ARRAY(_pData);
	_pData = pNewData;
	_dataLen += IncSize;

	//�ع�����ʼλ������
	_initSectIndex();

	return  true;
	__LEAVE_FUNCTION
	return false;
}

bool IniReader::modifyKeyValuePair(const char* pSect, const char* pKey, const char* pValue)
{
	__ENTER_FUNCTION

	//���Ҷ�
	int pos = _findSect(pSect);
	Assert(pos != -1);

	//������һ�У���ֵ���У�
	pos = _findNextLineBegin(pos);
	Assert(pos != -1);

	//���Ҽ�,û�ҵ������
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

		//���һ��
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

	//����ԭ�������ݵ�ǰ��
	if (pos > 0)
	{
		memcpy(pNewData, _pData, pos);
	}

	//�����¼�ֵ��
	memcpy(&pNewData[pos], keyValuePair, newSize);

	//����ԭ���ݵĺ��
	if (_dataLen - endPos > 0)
	{
		memcpy(&pNewData[pos+newSize], &_pData[endPos], _dataLen - endPos);
	}

	//�ɵ�ԭ����
	SAFE_DELETE_ARRAY(_pData);
	_pData = pNewData;
	_dataLen += incSize;

	//�ع�����ʼλ������
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

	//û�ҵ�
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
	
	//��������
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
		//��ȡһ��
		memset(lineBuffer, 0, sizeof(lineBuffer));
		int nextLineBegin = _getLine(lineBuffer, INI_LINE_LEN, begin);
		
		int lineLen = (int)str_util::strlen(lineBuffer);
		//��������
		if (lineLen == 0)
		{
			begin = nextLineBegin;
			continue;
		}

		//����ע����
		if (lineBuffer[0] == '#')
		{
			begin = nextLineBegin;
			continue;
		}
		
		memset(keyBuffer, 0, sizeof(keyBuffer));
		for (int i = 0; i < lineLen; ++i)
		{
			//���ҵ���һ���ˣ�ʧ��
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
				//ɾ���ո�
				str_util::trim(keyBuffer);
				//�ҵ�Key
				if (str_util::strcmp(keyBuffer, pKey) == 0)
				{
					//����Key�����е���ʼλ��
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

	//���
	if (NULL == pSect)
	{
		AssertEx(0, "sect error!");
	}
	if (str_util::strlen(pSect) > INI_SECT_LEN - 8)
	{
		AssertEx(0, "sect too long!");
		return;
	}
	//�δ�����ֱ�ӷ���
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
	//�ļ���βû�п�������ӿ���
	if (_pData[_dataLen - 1] != '\n')
	{
		pNewData = new char[_dataLen + incLen + 2];
		Assert(pNewData != NULL);
		
		//����ԭ��������
		memcpy(pNewData, _pData, _dataLen);
		
		//��ӻس�����
		pNewData[_dataLen] = '\r';
		pNewData[_dataLen + 1] = '\n';

		//����¶�
		memcpy(&pNewData[_dataLen + 2], sectBuffer, incLen);

		//�޸��ļ�����
		_dataLen += incLen + 2;
	}
	else
	{
		pNewData = new char[_dataLen + incLen + 2];
		Assert(pNewData != NULL);

		//����ԭ��������
		memcpy(pNewData, _pData, _dataLen);

		//����¶�
		memcpy(&pNewData[_dataLen], sectBuffer, incLen);

		//�޸��ļ�����
		_dataLen += incLen;
	}

	//�ɵ�ԭ�����ڴ��
	SAFE_DELETE_ARRAY(_pData);
	_pData = pNewData;

	//���¹���������
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
		//�������Ⱥţ��ñ�ǣ����������Ҫ����
		if (_pData[i] == '=')
		{
			flag = true;
			continue;
		}

		//���������з�����
		if (_pData[i] == '\n' || _pData[i] == '\r' || _pData[i] == ';')
		{
			break;
		}
		if (flag)
		{
			//����
			pValue[os] = _pData[i];
			++os;
		}
	}

	//ȥ���ո�
	str_util::trim(pValue);
	return str_util::strlen(pValue)>0;

	__LEAVE_FUNCTION
	return false;
}

void IniReader::_initSectIndex()
{
	__ENTER_FUNCTION
	Assert(_dataLen>0);
	
	//�������Ŀ
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
		//������ɵ�
		if (_pSectIndexArr != NULL)
		{
			SAFE_DELETE_ARRAY(_pSectIndexArr);
		}

		//������������ռ�
		_pSectIndexArr = new int[_sectCount];
		int pos = 0;
		//��ʼ��
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
