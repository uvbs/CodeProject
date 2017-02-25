////////////////////////////////////////////////////////////////////////////////////
// 
// @file: DBCSystem.h
// @author: by Mr.Chen
// @date: 2014/8/15	10:55
// @brief: 内存数据库系统
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __DATABASE_H_
#define __DATABASE_H_

#include "dbc_file.h"
#include "singleton.h"
#include <vector>
#include <map>
#include <string>
using std::vector;
using std::map;
using std::string;

namespace file_parser {

//----------------------------------------------------------
//数据库
class DBC
{
public:
	//按照索引查找
	const FIELD*	searchIndexEQ(int iIndexValue) const;
	//按照行
	const FIELD*	searchLineNumEQ(int iLineNum) const;
	//查找某列等于指定值的第一行
	const FIELD*	searchFirstColumnEQ(int nColumnNum, const FIELD& value) const;
	//按照位置
	const FIELD* searchPosition(int nRecordLine, int nColumNum) const;

public:
	const DBCFile* getDBCFile(void) const { return _pDBCFile; }
	//取得ID
	int	 getID(void) const;
	//取得列数
	uint getFieldsNum(void) const;
	//取得记录的条数
	uint getRecordsNum(void) const;
	//打开文本文件，生成一个数据库
	bool openFromTXT(const char* szFileName);

public:
	DBC(int id);
	virtual ~DBC();

protected:
	DBCFile* _pDBCFile;
};

//============================================================================
// 所有需要打开的数据库定义
struct _DATABASE_DEFINE
{
	int	 nFileID;					//文件的ID
	const char* pFileName;	//文件名字
};

class DBCSystem : public Singleton<DBCSystem>
{
public:
	//打开系统中所有数据库
	void			openDatabases(_DATABASE_DEFINE* dbFileList, int nFileListCount);
	//关闭系统中所有数据库
	void			closeDatabases();

	//得到一个已经打开的数据库，返回数据库指针
	const DBC*	getDatabase(int idDataBase) const;
	const DBC*	getDatabase(std::string& tableName) const;

private:
	//数据库
	std::map<int, DBC*> _mapDatabaseBuf;
	std::map<std::string, int> _mapNameToID;

};

} // namespace file_parser
using namespace file_parser;

#endif	// __DATABASE_H_

