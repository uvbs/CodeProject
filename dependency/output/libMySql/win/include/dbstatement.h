////////////////////////////////////////////////////////////////////////////////////
// 
// @file: DBStatement.h
// @author: by Mr.Chen
// @date: 2014/11/9	15:19
// @brief: SQLÓï¾ä·â×°
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __DBSTATEMENT_H_
#define __DBSTATEMENT_H_

#include "mysqltype.h"

class CDBStatement
{
friend class CDataBase;
public:
    static const int MAX_STATEMENT_PARAM_COUNT =256; 
    static const int MAX_STRING_LEN = 1024;

public:
	CDBStatement();
	~CDBStatement();

	bool		push (const char param);
	bool		push (const uchar param);

	bool		push (const int16 param);
	bool		push (const uint16 param);

	bool		push (const int param);
	bool		push	(const uint param);

	bool		push (const float param);
	bool		push	(const double param);

	bool		push	(const int64 param);
	bool		push	(const uint64 param);

	bool		push	(const char* const string, const int length);

	bool		push	(void* data, const int size);
	bool		pushUnsafe (const char* const string);

private:	
	bool		init (const char* const sql, MYSQL* pConnection);
	void		clear ();

private:
	int			            _count;
	MYSQL_STMT*	_pStatement;
	MYSQL_BIND*	_params;

#ifdef _WIN32
	uint			_lengths[MAX_STATEMENT_PARAM_COUNT];
#else
	uint64		_lengths[MAX_STATEMENT_PARAM_COUNT];
#endif

	int64			_intData[MAX_STATEMENT_PARAM_COUNT];
	double		_doubleData[MAX_STATEMENT_PARAM_COUNT];
	float			_floatData[MAX_STATEMENT_PARAM_COUNT];
	//char			m_CharData[MAX_STATEMENT_PARAM_COUNT][MAX_STRING_LEN];
};

#endif   //__DBSTATEMENT_H_
