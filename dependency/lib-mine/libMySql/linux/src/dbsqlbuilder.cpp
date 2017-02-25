#ifdef _WIN32
	#include "winsock2.h" 
#endif
#include "mysql.h"
#include "dbsqlbuilder.h"
#include "dbresult.h"
#include "assert.h"
#include "string_util.h"
#include "macrodef.h"

CDBSQLBuilder::CDBSQLBuilder()
{
__ENTER_FUNCTION
    _sql = new char[MAX_SQL_LENGTH];
	clear();
__LEAVE_FUNCTION
}

CDBSQLBuilder::~CDBSQLBuilder()
{
__ENTER_FUNCTION
	SAFE_DELETE_ARRAY(_sql);
	clear();
__LEAVE_FUNCTION
}

void CDBSQLBuilder::clear()
{
__ENTER_FUNCTION
	//memset(m_sql, 0 ,sizeof(m_sql));
	_curCharCount = 0;
	_pConnection = NULL;
__LEAVE_FUNCTION
}

void CDBSQLBuilder::init(MYSQL* pConnection)
{	
__ENTER_FUNCTION
	Assert(pConnection);	
	_pConnection = pConnection;
__LEAVE_FUNCTION
}

bool CDBSQLBuilder::pushHead(const char* const head)
{
__ENTER_FUNCTION
	Assert(head);
	int copiedLength = 0;
	copiedLength = str_util::snprintf(_sql, MAX_SQL_LENGTH, "%s", head);
	if (-1 == copiedLength)
	{
		return false;
	}

	_curCharCount += copiedLength;

	return true;
__LEAVE_FUNCTION
	return false;
}

bool CDBSQLBuilder::push(const char* const str, const int length)
{
__ENTER_FUNCTION
	Assert(str && length > 0 );
	//TODO sprintf执行失败没有处理
	_curCharCount += str_util::snprintf(_sql + _curCharCount, MAX_SQL_LENGTH, "%s", str);
	return true;
__LEAVE_FUNCTION
	return false;
}

bool CDBSQLBuilder::push(const int param)
{
__ENTER_FUNCTION
	//TODO sprintf执行失败没有处理
	_curCharCount += str_util::snprintf(_sql + _curCharCount, MAX_SQL_LENGTH, "%d", param);
	return true;
__LEAVE_FUNCTION
	return false;
}

bool CDBSQLBuilder::push(void* pData, int size)
{
__ENTER_FUNCTION
	Assert(pData && size > 0 );
	//TODO mysql_real_escape_string执行失败没有处理
 	_curCharCount += mysql_real_escape_string(
 		_pConnection, 
 		_sql + _curCharCount, 
 		(char*)pData, 
 		size);

	return true;
__LEAVE_FUNCTION
	return false;
}

bool CDBSQLBuilder::pushUnsafe( const char* const string )
{
	__ENTER_FUNCTION
	return push(string, str_util::strlen(string) + 1);
	__LEAVE_FUNCTION
	return false;
}
