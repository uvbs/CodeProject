#include <sys/stat.h>
#include <stdio.h>
#include "serializer_table.h"
#include "string_util.h"
#include <string.h> //memset

namespace file_parser { 

TableSerializer::StringPool		TableSerializer::_s_StringPool;
TableSerializer::IntArrayPool		TableSerializer::_s_IntArrayPool;
TableSerializer::FloatArrayPool	TableSerializer::_s_FloatArrayPool;

char TableSerializer::_s_AnsiFileName[260] = {0};

char* g_ColumnTypeToString[] = 
{
	"INVALID",
	"INT",
	"UINT",
	"INT64",
	"UINT64",
	"FLOAT",
	"STRING",
	"STRING_INT",
	"STRING_FLOAT",
	"INT",
};

char* columnIndexToChar(const int index);

TableSerializer::TableSerializer()
{
	clear();
}

TableSerializer::~TableSerializer() 
{
	SAFE_DELETE_ARRAY(_pReadBuf);
	clear();
}

void TableSerializer::clear()
{
	_bHasReadBuf = false;
	_pCurr = NULL;
	_pReadBuf = NULL;
	_pFileEnd = NULL;
	_pLineEnd = NULL;
	_pFile = NULL;
	_nColumnCount	= 0;
	for (int i = 0; i < MAX_COLUMN_COUNT; ++i)
	{
		_nColumnsType[i] = EM_TYPE_COLUMN_INVALID;
	}
	_cReplaceChar	= 0;
	_nColumnOfCurrLine = 0;
	_bCheckColumn = false;
}

void TableSerializer::openRead(const char* fileName)
{
	strcpy_s(_s_AnsiFileName, fileName, sizeof(_s_AnsiFileName));
	_pFile = ::fopen(_s_AnsiFileName, "rb");
	if (NULL == _pFile)
	{
		return;
	}
	fseek(_pFile, 0, SEEK_SET);
	uchar bom[2] = { 0 };
	fread(bom, sizeof(uchar), 2, _pFile);
	if ((bom[0] == 0xFF && bom[1] == 0xFE) || (bom[0] == 0xFE && bom[1] == 0xFF))
	{
		//return true;
	}
	else
	{
		fseek(_pFile, 0, SEEK_SET);
	}
}

void TableSerializer::set(int& value)
{	
	_checkColumnType(EM_TYPE_COLUMN_INT);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
	{
		return;
	}
	value = str_util::strToL(pBegin, NULL, 10);
	endParseLine();
}

void TableSerializer::set(uint& value)
{	
	_checkColumnType(EM_TYPE_COLUMN_UINT);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
	{
		return;
	}
	value = str_util::strToUL(pBegin, NULL, 10);
	endParseLine();
}

void TableSerializer::set(int64& value)
{
	_checkColumnType(EM_TYPE_COLUMN_INT64);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
	{
		return;
	}
	value = str_util::strToInt64(pBegin, NULL, 10);
	endParseLine();
}

void TableSerializer::set(uint64& value)
{
	_checkColumnType(EM_TYPE_COLUMN_UINT64);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
	{
		return;
	}
	value = str_util::strToUint64(pBegin, NULL, 10);
	endParseLine();
}

void TableSerializer::set(float& value)
{
	_checkColumnType(EM_TYPE_COLUMN_FLOAT);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
	{
		return;
	}
	value = static_cast<float>(str_util::strToDouble(pBegin, NULL));
	endParseLine();
}

void TableSerializer::set(char*& value)
{
	_checkColumnType(EM_TYPE_COLUMN_STRING);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
	{
		return;
	}
	//������ʱ�����ַ���
	const int64 maxSize = _pCurr - pBegin + 1;
	value = new char[maxSize];
	str_util::strncpy(value, pBegin, maxSize - 1);
	value[maxSize - 1] = 0;
	_s_StringPool.add(value);
	endParseLine();
}

void TableSerializer::set(IntArray& value)
{
	_checkColumnType(EM_TYPE_COLUMN_INT_ARRAY);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
	{
		return;
	}
	if (true == value.isValid())
	{
		Assert(0);
		return;
	}
	//������ʱ�����ַ���
	const int64 maxSize = _pCurr - pBegin + 1;
	if (maxSize <= 0)
	{
		Assert(0);
		return;
	}
	char* wholeString = new char[maxSize];
	memset(wholeString, 0, sizeof(char)*maxSize);
	str_util::strncpy(wholeString, pBegin, maxSize-1);
	//���ж��Ƿ���-1��-1�Ͳ�������
	if (str_util::strcmp(wholeString, "-1") == 0)
	{
		SAFE_DELETE_ARRAY(wholeString);
		endParseLine();
		return;
	}
	//ͳ�Ʒָ������
	int seperatorCount = 0;
	for (int i = 0; i < maxSize; ++i)
	{
		if (wholeString[i] == '|')
		{
			++seperatorCount;
		}
	}
	int *pArray = value.init(seperatorCount + 1);
	_s_IntArrayPool.add(pArray);
	char	seperator[] = "|";
	char*	token = NULL;
	char*	tokenContext	= NULL;

	token = str_util::strtok_s(wholeString, seperator, &tokenContext);
	int tokenCount = 0;
	while (token != NULL)
	{
		Assert(tokenCount < value._nCount);
		pArray[tokenCount] = str_util::strToL(token, NULL, 10);
		token = str_util::strtok_s(NULL, seperator, &tokenContext);
		tokenCount++;
	}
	SAFE_DELETE_ARRAY(wholeString);
	endParseLine();
}

void TableSerializer::set(FloatArray& value)
{
	_checkColumnType(EM_TYPE_COLUMN_FLOAT_ARRAY);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
	{
		return;
	}
	if (value._pArray != NULL)
	{
		Assert(0);
		return;
	}
	//������ʱ�����ַ���
	const int64 maxSize = _pCurr - pBegin + 1;
	if (maxSize <= 0)
	{
		Assert(0);
		return;
	}
	char* wholeString = new char[maxSize];
	memset(wholeString, 0, sizeof(char) * maxSize);
	str_util::strncpy(wholeString, pBegin, maxSize - 1);
	//ͳ�Ʒָ������
	int seperatorCount = 0;
	for (int i = 0; i < maxSize; ++i)
	{
		if (wholeString[i] == '|')
		{
			++seperatorCount;
		}
	}
	value._pArray = new float[seperatorCount + 1];
	_s_FloatArrayPool.add(value._pArray);
	value._nCount = seperatorCount + 1;
	char	seperator[]	 = "|";
	char*	token = NULL;
	char*	tokenContext	= NULL;
	
	token = str_util::strtok_s(wholeString, seperator, &tokenContext);
	int tokenCount = 0;
	while (token != NULL)
	{
		Assert(tokenCount < value._nCount);
		value._pArray[tokenCount] = static_cast<float>(str_util::strToDouble(token, NULL));
		token = str_util::strtok_s(NULL, seperator, &tokenContext);
		tokenCount++;
	}
	SAFE_DELETE_ARRAY(wholeString);
	endParseLine();
}

bool TableSerializer::isSeparator(char c)
{
	return (c == '\t') || (c == '\r') || (c == '\n') || c == 0;
}

char* columnIndexToChar(const int index)
{
	static char c[3] = { 0 };
	memset(c, 0, sizeof(c));
	if (index >= 26)
	{
		c[0] = 'A' + index / 26 - 1;
		c[1] = 'A' + index % 26;
	}
	else
	{
		c[0] = 'A' + index;
	}
	return c;
}

int fileLineCount(FILE* file);

int TableSerializer::lineCount() const
{
	return fileLineCount(_pFile);
}

void TableSerializer::skipHeader()
{
	prepareRead();

	//ָ���һ�У��е�����
	if (!nextLine())
	{
		Assert(false);
		return;
	}
	//ָ��ڶ��У��е�����
	if (!nextLine())
	{
		Assert(false);
		return;
	}
	//m_IsFirstColumn = true;

	//ͳ���е���Ϣ������
	//TODO Ӧ���ڶ�ȡ��һ����ʵ���ݵ�ʱ��ͳ��columnCount
	const int MAX_TYPE_STRING_LIST_LENGTH = MAX_COLUMN_COUNT * sizeof("STRING_INT");
	char typeString[MAX_TYPE_STRING_LIST_LENGTH] = { 0 };
	str_util::strncpy(typeString, _pCurr, _pLineEnd - _pCurr + 1);
	
	char seperator[]		= "\t";
	char* token			= NULL;
	char* token_context	= NULL;
	token = str_util::strtok_s(typeString, seperator, &token_context);
	_nColumnCount = 0;
	while (token != NULL)
	{
		trimRight(token);
		_nColumnsType[_nColumnCount] = _parseColumnType(token);
		token = str_util::strtok_s(NULL, seperator, &token_context);
		_nColumnCount++;
	}
}

char* FindLineEnd(const char* pString, const char* rightMost);

bool TableSerializer::nextLine()
{
	prepareRead();
	_nColumnOfCurrLine = 0;
	
	if (NULL == _pLineEnd)
	{
		_pLineEnd = FindLineEnd(_pCurr, _pFileEnd);
	}
	else
	{
		if (_pLineEnd < _pFileEnd)
		{
			if (*(_pLineEnd + 1) == '\r')
			{
				_pCurr = _pLineEnd + 2 + 1; // +2 ������"\r\n", +1 ���������һ���ַ�����������һ�п�ʼ��
			}
			else
			{
				_pCurr = _pLineEnd + 1 + 1; // +2 ������"\r\n", +1 ���������һ���ַ�����������һ�п�ʼ��
			}
			_pLineEnd = FindLineEnd(_pCurr, _pFileEnd);
		}
		else
		{
			return false;
		}
	}

	if (*_pCurr == '#' || *_pCurr == '\r' || *_pCurr == '\n')
	{
		nextLine();
	}

	//m_IsFirstColumn = false;

	if (_pCurr >= _pLineEnd)
		return false;

	if  (*_pCurr == '\t' || *_pCurr == 0)
	{
		char msg[512] = { 0 };
		str_util::snprintf(msg, 512, "\nthere's empty row in table[%s]!", _s_AnsiFileName);
		//Tools::Log(msg);
		return false;
	}
	return true;
}

int FileSize(FILE* file)
{
	if (!file)
		return 0;

	int m_size = 0;
	int oldPosition = ftell(file);
	//������������д���ļ����Ա��ܹ���ȡfileStat
	fflush(file);
	struct stat fileStat;
#ifdef _WIN32
	int result = fstat(_fileno(file), &fileStat);
#endif
#ifdef _LINUX64
	int result = fstat(fileno(file), &fileStat);
#endif
	if( result != 0 )
	{
		Assert(false);
		return 0;
	}
	m_size = fileStat.st_size;
	fseek(file, oldPosition, SEEK_SET);
	Assert(m_size >= 0);
	return m_size;
}

//ʹ��ȫ�ֶ��󣬱���ռ�þֲ�ջ�ռ�
const int MAX_LINE_COUNT_TEMP_STRING_LENGTH = 16*1024;
char lineCountTempString[MAX_LINE_COUNT_TEMP_STRING_LENGTH];

int fileLineCount(FILE* file)
{
	if (!file)
	{
		return 0;
	}
	int oldPosition = ftell(file);
	//TODO �������ᵼ�µ������ݳ���16k��ʱ��ͳ�ƽ������
	//TODO ͳ���������Բ��ø��õ��㷨
	fseek(file, 0, SEEK_SET);
	int lineCount = 0;
	while (fgets(lineCountTempString, MAX_LINE_COUNT_TEMP_STRING_LENGTH, file))
	{
		lineCount++;
	}
	fseek(file, oldPosition, SEEK_SET);

	return lineCount;
}

void TableSerializer::prepareRead()
{
	if (_bHasReadBuf)
	{
		return;
	}
	if (NULL == _pFile)
	{
		Assert(false);
		return;
	}
	SAFE_DELETE_ARRAY(_pReadBuf);

	//�����ļ���С���仺����
	int fileSize = FileSize(_pFile);
	_pReadBuf = new uchar[fileSize + 4];
	memset(_pReadBuf, 0, fileSize + 4);

	//��ȡ�����ļ���������
	int q = ftell(_pFile);
	size_t readSize = fread(_pReadBuf, 1, fileSize, _pFile);
	_pCurr = reinterpret_cast<char*>(_pReadBuf);
	_pFileEnd = reinterpret_cast<char*>(_pReadBuf + readSize - 1);
	_bHasReadBuf = true;
}

////////////////////
//FindLineEnd
//�����ҵĵ�һ�����з���ָ��pNewLine��ǰһ���ַ�
//�����е��ַ�������Ϊm_string��pNewLine - 1
////////////////////
char* FindLineEnd(const char* pString, const char* rightMost)
{
	const char* pNewLine	= pString;
	const char* pEnd	 = rightMost;
	while (pNewLine <= pEnd)
	{
		if (	(*pNewLine == '\r' && *(pNewLine + 1) == '\n' && pNewLine < pEnd)
			||	(*pNewLine == '\n' && pNewLine < pEnd) )
		{
			return const_cast<char*>(pNewLine - 1);
		}
		++pNewLine;
	}
	return const_cast<char*>(pEnd);
}

char*	TableSerializer::beginParseLine()
{
	//���ӵ�ǰ����
	_nColumnOfCurrLine++;
	if (_pCurr > _pLineEnd)
	{
		return NULL;
	}
	char* pBegin = _pCurr;
	while (!isSeparator(*_pCurr))
	{
		_pCurr++;
	}
	_cReplaceChar = *_pCurr;
	*_pCurr = 0;
	return pBegin;
}

void TableSerializer::endParseLine()
{
	*_pCurr = _cReplaceChar;
	//����tab�ָ���
	_pCurr++;
}

void TableSerializer::skipField()
{
	if (NULL == beginParseLine())
	{
		return;
	}
	endParseLine();
}

TableSerializer::EM_TYPE_COLUMN TableSerializer::_parseColumnType(const char* columnType)
{
	if (str_util::strcmp("INT", columnType) == 0)
	{
		return EM_TYPE_COLUMN_INT;
	}
	else if (str_util::strcmp("UINT", columnType) == 0)
	{
		return EM_TYPE_COLUMN_UINT;
	}
	else if (str_util::strcmp("INT64", columnType) == 0)
	{
		return EM_TYPE_COLUMN_INT64;
	}
	else if (str_util::strcmp("UINT64", columnType) == 0)
	{
		return EM_TYPE_COLUMN_UINT64;
	}
	else if (str_util::strcmp("FLOAT", columnType) == 0)
	{
		return EM_TYPE_COLUMN_FLOAT;
	}
	else if (str_util::strcmp("STRING", columnType) == 0)
	{
		return EM_TYPE_COLUMN_STRING;
	}
	else if (str_util::strcmp("STRING_INT", columnType) == 0)
	{
		return EM_TYPE_COLUMN_INT_ARRAY;
	}
	else if (str_util::strcmp("STRING_FLOAT", columnType) == 0)
	{
		return EM_TYPE_COLUMN_FLOAT_ARRAY;
	}
	else if (str_util::strcmp("MONEY", columnType) == 0)
	{
		return EM_TYPE_COLUMN_INT;
	}
	else if (str_util::strcmp("KEY_VALUE", columnType) == 0)
	{
		return EM_TYPE_COLUMN_STRING;
	}
	else
	{
		//Tools::Log("table[%s]invalid column type = %s", s_ansiFileName, columnType);
		return EM_TYPE_COLUMN_INVALID;
	}
}

void TableSerializer::_checkColumnType(EM_TYPE_COLUMN columnType)
{
	if (_bCheckColumn)
	{
		Assert(_nColumnsType[_nColumnOfCurrLine] == columnType);
		if (_nColumnsType[_nColumnOfCurrLine] != columnType)
		{
			/*Log("\ntable [%s], column [%d] (column[%s] in excel) isn't match!\n"
				"in struct TableRow : %s\n"
				"in .tab file : %s\n",
				s_ansiFileName, 
				m_ColumnOfCurrentLine, 
				ColumnIndexToChar(m_ColumnOfCurrentLine),
				g_ColumnTypeToString[columnType],
				g_ColumnTypeToString[m_Columns[m_ColumnOfCurrentLine]]);*/
		}
	}
}

bool TableSerializer::checkColumnCount() const
{
	if (_bCheckColumn && _nColumnOfCurrLine != _nColumnCount)
	{
		char msg[512] = {0};
		str_util::snprintf(
			msg, 512, 
			"\nin table[%s], column count isn't match!\ncolumn count:\nTableRow = %d\n *.tab File = %d", 
			_s_AnsiFileName, _nColumnOfCurrLine, _nColumnCount);
		//Tools::Log(msg);
	}
	return true;
}

} // namespace file_parser

