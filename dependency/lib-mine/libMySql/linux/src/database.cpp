#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#ifdef _WIN32
	#include "winsock2.h" 
#endif
#include "mysql.h"
#include "database.h"
#include "dbresult.h"
#include "assert.h"

CDataBase::CDataBase()
{
	__ENTER_FUNCTION
	clear();
	__LEAVE_FUNCTION
}

CDataBase::~CDataBase()
{
	__ENTER_FUNCTION
	if (isConnected())
	{
		release();
	}
	clear();
	__LEAVE_FUNCTION
}

bool CDataBase::release()
{
__ENTER_FUNCTION
	mysql_close(_pConnection);
	setDisconnected();
	return true;
__LEAVE_FUNCTION
	return false;
}

bool CDataBase::selectDataBase(const char* const name)
{
__ENTER_FUNCTION
	Assert(name);
	mysql_select_db(_pConnection, name);
	return true;
__LEAVE_FUNCTION
	return false;
}

bool CDataBase::createDataBase(const char* const name)
{
__ENTER_FUNCTION

	Assert(name);
	CDBSQLBuilder* pBuilder = createSQLBuilder();
	if (!pBuilder)
	{
		return false;
	}
	pBuilder->pushUnsafe("create database");
	pBuilder->pushUnsafe(" ");
	pBuilder->pushUnsafe(name);
	execute(*pBuilder);
	return true;
__LEAVE_FUNCTION
	return false;
}

bool CDataBase::init(const char* host,const char* user,const char* pwd,const char* db, uint port, const char* encoding)
{
__ENTER_FUNCTION

	if (_isConnected || NULL == host ||
		NULL == user || NULL == pwd || NULL == db)
	{
		return false;
	}

	_pConnection = mysql_init(0);
	if (NULL == _pConnection)
	{
		//assert(false);
		return false;
	}

	// enable automatic reconnection 
	bool param = true;
	if(mysql_options(_pConnection, MYSQL_OPT_RECONNECT, &param))
	{
		_errorNO = mysql_errno(_pConnection);
		AssertEx( _errorNO == 0 ,mysql_error(_pConnection));
	}

	MYSQL* pConnection = mysql_real_connect(
		_pConnection, 
		host, 
		user, 
		pwd, 
		db, 
		port, 
		NULL, 
		CLIENT_MULTI_STATEMENTS);

	//assert(NULL != pConnection);
	if (NULL == pConnection || pConnection != _pConnection)
	{
		printf("Failed to connect to database, Error: %s\n", mysql_error(_pConnection)); 
		return false;
	}

	//连接成功
	setConnected();

	//设置编码格式
	if(mysql_set_character_set(_pConnection, encoding))
	{
		printf("New client character set: %s\n", mysql_character_set_name(_pConnection));
		return false;
	}

	//TODO 测试代码
	//bool bRet = SelectDataBase("HuLu");
	//Assert(bRet);
	
	return true;
__LEAVE_FUNCTION
	return false;
}

int CDataBase::reconnect()
{
__ENTER_FUNCTION

	if (NULL == _pConnection)
	{
		return EM_RECONNECT_FAILED;
	}

	unsigned long preTheadID = mysql_thread_id(_pConnection);

	//如果失去连接，使用ping重新连接
	if(mysql_ping(_pConnection))
	{
		_errorNO = mysql_errno(_pConnection);
		AssertEx(_errorNO == 0, mysql_error(_pConnection));
		return EM_RECONNECT_FAILED;
	}

	unsigned long nextThreadID = mysql_thread_id(_pConnection);

	if (preTheadID != nextThreadID)
	{
		printf("reconnect occurs ...");
		return EM_RECONNECT_OK;
	}
	
	return EM_RECONNECT_NONE;
__LEAVE_FUNCTION
	return EM_RECONNECT_FAILED;
}


bool CDataBase::execute(const char* const sql,  const int length)
{
__ENTER_FUNCTION

	Assert(sql && length > 0);
	//如果包含二进制内容需要使用mysql_real_query()
	int queryFailed = mysql_real_query(_pConnection, sql, strlen(sql));
 	if (queryFailed)
 	{
 		_errorNO = mysql_errno(_pConnection);
		AssertEx( _errorNO == 0 ,mysql_error(_pConnection));
 	}

	return true;
__LEAVE_FUNCTION
	return false;
}

bool CDataBase::execute(const CDBSQLBuilder& sqlBuilder)
{
__ENTER_FUNCTION
	return execute(sqlBuilder.getSQL(), sqlBuilder.getSize());
__LEAVE_FUNCTION
	return false;
}

bool CDataBase::execute(const CDBStatement& statement)
{
__ENTER_FUNCTION

	my_bool bindFailed = mysql_stmt_bind_param(statement._pStatement, const_cast<MYSQL_BIND*>(statement._params));
	if (bindFailed)
	{
		_errorNO = mysql_errno(_pConnection);
		AssertEx( _errorNO == 0 ,mysql_error(_pConnection));
		const char* errmsg = mysql_stmt_error(statement._pStatement);
		if (errmsg)
		{
			printf("mysql_stmt_bind_param error: %s", errmsg);
		}
	}
	return executeStatement(statement._pStatement);
__LEAVE_FUNCTION
	return false;
}

bool CDataBase::executeUnsafe(const char* const sql)
{
__ENTER_FUNCTION
	return execute(sql, strlen(sql));
__LEAVE_FUNCTION
	return false;
}

CDBResult* CDataBase::callProcedure(const char* const name, const char* const format, ...)
{
__ENTER_FUNCTION
	Assert(name && format);
	char tempMessage[2048] = { 0 };
	char args[1024] = { 0 };

	va_list vaArgList;
	va_start(vaArgList, format);

	vsnprintf(args, sizeof(args), format, vaArgList);
#ifdef _LINUX64
	snprintf(tempMessage, sizeof(tempMessage),"call %s(%s)", name, args);
#else
	_snprintf(tempMessage, sizeof(tempMessage),"call %s(%s)", name, args);
#endif
	va_end(vaArgList);

	bool bResult = executeUnsafe(tempMessage);
	Assert(bResult);

	return getResult();

__LEAVE_FUNCTION
	return NULL;
}

CDBStatement* CDataBase::createStatement(const char* const sqlTemplate)
{
__ENTER_FUNCTION
	Assert(sqlTemplate);
	//static CDBStatement st;
	_st.init(sqlTemplate, _pConnection);
	return &_st;
__LEAVE_FUNCTION
	return NULL;
}

CDBSQLBuilder* CDataBase::createSQLBuilder()
{
__ENTER_FUNCTION
	//static CDBSQLBuilder s_sqlBuilder;
	_sqlBuilder.init(_pConnection);
	return &_sqlBuilder;
__LEAVE_FUNCTION
	return NULL;
}

bool CDataBase::executeStatement(MYSQL_STMT* pStatement)
{
__ENTER_FUNCTION

    __MYTRY
	{
		Assert(pStatement);
		int queryFailed = mysql_stmt_execute(pStatement);
		if (queryFailed)
		{
			_errorNO = mysql_errno(_pConnection);
			AssertEx( _errorNO == 0 ,mysql_error(_pConnection));
			const char* errmsg = mysql_stmt_error(pStatement);
			//AssertRetFalse(false, errmsg);
			if (errmsg)
			{
				printf("mysql_stmt_execute error: %s", errmsg);	
			}
		}
		mysql_stmt_close(pStatement);
	}
	__MYCATCH
	{
		//mysql_stmt_close(pStatement);
	}

	return true; 
__LEAVE_FUNCTION
	return false;
}

void CDataBase::clear()
{
__ENTER_FUNCTION
	_errorNO = 0;
	_pConnection	= NULL;
	setDisconnected();
__LEAVE_FUNCTION
}

CDBResult* CDataBase::getResult()
{
__ENTER_FUNCTION
	MYSQL_RES* dbResult = mysql_store_result(_pConnection);
	if (NULL == dbResult)
	{
 		int errorNo = mysql_errno(_pConnection);
		Assert( errorNo == 0 );	
		return NULL;
	}

	clearResults();
	//static CDBResult result;
	_result.create(dbResult);

	return &_result;	
__LEAVE_FUNCTION
	return NULL;
}

bool CDataBase::clearResults()
{
__ENTER_FUNCTION
	//取出所有结果集，保证后续SQL的执行不会导致
	//错误2014: (CR_COMMANDS_OUT_OF_SYNC) 命令不同步，你现在不能运行该命令。
	while (1)
	{
		int noMoreResult = mysql_next_result(_pConnection);
		if (noMoreResult  > 0)
		{
			//出错
			//assert(false);
			return false;
		}
		else if (noMoreResult)
		{
			//已经取出了所有结果集
			break;
		}
		else if (!noMoreResult)
		{
			//还有更多的结果集
			continue;
		}
	}
	return true;
__LEAVE_FUNCTION
	return false;
}
