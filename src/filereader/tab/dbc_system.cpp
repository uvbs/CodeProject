#include "socketdef.h"
#include "dbc_system.h"
#include "macrodef.h"
#include "myassert.h"
#include <stdio.h>

WX_IMPLEMENT_SINGLEON(file_parser::DBCSystem);

namespace file_parser {

DBC::DBC(int id)
{
	_pDBCFile = new DBCFile(id);
}

DBC::~DBC()
{
	SAFE_DELETE(_pDBCFile);
}

const FIELD* DBC::searchIndexEQ(int iIndexValue) const 
{
	MyAssert(_pDBCFile);
	return _pDBCFile->searchIndexEQ(iIndexValue);
}

const FIELD* DBC::searchLineNumEQ(int lineNum) const
{
	MyAssert(_pDBCFile);
	return  _pDBCFile->searchPosition(lineNum, 0); 
}

//����ĳ�е���ָ��ֵ�ĵ�һ��
const FIELD* DBC::searchFirstColumnEQ(int columnNum, const FIELD& value) const
{
	MyAssert(_pDBCFile);
	return _pDBCFile->searchFirstColumnEQ(columnNum, value.iValue);
}

const FIELD* DBC::searchPosition(int recordLine, int columNum) const
{
	MyAssert(_pDBCFile);
	return _pDBCFile->searchPosition(recordLine, columNum);
}

//ȡ��ID
int DBC::getID(void) const
{
	MyAssert(_pDBCFile);
	return _pDBCFile->getID(); 
}

//ȡ������
uint DBC::getFieldsNum(void) const
{
	MyAssert(_pDBCFile);
	return _pDBCFile->getFieldsNum(); 
}

//ȡ�ü�¼������
uint DBC::getRecordsNum(void)const
{
	MyAssert(_pDBCFile);
	return _pDBCFile->getRecordsNum(); 
}

bool DBC::openFromTXT(const char* szFileName)
{
	MyAssert(_pDBCFile);
	return _pDBCFile->openFromTXT(szFileName);
}

//============================================================================

void DBCSystem::openDatabases(_DATABASE_DEFINE* dbFileList, int count)
{
	__ENTER_FUNCTION

	// �����������ļ�
	for(int i = 0; i < count; i++)
	{
		//����DBC����
		DBC* pDataBase = new DBC(dbFileList[i].nFileID);
		if (!pDataBase)
		{
			printf("Create database memory failed! File name is %s!\n", dbFileList[i].pFileName);
		}
		//���������ļ�
		if (!(pDataBase->openFromTXT(dbFileList[i].pFileName)))
		{
			printf("Read database file %s is failed!\n", dbFileList[i].pFileName);
		}
		//����Ƿ��ظ�
		if (_mapDatabaseBuf.find(dbFileList[i].nFileID) != _mapDatabaseBuf.end())
		{
			printf("Database file %s is existed!\n", dbFileList[i].pFileName);
		}
		//���������ļ��б�
		_mapDatabaseBuf.insert(std::make_pair(dbFileList[i].nFileID, pDataBase));
		std::string tableName = dbFileList[i].pFileName;
		_mapNameToID.insert(std::make_pair(tableName, dbFileList[i].nFileID));
		printf("Create Database File %s OK!\n", dbFileList[i].pFileName);
	}

	__LEAVE_FUNCTION
}

void DBCSystem::closeDatabases()
{
	__ENTER_FUNCTION

	std::map<int, DBC*>::iterator it;
	for(it = _mapDatabaseBuf.begin(); it!=_mapDatabaseBuf.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
	_mapDatabaseBuf.clear();

	__LEAVE_FUNCTION
}

const DBC* DBCSystem::getDatabase(int idDataBase) const
{
	__ENTER_FUNCTION

	std::map<int, DBC* >::const_iterator it = _mapDatabaseBuf.find(idDataBase);
	if (it != _mapDatabaseBuf.end())
	{
		return (const DBC*)it->second;
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

const DBC* DBCSystem::getDatabase(std::string& tableName) const
{
__ENTER_FUNCTION

	std::map<std::string, int>::const_iterator it = _mapNameToID.find(tableName);
	if (it == _mapNameToID.end())
	{
		return NULL;
	}
	return getDatabase(it->second);

__LEAVE_FUNCTION
	return NULL;
}

} // namespace file_parser
