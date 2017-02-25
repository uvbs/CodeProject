////////////////////////////////////////////////////////////////////////////////////
// 
// @file: DBSQLBuilder.h
// @author: by Mr.Chen
// @date: 2014/11/9	15:06
// @brief: SQL语句构建器
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __DBSQLBUILDER_H_
#define __DBSQLBUILDER_H_

#include "mysqltype.h"

class CDBSQLBuilder
{
friend class CDataBase;
public:
    static const int MAX_SQL_LENGTH = 64*1024;

public:
	CDBSQLBuilder();
	~CDBSQLBuilder();

	//bool Execute();

	bool		pushHead(const char* const head);
	bool		push(const int param);
	bool		push(const char* const string, const int length);
	bool		push(void* pData, const int size);
	bool		pushUnsafe(const char* const string);

private:
	
	void		clear();
	void		init(MYSQL* pConnection);
 	const char* getSQL() const { return _sql; }
 	const int	 getSize() const { return _curCharCount; }

private:
	char*		_sql;
	int			_curCharCount;
	//TODO 去掉对Connection的引用
	MYSQL*	_pConnection;
};

#endif   //__DBSQLBUILDER_H_

