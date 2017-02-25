#include "dbc_file.h"
#include <string>
#include <map>
#include <exception>
#include "string_util.h"
#include "macrodef.h"
#include "assert.h"
#include <stdio.h>
#include <string.h>

namespace file_parser
{
template <FIELD_TYPE T>
bool DBCFile::isFieldEQ(const FIELD& a, const FIELD& b)
{
	if (T == T_INT)
	{
		return a.iValue == b.iValue;
	}
	else if (T == T_FLOAT)
	{
		return a.fValue == b.fValue;
	}
	else
	{
		try
		{
			return (str_util::strcmp(a.pString, b.pString) == 0);
		}
		catch(...)
		{
			return false;
		}
	}
}

DBCFile::DBCFile(uint id)
{
	_Id = id;
	_pStringBuf = NULL;
	_indexColum = -1;
}

DBCFile::~DBCFile()
{
	SAFE_DELETE_ARRAY(_pStringBuf);
}

bool DBCFile::openFromMemory(const char* pMemory, const char* pDeadEnd, const char* szFileName)
{
	Assert(pMemory && pDeadEnd);
	//----------------------------------------------------
	//判断是否是二进制格式
	if(pDeadEnd - pMemory >= sizeof(FILE_HEAD) && *((uint*)pMemory) == 0XDDBBCC00)
	{
		return openFromMemoryImplBinary(pMemory, pDeadEnd, szFileName);
	}
	else
	{
		return openFromMemoryImplText(pMemory, pDeadEnd, szFileName);
	}
}

bool DBCFile::openFromMemoryImplBinary(const char* pMemory, const char* pDeadEnd, const char* szFileName)
{
	register const char* pMem = pMemory;

	//----------------------------------------------------
	//Read Head
	FILE_HEAD theHead;
	memcpy(&theHead, pMem, sizeof(FILE_HEAD));
	if (theHead.identify != 0XDDBBCC00 )
	{
		return false;
	}
	//check memory size
	if (sizeof(FILE_HEAD) + 
		sizeof(uint) * theHead.fieldsNum + 
		sizeof(FIELD) * theHead.recordsNum * theHead.fieldsNum +
		theHead.stringBlockSize > (uint16)(pDeadEnd - pMemory))
	{
		return false;
	}

	pMem += sizeof(FILE_HEAD);

	//----------------------------------------------------
	//Init 
	_recordsNum = theHead.recordsNum;
	_fieldsNum  = theHead.fieldsNum;
	_stringBufSize = theHead.stringBlockSize;

	//---------------------------------------------
	//Create String Blok
	_pStringBuf = new char[theHead.stringBlockSize];
	if(!_pStringBuf) 
	{
		return false;
	}
	//------------------------------------------------------
	// Read Field Types
	vector< uint > vFieldType;
	vFieldType.resize(theHead.fieldsNum);
	memcpy(&(vFieldType[0]), pMem, sizeof(uint)*theHead.fieldsNum);
	pMem += sizeof(uint)*theHead.fieldsNum;

	//Check it!
	_fieldType.resize(theHead.fieldsNum);
	for (int i = 0; i<(int)theHead.fieldsNum; i++)
	{
		switch (vFieldType[i])
		{
		case T_INT:
		case T_FLOAT:
		case T_STRING:
			_fieldType[i] = (FIELD_TYPE)vFieldType[i];
			break;

		default:
			SAFE_DELETE_ARRAY(_pStringBuf);
			return false;
		}
	}

	//------------------------------------------------------
	// Read All Field
	_vDataBuf.resize(theHead.recordsNum * theHead.fieldsNum);
	memcpy(&(_vDataBuf[0]), pMem, sizeof(FIELD) * theHead.recordsNum * theHead.fieldsNum);
	pMem += sizeof(FIELD) * theHead.recordsNum * theHead.fieldsNum;

	//------------------------------------------------------
	// Read String Block
	memcpy(_pStringBuf, pMem, _stringBufSize);
	_pStringBuf[_stringBufSize-1]=0;

	//------------------------------------------------------
	// TO runtime address
	for(int j = 0; j < (int)theHead.fieldsNum; j++)
	{
		if(vFieldType[j] != T_STRING) continue;
		for(int i = 0; i < (int)theHead.recordsNum; i++)
		{
			_vDataBuf[i*getFieldsNum()+j].pString += reinterpret_cast<unsigned long long>(_pStringBuf);
		}
	}

	//------------------------------------------------------
	//生成索引列
	createIndex(0, szFileName);

	return true;
}

bool DBCFile::openFromMemoryImplText(const char* pMemory, const char* pDeadEnd, const char* szFileName)
{
	char szLine[1024*10] = {0};
	//读第一行
	register const char* pMem = pMemory;
	pMem = str_util::getLineFromMemory(szLine, 1024*10, pMem, pDeadEnd);
	if(!pMem)
	{
		return false;
	}
	//分析列数和类型
	pMem = str_util::getLineFromMemory(szLine, 1024*10, pMem, pDeadEnd);
	if(!pMem) 
	{
		return false;
	}
	//分解
	vector< std::string > vRet;
	str_util::strToVector(szLine, vRet, "\t", true, true);
	if(vRet.empty()) 
	{
		return false;
	}
	//生成Field Types
	FILEDS_TYPE vFieldsType;
	vFieldsType.resize(vRet.size());
	for(int i = 0; i < (int)vRet.size(); i++) 
	{
		if(vRet[i] == "INT") 
		{
			vFieldsType[i] = T_INT;
		}
		else if(vRet[i] == "FLOAT")
		{
			vFieldsType[i] = T_FLOAT;
		}
		else if(vRet[i] == "STRING")
		{
			vFieldsType[i] = T_STRING;
		}
		else if(vRet[i] == "STRING_INT") 
		{
			vFieldsType[i] = T_STRING; //TODO
		}
		else
		{
			return false;
		}
	}

	//--------------------------------------------------------------
	//初始化
	int recordsNum = 0;
	int fieldsNum = (int)vFieldsType.size();

	//临时字符串区
	vector< std::pair< std::string, int > > vStringBuf;
	//检索表
	std::map< std::string, int > mapStringBuf;

	//--------------------------------------------------------------
	//开始读取
	int stringBufSize = 0;
	do
	{
		//读取一行
		pMem = str_util::getLineFromMemory(szLine, 1024 * 10, pMem, pDeadEnd);
		if (!pMem) 
		{
			break;
		}
		//是否是注释行
		if(szLine[0] == '#') 
		{
			continue;
		}
		//分解
		str_util::strToVector(szLine, vRet, "\t", true, false);
		//列数不对
		if(vRet.empty())
		{
			continue;
		}
        if(vRet.size() != fieldsNum) 
		{
			//补上空格
			if((int)vRet.size() < fieldsNum)
			{
				int subNum = fieldsNum - (int)vRet.size();
				for (int i = 0; i < subNum; i++)
				{
					vRet.push_back("");
				}
			}
		}

		//第一列不能为空
		if(vRet[0].empty())
		{
			continue;
		}
		for (register int i = 0; i < fieldsNum; i++)
		{
			FIELD newField;
			switch(vFieldsType[i])
			{
			case T_INT:
				{
					newField.iValue = str_util::strToInt(vRet[i].c_str());
					_vDataBuf.push_back(newField);
				}
				break;

			case T_FLOAT:
				{
					newField.fValue = str_util::strToFloat(vRet[i].c_str());
					_vDataBuf.push_back(newField);
				}
				break;

			case T_STRING:
				if(vRet[i].empty())
				{
					newField.iValue = 0;
				}
				else
				{
					//const char * p = vRet[i].c_str();
					std::map< std::string, int >::iterator it = mapStringBuf.find(vRet[i]);
					if(it == mapStringBuf.end())
					{
						vStringBuf.push_back(std::make_pair(vRet[i], stringBufSize));
						mapStringBuf.insert(std::make_pair(vRet[i], (int)vStringBuf.size() - 1));
						newField.iValue = stringBufSize + 1; // first CHAR is '\0' for blank string
	
						stringBufSize += str_util::strlen(vRet[i].c_str()) + 1;
					}
					else
					{
						newField.iValue = vStringBuf[it->second].second + 1;
					}
				}

				_vDataBuf.push_back(newField);
				break;
			}
		}

		recordsNum++;
	}while(true);

	//--------------------------------------------------------
	//生成正式数据库
	_recordsNum = recordsNum;
	_fieldsNum  = fieldsNum;
	_stringBufSize = stringBufSize+1;

	//Create String Blok
	_pStringBuf = new char[_stringBufSize];
	if (!_pStringBuf) 
	{
		return false;
	}
	//------------------------------------------------------
	// Create Field Types
	_fieldType = vFieldsType;

	//------------------------------------------------------
	// Create String Block
	uchar byBlank = '\0';
	_pStringBuf[0] = '\0';

	register char* p = _pStringBuf + 1;
	for(int i = 0; i < (int)vStringBuf.size(); i++)
	{
		memcpy(p, vStringBuf[i].first.c_str(), vStringBuf[i].first.size());	
		p += vStringBuf[i].first.size();
		*(p++) = '\0';
	}

	//------------------------------------------------------
	// Relocate String Block
	for (register int i = 0; i < fieldsNum; i++)
	{
		if (vFieldsType[i] != T_STRING) continue;
		for (register int j = 0; j < recordsNum; j++)
		{
			FIELD& theField = _vDataBuf[j * fieldsNum + i];
			theField.pString = _pStringBuf + theField.iValue;
		}
	}

	//------------------------------------------------------
	//生成索引列
	createIndex(0, szFileName);

	return true;
}

bool DBCFile::openFromTXT(const char* szFileName)
{
	Assert(szFileName);
	//----------------------------------------------------
	//打开文件
	FILE* fp = fopen(szFileName, "rb");
	if(NULL == fp) 
	{
		return false;
	}
	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	//读入内存
	char* pMemory = new char[fileSize + 1];
	fread(pMemory, 1, fileSize, fp);
	pMemory[fileSize] = 0;

	bool bRet = openFromMemory(pMemory, pMemory + fileSize + 1, szFileName);
	SAFE_DELETE_ARRAY(pMemory);
	return bRet;
}

void DBCFile::createIndex(int nColum, const char* szFileName)
{
	if (nColum < 0 || nColum >= _fieldsNum || _indexColum == nColum) 
	{
		return;
	}
	_hashIndex.clear();

	for(int i = 0; i < _recordsNum; i++)
	{
		FIELD* p = &(_vDataBuf[i * _fieldsNum]);
		FIELD_HASHMAP::iterator itFind = _hashIndex.find(p->iValue);
		if (itFind != _hashIndex.end())
		{
			char temp[260];
			str_util::snprintf(temp, 260, "[%s]Multi index at line %d(SAME:value=%d)", szFileName, i+1, p->iValue);
#ifdef GAME_CLIENT
			throw std::string(temp);
#else
			AssertEx(false, temp);
#endif
			return;
		}
		_hashIndex.insert(std::make_pair(p->iValue, p));
	}
}

const FIELD* DBCFile::searchIndexEQ(int iIndex) const
{
	FIELD_HASHMAP::const_iterator itFind = _hashIndex.find(iIndex);
	if (itFind == _hashIndex.end()) 
	{
		return NULL;
	}
	return itFind->second;
}

const FIELD* DBCFile::searchPosition(int recordLine, int columNum) const
{
	int pos = recordLine * getFieldsNum() + columNum;
	if (pos < 0 || pos >= (int)_vDataBuf.size()) 
	{
		char temp[260];
		str_util::snprintf(temp, 260, "DBC:Invalid search request[rec=%d, col=%d]", recordLine, columNum);
#ifdef GAME_CLIENT
		throw std::string(temp);
#else
		AssertEx(false, temp);
#endif
		return NULL;
	}

	return &(_vDataBuf[pos]);
}

//查找某列等于指定值的第一行
const FIELD* DBCFile::searchFirstColumnEQ(int columnNum, const FIELD& value) const
{
	if (columnNum < 0 || columnNum >= _fieldsNum)
	{
		return 0;
	}
	FIELD_TYPE type = _fieldType[columnNum];
	for(register int i = 0; i < _recordsNum; i++)
	{
		const FIELD& theField = _vDataBuf[_fieldsNum * i + columnNum];
		bool bRet = false;
		if (T_INT == type) 
		{
			bRet = isFieldEQ<T_INT>(theField, value);
		}
		else if (T_FLOAT == type)
		{
			bRet = isFieldEQ<T_FLOAT>(theField, value);
		}
		else
		{
			bRet = isFieldEQ<T_STRING>(theField, value);
		}
		if (bRet) 
		{
			return &(_vDataBuf[_fieldsNum * i]);
		}
	}

	return 0;
}

} // namespace file_parser
