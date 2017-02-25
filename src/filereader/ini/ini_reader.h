////////////////////////////////////////////////////////////////////////////////////
// 
// @file: iniReader.h
// @author: by Mr.Chen
// @date: 2014/8/21	10:24
// @brief: INI�����ļ�
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
		INI_SECT_LEN	= 256,		//�γ�
		INI_VALUE_LEN	= 1024,		//ֵ��
		INI_KEY_LEN		= 256,		//����
		INI_LINE_LEN	= 2048,		//�г�
		MAX_FILE_NAME_LENGTH = 1024, 
	};
public:
	IniReader();
	IniReader(const char* file);
	~IniReader();
public:
	// ���ļ�
	bool			open(const char* file);
	// �����ļ�
	bool			save();
	
	// ��ȡ�������ɹ����ؼ�ֵ��ʧ�ܷ���-1
	int				readInt(const char* pSect, const char* pKey) const;
	// ��ȡ���������ɹ����ؼ�ֵ��ʧ�ܷ���-1
	float			readFloat(const char* pSect, const char* pKey) const;
	// ��ȡ�ַ�������ֵͨ������pOut��ȡ������ֵ��ʾʧ�ܻ�ɹ�
	bool			readString(const char* pSect, const char* pKey, char* pOut, int len) const;

	// д������
	bool			writeInt(const char* pSect, const char* pKey, int value);
	// д�븡����
	bool			writeFloat(const char* pSect, const char* pKey, float value);
	// д���ַ���
	bool			writeString(const char* pSect, const char* pKey, char* pValue);
private:
	/************************************************************************/
	/* ��ʼ�����������ö�ָ�������Աָ����ε��ڴ�ƫ��λ��
	/************************************************************************/
	void			_initSectIndex();

	/************************************************************************/
	/* ���Ҷε�ƫ��λ�ã�ʧ�ܷ���-1
	/************************************************************************/
	int				_findSect(const char* pSect) const;

	/************************************************************************/
	/* ���Ҽ���ƫ��λ�ã�ʧ�ܷ���-1
	/************************************************************************/
	int				_findKey(int begin, const char* pKey) const;

	/************************************************************************/
	/* �ҵ���һ�е���ʼλ��
	/************************************************************************/
	int				_findNextLineBegin(int currPos) const;

	/************************************************************************/
	/* ��ȡ����Ӧ��ֵ
	/************************************************************************/
	bool			_getValue(int begin, char* pValue, int len) const;

	/************************************************************************/
	/* ͨ��λ�û�����ڶ�λ��������±�
	/************************************************************************/
	int				_getIndexByPos(int pos) const;

	/************************************************************************/
	/* ��ȡһ�У�������һ�е���ʼλ��
	/************************************************************************/
	int				_getLine(char* pBuffer, int bufferLen, int beginPos) const;

public:
	/************************************************************************/
	/* ��Ӷ�
	/************************************************************************/
	void			addSect(const char* pSect);

	/************************************************************************/
	/* ��Ӽ�ֵ��
	/************************************************************************/
	bool			addKeyValuePair(const char* pSect, const char* pKey, const char* pValue);

	/************************************************************************/
	/* �޸ļ�ֵ��
	/************************************************************************/
	bool			modifyKeyValuePair(const char* pSect, const char* pKey, const char* pValue);

private:
	//�ļ���
	char			_filename[MAX_FILE_NAME_LENGTH];
	//�ļ����ݳ���
	int				_dataLen;
	//�ļ�����
	char*			_pData;
	//�����ڴ����ʼλ������
	int*			_pSectIndexArr;
	//������
	int				_sectCount;
};

} //namespace file_parser
using namespace file_parser;

#endif //__INI_READER_H_

