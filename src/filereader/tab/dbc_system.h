////////////////////////////////////////////////////////////////////////////////////
// 
// @file: DBCSystem.h
// @author: by Mr.Chen
// @date: 2014/8/15	10:55
// @brief: �ڴ����ݿ�ϵͳ
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
//���ݿ�
class DBC
{
public:
	//������������
	const FIELD*	searchIndexEQ(int iIndexValue) const;
	//������
	const FIELD*	searchLineNumEQ(int iLineNum) const;
	//����ĳ�е���ָ��ֵ�ĵ�һ��
	const FIELD*	searchFirstColumnEQ(int nColumnNum, const FIELD& value) const;
	//����λ��
	const FIELD* searchPosition(int nRecordLine, int nColumNum) const;

public:
	const DBCFile* getDBCFile(void) const { return _pDBCFile; }
	//ȡ��ID
	int	 getID(void) const;
	//ȡ������
	uint getFieldsNum(void) const;
	//ȡ�ü�¼������
	uint getRecordsNum(void) const;
	//���ı��ļ�������һ�����ݿ�
	bool openFromTXT(const char* szFileName);

public:
	DBC(int id);
	virtual ~DBC();

protected:
	DBCFile* _pDBCFile;
};

//============================================================================
// ������Ҫ�򿪵����ݿⶨ��
struct _DATABASE_DEFINE
{
	int	 nFileID;					//�ļ���ID
	const char* pFileName;	//�ļ�����
};

class DBCSystem : public Singleton<DBCSystem>
{
public:
	//��ϵͳ���������ݿ�
	void			openDatabases(_DATABASE_DEFINE* dbFileList, int nFileListCount);
	//�ر�ϵͳ���������ݿ�
	void			closeDatabases();

	//�õ�һ���Ѿ��򿪵����ݿ⣬�������ݿ�ָ��
	const DBC*	getDatabase(int idDataBase) const;
	const DBC*	getDatabase(std::string& tableName) const;

private:
	//���ݿ�
	std::map<int, DBC*> _mapDatabaseBuf;
	std::map<std::string, int> _mapNameToID;

};

} // namespace file_parser
using namespace file_parser;

#endif	// __DATABASE_H_

