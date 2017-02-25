/*
|==========================================
|	DBC数据库文件类
|		(服务器/客户端通用)
|==========================================
|
|		--------------------
|		|  数据库文件格式  |
|		--------------------
|
|		Offset |	Type  |  Description  
|		-------+----------+------------------
|	Head
|		0X000	  UINT		DBC File Identity， always 0XDDBBCC00
|		0X004	  UINT      Number of records in the file 
|		0X008     UINT      Number of 4-BYTE fields per record
|		0X010     UINT      String block size 
|   FieldType
|		0X014     UINT[FieldNum]   
|							  The type fo fields(0-INT, 1-FLOAT, 2-string)
|   FieldBlock
|				  UINT[FieldNum*RecordNum]
|							  DataBlock
|	StringBlock
|				  CHAR[StringSize]
|							  StringBlock
|
*/
////////////////////////////////////////////////////////////////////////////////////
// 
// @file: DBCFile.h
// @author: by Mr.Chen
// @date: 2014/8/15	09:55
// @brief: 数据库文件 Ref from CYOU.
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __DBC_H_
#define __DBC_H_

#include <vector>
//#ifdef _WIN32
//#include <hash_map>
//#else
//#include <ext/hash_map>
//#endif
#include "type.h"
#include "macrodef.h"

using std::vector;

namespace file_parser 
{
	//字段数据类型
	enum FIELD_TYPE
	{
		T_INT		= 0,	//整数
		T_FLOAT	= 1,	//浮点数
		T_STRING	= 2,	//字符串
	};

	//数据段
	union FIELD
	{
		float		fValue;
		int			iValue;
		uint16	wValue;
		char		cValue;
		const char*	pString;	// Just for runtime!

		//Construct
		FIELD() {}
		FIELD(float value) { fValue = value; }
		FIELD(int value) { iValue = value; }
		FIELD(uint16 value) { wValue = value; }
		FIELD(char value) { cValue = value; }
		FIELD(const char* value) { pString = value; }
	};

	class DBCFile	
	{
	public:
		//文件头
		struct FILE_HEAD
		{
			uint identify;				//标示	0XDDBBCC00
			int	 fieldsNum;			//列数
			int	 recordsNum;			//行数
			int	 stringBlockSize;	//字符串区大小
		};

		//数据库格式描述
		typedef vector<FIELD_TYPE>  FILEDS_TYPE;
		//数据区
		typedef vector<FIELD>  DATA_BUF;

	public:
		//打开文本文件，生成一个数据库
		bool openFromTXT(const char* szFileName);
		//根据内存中的文件打开
		bool openFromMemory(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);
	
	protected:
		//读取文本格式内容
		bool openFromMemoryImplText(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);
		//读取二进制格式内容
		bool openFromMemoryImplBinary(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);

	public:
		//按索引查找(第一列为索引)
		const FIELD* searchIndexEQ(int nValue) const;
		//按照位置查找
		const FIELD* searchPosition(int nRecordLine, int nColumNum) const;
		//查找某列等于指定值的第一行
		const FIELD* searchFirstColumnEQ(int nColumnNum, const FIELD& value) const;

	public:
		//取得ID
		uint getID(void) const				{ return _Id; }
		//取得列数
		int	 getFieldsNum(void) const	    { return _fieldsNum; }
		//取得记录的条数
		int	 getRecordsNum(void) const		{ return _recordsNum; }
		//生成索引列
		void createIndex(int column = 0, const char* filename=0);

	protected:
		typedef HASH_MAP<int, FIELD*> FIELD_HASHMAP;
//#ifdef __SGI_STL_PORT
//		typedef std::hash_map< int, FIELD*>	FIELD_HASHMAP;
//#else
//	#ifdef _WIN32
//		typedef stdext::hash_map< int, FIELD*>	FIELD_HASHMAP;
//	#else
//		typedef __gnu_cxx::hash_map< int,FIELD*>    FIELD_HASHMAP;
//							  // hash_compare <INT, less<INT> > ,
//							   //allocator< pair<const INT, FIELD*> > >
//	#endif
//		
//#endif
		//数据库格式文件名
		uint _Id;
		//数据库格式描述
		FILEDS_TYPE _fieldType;
		//行数
		int	 _recordsNum;
		//列数
		int	 _fieldsNum;
		//数据区
		DATA_BUF _vDataBuf;		//size = m_nRecordsNum*m_nFieldsNum
		//字符串区
		char* _pStringBuf;
		//字符串区大小
		int	 _stringBufSize;
		//索引表
		FIELD_HASHMAP	_hashIndex;
		//索引列
		int	 _indexColum;

	public:
		//比较两个值是否相等
		template < FIELD_TYPE T>
		static bool isFieldEQ(const FIELD& a, const FIELD& b);

	public:
		DBCFile(uint id);
		virtual ~DBCFile();
	};

} // namespace file_parser

#endif // __DBC_H_
