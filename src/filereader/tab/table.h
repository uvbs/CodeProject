////////////////////////////////////////////////////////////////////////////////////
// 
// @file: table.h
// @author: by Mr.Chen
// @date: 2014/5/15	17:09
// @brief: tab�ļ���ȡ��
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __TABLE_H_
#define __TABLE_H_

#include "type.h"
#include "assert.h"
#include "macrodef.h"
#include "serializer_table.h"

namespace file_parser {

template<typename T>
class Table
{
public:
	Table()
	{ 
		_pRows = NULL; 
		_count = 0; 
		_lastIndex = 0; 
	}
	~Table()	{ SAFE_DELETE_ARRAY(_pRows); }

	//���ر��
	bool load(const char* fileName);

	//���ݱ���ID������Ӧ�С�����ID��ָ����е�һ�������ֵ����һ����0��1��ʼ������ĳ��Ʒ����Ǵ�13010001��ʼ��
	bool isContainsRow(const int ID);	
	const T& operator[](const int ID) const { return getRow(ID); } //ע�⣡�����operator[]���ص��Ǹ���ID���ҵ���Row�������Ǹ���Index���ҵ���Row��
	
	//���ݱ���row�����±��������,����ĳЩ����±������ű��
	const T& getRowByIndex(const int index) const { Assert(index >= 0 && index < _count); return _pRows[index]; }

	int	 rowCount() const { return _count; }

private:
	T*	_pRows;
	int	 _count;
	mutable int	 _lastIndex;
	int	 binarySearch(const T* const p, const int startIndex, const int endIndex, const int key) const;
	const T& getRow(const int ID) const;
};

template<typename T>
const T& Table<T>::getRow(const int ID) const
{  
	if (_lastIndex >= 0 
		&& _lastIndex < _count 
		&& _pRows[_lastIndex] == ID)
	{
		return _pRows[_lastIndex];
	}
	int foundIndex = binarySearch(_pRows, 0, _count - 1, ID);
	if (foundIndex < 0)
	{
		static T s_emptyRow = T();
		return s_emptyRow;
		//throw 1;
	}
	_lastIndex = foundIndex;
	return _pRows[foundIndex];
}

template<typename T>
bool Table<T>::isContainsRow(const int ID)
{
	int foundIndex = binarySearch(_pRows, 0, _count - 1, ID);
	if (foundIndex >= 0)
	{
		_lastIndex = foundIndex;
	}
	return foundIndex >= 0;
}

template<typename T>
bool Table<T>::load(const char* fileName)
{
	TableSerializer s;
	s.setCheckColumn(true);
	s.openRead(fileName);
	s.skipHeader();
	const int TITLE_ROW_COUNT = 2;
	//TODO maxRowCount��û���ٳ���"#"ע�͵�����
	const int maxRowCount = s.lineCount() - TITLE_ROW_COUNT;
	SAFE_DELETE_ARRAY(_pRows);
	_pRows = new T[maxRowCount];
	_count = 0;
	while (s.nextLine())
	{
		_pRows[_count].mapData(s);
		if (_count > 0 && _pRows[_count] <= _pRows[_count - 1])
		{
			//���ID�����в���˳�򣨿��ܴ����ظ���ID�����޷���֤���ֲ���
			//Tools::Log( "���ID�����в���˳�򣨿��ܴ����ظ���ID��, index = %d", m_Count);
		}
		if (_count == 0)
		{
			s.checkColumnCount();
		}
		++_count;
	}
	//Tools::Log("Load table %s OK!", fileName);
	s.close();	
	return true;
}

template<typename T>
int Table<T>::binarySearch(const T* const p, const int startIndex, const int endIndex, const int key) const
{
	int start	= startIndex;
	int end = endIndex;
	int middle = (end + start) / 2;
	while (start < end && !(p[middle] == key))
	{
		if (p[middle] < key)
		{
			start = middle + 1;
		}
		else// if (p[middle] < key)
		{
			end = middle - 1;
		}
		middle  = (end + start) / 2;
	}
	return p[middle] == key ? middle : -1;
}

} // namespace file_parser

#endif // __TABLE_H_

