////////////////////////////////////////////////////////////////////////////////////
// 
// @file: table.h
// @author: by Mr.Chen
// @date: 2014/5/15	17:09
// @brief: tab文件读取器
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

	//加载表格
	bool load(const char* fileName);

	//根据表格的ID索引对应行。表格的ID是指表格中第一列所填的值，不一定从0或1开始，例如某物品表就是从13010001开始的
	bool isContainsRow(const int ID);	
	const T& operator[](const int ID) const { return getRow(ID); } //注意！这里的operator[]返回的是根据ID查找到的Row，而不是根据Index查找到的Row！
	
	//根据表格的row数组下标索引表格,用于某些情况下遍历整张表格
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
	//TODO maxRowCount中没有刨除用"#"注释掉的行
	const int maxRowCount = s.lineCount() - TITLE_ROW_COUNT;
	SAFE_DELETE_ARRAY(_pRows);
	_pRows = new T[maxRowCount];
	_count = 0;
	while (s.nextLine())
	{
		_pRows[_count].mapData(s);
		if (_count > 0 && _pRows[_count] <= _pRows[_count - 1])
		{
			//表格ID的排列不是顺序（可能存在重复的ID），无法保证二分查找
			//Tools::Log( "表格ID的排列不是顺序（可能存在重复的ID）, index = %d", m_Count);
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

