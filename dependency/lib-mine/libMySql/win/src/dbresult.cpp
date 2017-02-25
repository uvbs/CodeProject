#ifdef _WIN32
#include "winsock2.h" 
#endif
#include <stdlib.h>
#include "mysql.h"
#include "dbresult.h"
#include "string_util.h"
#include "assert.h"

CDBResult::CDBResult()
{
__ENTER_FUNCTION
	clear();
__LEAVE_FUNCTION
}

CDBResult::~CDBResult()
{
__ENTER_FUNCTION
	release();
	clear();
__LEAVE_FUNCTION
}

bool CDBResult::create(MYSQL_RES* pResult)
{
__ENTER_FUNCTION
	if (_pResult)
	{
		release();
	}
	clear();
	_pResult		= pResult;
	
#ifdef _WIN32
	_rowCount	= static_cast<int>(mysql_num_rows(pResult));
	_fieldCount	= static_cast<int>(mysql_num_fields(pResult));
#else
	_rowCount	= static_cast<uint64>(mysql_num_rows(pResult));
	_fieldCount	= static_cast<uint64>(mysql_num_fields(pResult));
#endif
	
	return true;
__LEAVE_FUNCTION
	return false;
}

bool CDBResult::next()
{
__ENTER_FUNCTION
	if (!_pResult)
	{
		return false;
	}

	_curRow = mysql_fetch_row(_pResult);

#ifdef _WIN32
	_lengths = reinterpret_cast<uint*>(mysql_fetch_lengths(_pResult));
#else
	_lengths = reinterpret_cast<uint64*>(mysql_fetch_lengths(_pResult));
#endif

	_curIndex++;

	return NULL != _curRow;
__LEAVE_FUNCTION
	return false;
}

int CDBResult::getFieldAsInt(const int fieldIndex) const
{
__ENTER_FUNCTION
	//TODO 下标越界断言
	Assert(fieldIndex >= 0 && fieldIndex < _fieldCount);
	return str_util::strToInt(_curRow[fieldIndex]);
__LEAVE_FUNCTION
	return -1;
}

uint CDBResult::getFieldAsUint(const int fieldIndex) const
{
__ENTER_FUNCTION
	Assert(fieldIndex >= 0 && fieldIndex < _fieldCount);
	return str_util::strToUint(_curRow[fieldIndex]);

__LEAVE_FUNCTION
	return -1;
}

int64	 CDBResult::getFieldAsInt64(const int fieldIndex) const
{
	__ENTER_FUNCTION
	Assert(fieldIndex >= 0 && fieldIndex < _fieldCount);
	return str_util::strToInt64(_curRow[fieldIndex]);
	__LEAVE_FUNCTION
	return -1;
}

uint64	 CDBResult::getFieldAsUint64(const int fieldIndex) const
{
	__ENTER_FUNCTION
	Assert(fieldIndex >= 0 && fieldIndex < _fieldCount);
	return str_util::strToUint64(_curRow[fieldIndex]);
	__LEAVE_FUNCTION
	return -1;
}

double CDBResult::getFieldAsDouble(const int fieldIndex) const
{
__ENTER_FUNCTION
	Assert(fieldIndex >= 0 && fieldIndex < _fieldCount);
	return str_util::strToFloat(_curRow[fieldIndex]);
__LEAVE_FUNCTION
	return 0.0f;
}

const char* CDBResult::getFieldAsString(const int fieldIndex) const
{
__ENTER_FUNCTION
	//TODO 下标越界断言
	Assert(fieldIndex >= 0 && fieldIndex < _fieldCount);
	return _curRow[fieldIndex];
__LEAVE_FUNCTION
	return NULL;
}

// const X_WCHAR*  CDBResult::GetFieldAsUnicodeString(const int fieldIndex) const
// {
// 	__ENTER_FUNCTION
// 	//TODO 下标越界断言
// 	AssertRetNull(fieldIndex >= 0 && fieldIndex < m_FieldCount);
// 	
// 	static X_WCHAR m_Buffer[1024];
// 	memset(m_Buffer,0,sizeof(X_WCHAR)*1024);
// 	X_CTools::UTF8ToWideChar(m_CurrentRow[fieldIndex],m_Buffer,1024);
// 
// 	return m_Buffer;
// 	__LEAVE_FUNCTION
// 		return X_NULL;
// }

DBBlob CDBResult::getFieldAsBlob(const int fieldIndex) const
{
	DBBlob blob = {0};
__ENTER_FUNCTION
	//TODO 下标越界断言
	Assert(fieldIndex >= 0 && fieldIndex < _fieldCount);
	blob.m_Length = _lengths[fieldIndex];
	blob.m_pData = _curRow[fieldIndex];
	return blob;
__LEAVE_FUNCTION
	return blob;
}

bool	CDBResult::getFieldAsBlob(const int fieldIndex, DBBlob& blob) const
{
	__ENTER_FUNCTION
	
	Assert(fieldIndex >= 0 && fieldIndex < _fieldCount);
	blob.m_Length = _lengths[fieldIndex];
	blob.m_pData = _curRow[fieldIndex];
	return true;

	__LEAVE_FUNCTION
	return false;
}

void CDBResult::clear()
{
__ENTER_FUNCTION
	_pResult	= NULL;
	_curRow = NULL;
	_curIndex	= 0;
	_rowCount = 0;
	_fieldCount	= 0;
	_lengths	 = NULL;
__LEAVE_FUNCTION
}

void CDBResult::release()
{
__ENTER_FUNCTION
	mysql_free_result(_pResult);
	_pResult = NULL;
__LEAVE_FUNCTION
}
