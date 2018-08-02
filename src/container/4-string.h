////////////////////////////////////////////////////////////////////////////////////
// 
// @file: 4-string.h
// @author: by Mr.Chen
// @date: 2014/9/19	13:57
// @brief: 字符串封装类
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __STRING_
#define __STRING_

#include "myassert.h"
#include "macrodef.h"
#include "string_util.h"
#include <string.h>
#include <istream>
#include <ostream>
using std::istream;
using std::ostream;

class String
{
public:
	String() {_size=0; _buffer=NULL;}
	String(const char* str) 
	{
		if (!str)  { _size=0; _buffer=NULL; }
		else 
		{
			_size = str_util::strlen(str);
			_buffer = new char[_size + 1];
			str_util::strncpy(_buffer,str,_size);
			_buffer[_size] = 0;
		}
	}
	String(const String& rhs) 
	{ 
		_size = rhs._size;
		if (!rhs._buffer) { _buffer = 0; }
		else
		{
			_buffer = new char[_size + 1];
			str_util::strncpy(_buffer, rhs._buffer, _size);
			_buffer[_size] = 0;
		}
	}
	~String() {SAFE_DELETE_ARRAY(_buffer);}

	String& operator = (const String&);
	String& operator = (const char*);
	bool operator == (const String&);
	bool operator == (const char*);
	String& operator += (const String&);
	String& operator += (const char*);

	char& operator[](int);
	int size() const { return _size; }
	const char* c_str() const { return _buffer; }

	//从位置offset处开始查找目标串str
	int find(String& str, int offset);
	int find_kmp(String& str, int offset);
	int *get_next();

private:
	int _size;
	char *_buffer;
};

////////////////////////全局函数////////////////////////
inline istream& operator>>(istream &is, String &s)
{
	const int limit_string_size=4096;
	char inBuf[limit_string_size];
	is/*>>setw(limit_string_size)*/>>inBuf;
	s=inBuf;
	return is;
}
inline ostream& operator<<(ostream &os, String &s)
{
	return os<<s.c_str();
}
bool operator==(const String &str1, const String &str2);
bool operator==(const String&str, const char *s);

#endif //__STRING_
