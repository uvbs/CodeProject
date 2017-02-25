////////////////////////////////////////////////////////////////////////////////////
// 
// @file: iniReader.h
// @author: by Mr.Chen
// @date: 2014/8/21	10:24
// @brief: INI配置文件
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __INI_READER_H_
#define __INI_READER_H_

#include "type.h"

namespace file_parser {

////////////////////////////////////////////////////////////////////////////////
//	class IniReader
////////////////////////////////////////////////////////////////////////////////
class IniReader
{
	enum
	{
		INI_SECT_LEN	= 256,		//段长
		INI_VALUE_LEN	= 1024,		//值长
		INI_KEY_LEN		= 256,		//键长
		INI_LINE_LEN	= 2048,		//行长
		MAX_FILE_NAME_LENGTH = 1024, 
	};
public:
	IniReader();
	IniReader(const char* file);
	~IniReader();
public:
	// 打开文件
	bool			open(const char* file);
	// 保存文件
	bool			save();
	
	// 读取整数，成功返回键值，失败返回-1
	int				readInt(const char* pSect, const char* pKey) const;
	// 读取浮点数，成功返回键值，失败返回-1
	float			readFloat(const char* pSect, const char* pKey) const;
	// 读取字符串，键值通过参数pOut获取，返回值表示失败或成功
	bool			readString(const char* pSect, const char* pKey, char* pOut, int len) const;

	// 写入整数
	bool			writeInt(const char* pSect, const char* pKey, int value);
	// 写入浮点数
	bool			writeFloat(const char* pSect, const char* pKey, float value);
	// 写入字符串
	bool			writeString(const char* pSect, const char* pKey, char* pValue);
private:
	/************************************************************************/
	/* 初始化段索引，让段指针数组成员指向各段的内存偏移位置
	/************************************************************************/
	void			_initSectIndex();

	/************************************************************************/
	/* 查找段的偏移位置，失败返回-1
	/************************************************************************/
	int				_findSect(const char* pSect) const;

	/************************************************************************/
	/* 查找键的偏移位置，失败返回-1
	/************************************************************************/
	int				_findKey(int begin, const char* pKey) const;

	/************************************************************************/
	/* 找到下一行的起始位置
	/************************************************************************/
	int				_findNextLineBegin(int currPos) const;

	/************************************************************************/
	/* 获取键对应的值
	/************************************************************************/
	bool			_getValue(int begin, char* pValue, int len) const;

	/************************************************************************/
	/* 通过位置获得其在段位置数组的下标
	/************************************************************************/
	int				_getIndexByPos(int pos) const;

	/************************************************************************/
	/* 获取一行，返回下一行的起始位置
	/************************************************************************/
	int				_getLine(char* pBuffer, int bufferLen, int beginPos) const;

public:
	/************************************************************************/
	/* 添加段
	/************************************************************************/
	void			addSect(const char* pSect);

	/************************************************************************/
	/* 添加键值对
	/************************************************************************/
	bool			addKeyValuePair(const char* pSect, const char* pKey, const char* pValue);

	/************************************************************************/
	/* 修改键值对
	/************************************************************************/
	bool			modifyKeyValuePair(const char* pSect, const char* pKey, const char* pValue);

private:
	//文件名
	char			_filename[MAX_FILE_NAME_LENGTH];
	//文件数据长度
	int				_dataLen;
	//文件数据
	char*			_pData;
	//段在内存的起始位置数组
	int*			_pSectIndexArr;
	//段数量
	int				_sectCount;
};

} //namespace file_parser
using namespace file_parser;

#endif //__INI_READER_H_

