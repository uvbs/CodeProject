////////////////////////////////////////////////////////////////////////////////////
// 
// @file: DBResult.h
// @author: by Mr.Chen
// @date: 2014/11/9	14:40
// @brief: ½á¹û¼¯
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __DBRESULT_H_
#define __DBRESULT_H_

#include "mysqltype.h"

class CDBResult
{
friend class CDataBase;
public:
	CDBResult();
	~CDBResult();

	bool              next();
	int			        getFieldAsInt(const int fieldIndex) const;
	uint			    getFieldAsUint(const int fieldIndex) const;
	int64			    getFieldAsInt64(const int fieldIndex) const;
	uint64		    getFieldAsUint64(const int fieldIndex) const;
	double		    getFieldAsDouble(const int fieldIndex) const;
	const char*	getFieldAsString(const int fieldIndex) const;
	DBBlob			getFieldAsBlob(const int fieldIndex) const;
	bool			    getFieldAsBlob(const int fieldIndex, DBBlob& blob) const;

private:
	bool			    create(MYSQL_RES* pResult);
	void			    clear();
	void			    release();

private:
	MYSQL_RES*	_pResult;
	char**		    _curRow;
	int			        _curIndex;

#ifdef _WIN32
	int			_rowCount;
	int			_fieldCount;
	uint*		_lengths;

#else
	uint64	_rowCount;
	uint64	_fieldCount;
	uint64*	_lengths;
#endif

};

#endif   //__DBRESULT_H_
