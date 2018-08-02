////////////////////////////////////////////////////////////////////////////////////
// 
// @file: serializer_table.h
// @author: by Mr.Chen
// @date: 2014/5/15	18:45
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef _SERIALIZER_TABLE_H
#define _SERIALIZER_TABLE_H

#include "serializer.h"
#include "myassert.h"
#include "macrodef.h"
#include <stdio.h>

namespace file_parser {

const int MAX_COLUMN_COUNT = 512;
class TableSerializer : public ISerializer
{
public:
	TableSerializer();
	virtual ~TableSerializer();

	//read
	void	openRead(const char* fileName);
	void	skipHeader();	//该接口用于跳过第一行(列名称)和第二行(列类型)
	bool	nextLine();
	int 	lineCount() const;
	bool	checkColumnCount() const;
	
	static void	reload()
	{
		_s_StringPool.reload();
		_s_IntArrayPool.reload();
		_s_FloatArrayPool.reload();
	}
	void	close()
	{
		if(_pFile) { fclose(_pFile); }
	}
	
	TableSerializer& operator << (int&			value)		{ set(value); return *this; }
	TableSerializer& operator << (uint&		value)		{ set(value); return *this; }
	TableSerializer& operator << (int64&		value)		{ set(value); return *this; }
	TableSerializer& operator << (uint64&	value)		{ set(value); return *this; }
	TableSerializer& operator << (float&		value)		{ set(value); return *this; }
	TableSerializer& operator << (char*&		value)		{ set(value); return *this; }
	TableSerializer& operator << (IntArray&	value)		{ set(value); return *this; }
	TableSerializer& operator << (FloatArray& value)		{ set(value); return *this; }

	void skipField();
	void setCheckColumn(bool check) { _bCheckColumn = check; }

private:
	void	set(int&		value);
	void	set(uint&		value);
	void	set(int64&	value);
	void	set(uint64&	value);
	void	set(float& 	value);
	void	set(char*& 	value);
	void	set(IntArray&	value);
	void	set(FloatArray& value);

	bool	isSeparator(char c);
	void	clear();
	void	prepareRead();

	char* beginParseLine();
	void	endParseLine();

private:
	FILE*	_pFile;
	char*	_pCurr;
	
	//列定义信息（从.tab文件中读取的列定义信息，用于与TableRow校对）
	enum EM_TYPE_COLUMN
	{
		EM_TYPE_COLUMN_INVALID=-1,
		EM_TYPE_COLUMN_INT,
		EM_TYPE_COLUMN_UINT,
		EM_TYPE_COLUMN_INT64,
		EM_TYPE_COLUMN_UINT64,
		EM_TYPE_COLUMN_FLOAT,
		EM_TYPE_COLUMN_STRING,
		EM_TYPE_COLUMN_INT_ARRAY,
		EM_TYPE_COLUMN_FLOAT_ARRAY,
		EM_TYPE_COLUMN_NUMBER,
	};

	EM_TYPE_COLUMN _parseColumnType(const char* columnType);
	void _checkColumnType(EM_TYPE_COLUMN columnType);

	EM_TYPE_COLUMN _nColumnsType[MAX_COLUMN_COUNT];
	int	 _nColumnCount;
	int	 _nColumnOfCurrLine;	//当前行第几列
	static char _s_AnsiFileName[260];

	//读取缓冲区
	bool	_bHasReadBuf;
	uchar*_pReadBuf;
	char* _pFileEnd;
	char* _pLineEnd;
	char _cReplaceChar;
	bool _bCheckColumn;

	class StringPool
	{
	public:
		static const int MAX_STRING_POOL_COUNT = 128 * 1024;
		char** _pString;
		int _count;
		StringPool() 
		{ 
			_pString = NULL;
			_count = 0;
		}
		~StringPool() 
		{ 
			for (int i = 0; i < _count; ++i)
			{
				SAFE_DELETE_ARRAY(_pString[i]);
			}
			SAFE_DELETE_ARRAY(_pString);
		}
		void reload()
		{
			for (int i = 0; i < _count; ++i)
			{
				SAFE_DELETE_ARRAY(_pString[i]);
			}
			SAFE_DELETE_ARRAY(_pString);
			_pString = new char*[MAX_STRING_POOL_COUNT]; 
			_count = 0;
		}
		void add(char* pString) 
		{ 
			MyAssert(_count < MAX_STRING_POOL_COUNT); 
			_pString[_count++] = pString; 
		}
	};

	class IntArrayPool
	{
	public:
		static const int MAX_INT_ARRAY_POOL_COUNT = 128 * 1024;
		int** _ppIntArrays;
		int _nCount;
		IntArrayPool()
		{ 
			_ppIntArrays = NULL;
			_nCount = 0;
			/*m_pIntArrays = new int*[MAX_INT_ARRAY_POOL_COUNT]; m_nCount = 0; */
		}
		~IntArrayPool() 
		{ 
			for (int i = 0; i < _nCount; ++i)
			{
				SAFE_DELETE(_ppIntArrays[i]);
			}
			SAFE_DELETE(_ppIntArrays);
		}
		void reload()
		{
			for (int i = 0; i < _nCount; ++i)
			{
				SAFE_DELETE_ARRAY(_ppIntArrays[i]);
			}
			SAFE_DELETE_ARRAY(_ppIntArrays);
			_ppIntArrays = new int*[MAX_INT_ARRAY_POOL_COUNT]; 
			_nCount = 0;
		}
		void add(int* pIntArray) 
		{ 
			MyAssert(_nCount < MAX_INT_ARRAY_POOL_COUNT); 
			_ppIntArrays[_nCount++] = pIntArray; 
		}
	};

	class FloatArrayPool
	{
	public:
		static const int MAX_FLOAT_ARRAY_POOL_COUNT = 128 * 1024;
		float** _ppFloatArrays;
		int	 _nCount;
		FloatArrayPool()
		{ 
			_ppFloatArrays = NULL;
			_nCount = 0;
			/*m_pFloatArrays = new float*[MAX_FLOAT_ARRAY_POOL_COUNT]; m_nCount = 0;*/ 
		}
		~FloatArrayPool() 
		{ 
			for (int i = 0; i < _nCount; ++i)
			{
				SAFE_DELETE_ARRAY(_ppFloatArrays[i]);
			}
			SAFE_DELETE_ARRAY(_ppFloatArrays);
		}
		void reload()
		{
			for (int i = 0; i < _nCount; ++i)
			{
				SAFE_DELETE_ARRAY(_ppFloatArrays[i]);
			}
			SAFE_DELETE_ARRAY(_ppFloatArrays);
			_ppFloatArrays = new float*[MAX_FLOAT_ARRAY_POOL_COUNT]; 
			_nCount = 0;
		}
		void add(float* pFloatArray) 
		{ 
			MyAssert(_nCount < MAX_FLOAT_ARRAY_POOL_COUNT); 
			_ppFloatArrays[_nCount++] = pFloatArray; 
		}
	};

	static StringPool _s_StringPool;
	static IntArrayPool _s_IntArrayPool;
	static FloatArrayPool _s_FloatArrayPool;

}; //class TableSerializer

} // namespace file_parser

#endif //__TABLE_SERIALIZER_H_

