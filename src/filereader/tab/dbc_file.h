/*
|==========================================
|	DBC���ݿ��ļ���
|		(������/�ͻ���ͨ��)
|==========================================
|
|		--------------------
|		|  ���ݿ��ļ���ʽ  |
|		--------------------
|
|		Offset |	Type  |  Description  
|		-------+----------+------------------
|	Head
|		0X000	  UINT		DBC File Identity�� always 0XDDBBCC00
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
// @brief: ���ݿ��ļ� Ref from CYOU.
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
	//�ֶ���������
	enum FIELD_TYPE
	{
		T_INT		= 0,	//����
		T_FLOAT	= 1,	//������
		T_STRING	= 2,	//�ַ���
	};

	//���ݶ�
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
		//�ļ�ͷ
		struct FILE_HEAD
		{
			uint identify;				//��ʾ	0XDDBBCC00
			int	 fieldsNum;			//����
			int	 recordsNum;			//����
			int	 stringBlockSize;	//�ַ�������С
		};

		//���ݿ��ʽ����
		typedef vector<FIELD_TYPE>  FILEDS_TYPE;
		//������
		typedef vector<FIELD>  DATA_BUF;

	public:
		//���ı��ļ�������һ�����ݿ�
		bool openFromTXT(const char* szFileName);
		//�����ڴ��е��ļ���
		bool openFromMemory(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);
	
	protected:
		//��ȡ�ı���ʽ����
		bool openFromMemoryImplText(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);
		//��ȡ�����Ƹ�ʽ����
		bool openFromMemoryImplBinary(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);

	public:
		//����������(��һ��Ϊ����)
		const FIELD* searchIndexEQ(int nValue) const;
		//����λ�ò���
		const FIELD* searchPosition(int nRecordLine, int nColumNum) const;
		//����ĳ�е���ָ��ֵ�ĵ�һ��
		const FIELD* searchFirstColumnEQ(int nColumnNum, const FIELD& value) const;

	public:
		//ȡ��ID
		uint getID(void) const				{ return _Id; }
		//ȡ������
		int	 getFieldsNum(void) const	    { return _fieldsNum; }
		//ȡ�ü�¼������
		int	 getRecordsNum(void) const		{ return _recordsNum; }
		//����������
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
		//���ݿ��ʽ�ļ���
		uint _Id;
		//���ݿ��ʽ����
		FILEDS_TYPE _fieldType;
		//����
		int	 _recordsNum;
		//����
		int	 _fieldsNum;
		//������
		DATA_BUF _vDataBuf;		//size = m_nRecordsNum*m_nFieldsNum
		//�ַ�����
		char* _pStringBuf;
		//�ַ�������С
		int	 _stringBufSize;
		//������
		FIELD_HASHMAP	_hashIndex;
		//������
		int	 _indexColum;

	public:
		//�Ƚ�����ֵ�Ƿ����
		template < FIELD_TYPE T>
		static bool isFieldEQ(const FIELD& a, const FIELD& b);

	public:
		DBCFile(uint id);
		virtual ~DBCFile();
	};

} // namespace file_parser

#endif // __DBC_H_
