////////////////////////////////////////////////////////////////////////////////////
// 
// @file: DataBase.h
// @author: by Mr.Chen
// @date: 2014/11/9	14:25
// @brief: MySQL数据库操作接口
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __DATABASE_H_
#define __DATABASE_H_

#include "mysqltype.h"
#include "dbresult.h"
#include "dbstatement.h"
#include "dbsqlbuilder.h"

class CDataBase
{
public:
	CDataBase();
	~CDataBase();

	//static CDataBase& GetInstance()
	//{
	//	static CDataBase s_DB;
	//	return s_DB;
	//}

	bool		init(const char* host,const char* user,const char* pwd,const char* db,uint port, const char* encoding="utf8");
	bool		release();
	int		    reconnect();
	bool		isConnected() const { return _isConnected; }
	bool		selectDataBase(const char* const name);
	bool		createDataBase(const char* const name);

	bool		execute(const char* const sql, const int length);
	bool		execute(const CDBStatement& statement);
	bool		execute(const CDBSQLBuilder& sqlBuilder);
	bool		executeUnsafe(const char* const sql);
	
	CDBResult*	getResult();
	int		            getErrNo() {return _errorNO;}
	CDBResult*	callProcedure(const char* const name, const char* const fromat, ...);
		
	CDBStatement*	createStatement(const char* const sqlTemplate);
	CDBSQLBuilder*	createSQLBuilder();

private:
	void		setConnected() { _isConnected = true;  }
	void		setDisconnected() { _isConnected = false; }
	
	bool		clearResults();
	void		clear();
	bool		executeStatement(MYSQL_STMT* pStatement);

private:
	MYSQL*	_pConnection;
	bool		_isConnected;
	int		    _errorNO;

private:
	CDBStatement   _st;
	CDBSQLBuilder  _sqlBuilder;
	CDBResult         _result;
};

#endif   //__DATABASE_H_

