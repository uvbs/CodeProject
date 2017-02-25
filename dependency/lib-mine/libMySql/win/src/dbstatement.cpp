#include "dbstatement.h"
#ifdef _WIN32
	#include "winsock2.h" 
#endif
#include "mysql.h"
#include "assert.h"
#include "string_util.h"
#include "macrodef.h"

CDBStatement::CDBStatement()
{
__ENTER_FUNCTION
    _params = new MYSQL_BIND[MAX_STATEMENT_PARAM_COUNT];
	clear();
__LEAVE_FUNCTION
}

CDBStatement::~CDBStatement()
{
__ENTER_FUNCTION
	SAFE_DELETE_ARRAY(_params);	
	_count = 0;
__LEAVE_FUNCTION
}

bool CDBStatement::init(const char* const sql, MYSQL* pConnection)
{
__ENTER_FUNCTION

	clear();
	_pStatement = mysql_stmt_init(pConnection);
	if (NULL == _pStatement)
		return false;

	// TODO strlen(sql)
	int failed = mysql_stmt_prepare(_pStatement, sql, str_util::strlen(sql));
	int count = mysql_stmt_param_count(_pStatement);
	return !failed;
	
__LEAVE_FUNCTION	
	return false;
}

void CDBStatement::clear()
{
__ENTER_FUNCTION

	_pStatement = NULL;
	_count = 0;
	memset(_params,  0, sizeof(_params));
	memset(_lengths, 0, sizeof(_lengths));
	memset(_intData, 0, sizeof(_intData));
	memset(_doubleData, 0, sizeof(_doubleData));
	memset(_floatData, 0, sizeof(_floatData));
	
__LEAVE_FUNCTION
}

bool CDBStatement::push(const char param) 
{
	__ENTER_FUNCTION
	
	memset(&_params[_count], 0, sizeof(_params[_count]));
	_lengths[_count] = sizeof(param);
	_intData[_count] = param;

	_params[_count].buffer = reinterpret_cast<char*>(&_intData[_count]);
	_params[_count].buffer_type = MYSQL_TYPE_TINY;
	_params[_count].length = 0;
	_params[_count].is_null = 0;
	_params[_count].buffer_length = sizeof(param);

	_count++;

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool CDBStatement::push(const uchar param) 
{
	__ENTER_FUNCTION
	
	memset(&_params[_count], 0, sizeof(_params[_count]));
	_lengths[_count] = sizeof(param);
	_intData[_count] = param;

	_params[_count].buffer = reinterpret_cast<char*>(&_intData[_count]);
	_params[_count].buffer_type = MYSQL_TYPE_TINY;
	_params[_count].length = 0;
	_params[_count].is_null = 0;
	_params[_count].is_unsigned = 1;
	_params[_count].buffer_length = sizeof(param);
	_count++;

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool CDBStatement::push(const int16 param) 
{
	__ENTER_FUNCTION
	
	memset(&_params[_count], 0, sizeof(_params[_count]));
	_lengths[_count] = sizeof(param);
	_intData[_count] = param;

	_params[_count].buffer = reinterpret_cast<char*>(&_intData[_count]);
	_params[_count].buffer_type = MYSQL_TYPE_SHORT;
	_params[_count].length = 0;
	_params[_count].is_null = 0;
	_params[_count].buffer_length = sizeof(param);
	_count++;

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool CDBStatement::push(const uint16 param) 
{
	__ENTER_FUNCTION
	
	memset(&_params[_count], 0, sizeof(_params[_count]));
	_lengths[_count] = sizeof(param);
	_intData[_count] = param;

	_params[_count].buffer = reinterpret_cast<char*>(&_intData[_count]);
	_params[_count].buffer_type = MYSQL_TYPE_SHORT;
	_params[_count].length = 0;
	_params[_count].is_null = 0;
	_params[_count].is_unsigned = 1;
	_params[_count].buffer_length = sizeof(param);
	_count++;

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool CDBStatement::push(const int param) 
{
	__ENTER_FUNCTION
	
	memset(&_params[_count], 0, sizeof(_params[_count]));
	_lengths[_count] = sizeof(param);
	_intData[_count] = param;

	_params[_count].buffer = reinterpret_cast<char*>(&_intData[_count]);
	_params[_count].buffer_type = MYSQL_TYPE_LONG;
	_params[_count].length = 0;
	_params[_count].is_null = 0;
	_params[_count].buffer_length = sizeof(param);
	_count++;

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool CDBStatement::push(const uint param) 
{
	__ENTER_FUNCTION
	
	memset(&_params[_count], 0, sizeof(_params[_count]));
	_lengths[_count] = sizeof(param);
	_intData[_count] = param;

	_params[_count].buffer = reinterpret_cast<char*>(&_intData[_count]);
	_params[_count].buffer_type = MYSQL_TYPE_LONG;
	_params[_count].length = 0;
	_params[_count].is_null = 0;
	_params[_count].is_unsigned = 1;
	_params[_count].buffer_length = sizeof(param);
	_count++;

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool CDBStatement::push(const float param) 
{
	__ENTER_FUNCTION
	
	memset(&_params[_count], 0, sizeof(_params[_count]));
	_lengths[_count] = sizeof(param);
	_floatData[_count] = param;

	_params[_count].buffer = reinterpret_cast<char*>(&_floatData[_count]);
	_params[_count].buffer_type = MYSQL_TYPE_FLOAT;
	_params[_count].length = 0;
	_params[_count].is_null = 0;
	_params[_count].buffer_length = sizeof(param);
	_count++;

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool CDBStatement::push(const double param) 
{
	__ENTER_FUNCTION

	memset(&_params[_count], 0, sizeof(_params[_count]));
	_lengths[_count] = sizeof(param);
	_doubleData[_count] = param;

	_params[_count].buffer = reinterpret_cast<char*>(&_doubleData[_count]);
	_params[_count].buffer_type = MYSQL_TYPE_DOUBLE;
	_params[_count].length = 0;
	_params[_count].is_null = 0;
	_params[_count].buffer_length = sizeof(param);
	_count++;

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool CDBStatement::push(const int64 param) 
{
	__ENTER_FUNCTION
	
	memset(&_params[_count], 0, sizeof(_params[_count]));
	_lengths[_count] = sizeof(param);
	_intData[_count] = param;

	_params[_count].buffer = reinterpret_cast<char*>(&_intData[_count]);
	_params[_count].buffer_type = MYSQL_TYPE_LONGLONG;
	_params[_count].length = 0;
	_params[_count].is_null = 0;
	_params[_count].buffer_length = sizeof(param);
	_count++;

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool CDBStatement::push(const uint64 param) 
{
	__ENTER_FUNCTION
	
	memset(&_params[_count], 0, sizeof(_params[_count]));
	_lengths[_count] = sizeof(param);
	_intData[_count] = param;

	_params[_count].buffer = reinterpret_cast<char*>(&_intData[_count]);
	_params[_count].buffer_type = MYSQL_TYPE_LONGLONG;
	_params[_count].length = 0;
	_params[_count].is_null = 0;
	_params[_count].is_unsigned = 1;
	_params[_count].buffer_length = sizeof(param);
	_count++;

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool CDBStatement::push(const char* const str, const int length) 
{
__ENTER_FUNCTION

	Assert(str && length > 0 );
	memset(&_params[_count], 0, sizeof(_params[_count]));
	_lengths[_count] = length;
	
	_params[_count].buffer = const_cast<char*>(str);
	_params[_count].buffer_type = MYSQL_TYPE_VAR_STRING;
	_params[_count].buffer_length = length;
	_params[_count].length = reinterpret_cast<unsigned long*>(&_lengths[_count]);
	_params[_count].is_null = 0;
	_count++;

	return true;
__LEAVE_FUNCTION
	return false;
};

// bool CDBStatement::Push(const X_WCHAR* const str, const int length)
// {
// 	__ENTER_FUNCTION
// 	
// 	AssertRetFalse(str && length > 0 );
// 	memset(&m_Params[m_Count], 0, sizeof(m_Params[m_Count]));
// 	
// 	memset(m_CharData[m_Count],0,sizeof(char));
// 	int iLen = X_CTools::WideByteToUTF8(str,m_CharData[m_Count],MAX_STRING_LEN);
// 
// 	m_Lengths[m_Count] = iLen;
// 	m_Params[m_Count].buffer = const_cast<char*>(m_CharData[m_Count]);
// 	m_Params[m_Count].buffer_type = MYSQL_TYPE_VAR_STRING;
// 	m_Params[m_Count].buffer_length = iLen;
// 	m_Params[m_Count].length = reinterpret_cast<unsigned long*>(&m_Lengths[m_Count]);
// 	m_Params[m_Count].is_null = 0;
// 	m_Count++;
// 	
// 	return true;
// 	__LEAVE_FUNCTION
// 		return false;
// }

bool CDBStatement::pushUnsafe(const char* const string) 
{
__ENTER_FUNCTION
	return push(string, str_util::strlen(string) + 1);
__LEAVE_FUNCTION
	return false;
};

bool CDBStatement::push(void* data, const int size)
{
__ENTER_FUNCTION
	Assert(data && size > 0 );
	memset(&_params[_count], 0, sizeof(_params[_count]));
	_lengths[_count] = size;
	
	_params[_count].buffer = data;
	_params[_count].buffer_type = MYSQL_TYPE_BLOB;
	_params[_count].length = reinterpret_cast<unsigned long*>(&_lengths[_count]);
	_params[_count].is_null = 0;
	_count++;

	return true;
__LEAVE_FUNCTION
	return false;
};

