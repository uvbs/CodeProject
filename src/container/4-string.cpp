#include "4-string.h"
#include "string_util.h"
#include <iostream>
using std::cout;
using std::endl;

inline String& String::operator =(const char *str)
{
	if (!str)
	{
		_size = 0;
		SAFE_DELETE_ARRAY(_buffer);
	}
	else
	{
		_size = str_util::strlen(str);
		SAFE_DELETE_ARRAY(_buffer);
		_buffer = new char[_size + 1];
		str_util::strncpy(_buffer, str, _size);
		_buffer[_size] = 0;
	}
	return *this;
}

inline String& String::operator =(const String &rhs)
{
	if (this != &rhs)
	{
		SAFE_DELETE_ARRAY(_buffer);
		_size = rhs._size;
		if (!rhs._buffer)
		{
			_buffer = 0;
		}
		else
		{
			_buffer = new char[_size+1];
			str_util::strncpy(_buffer, rhs._buffer, _size);
			_buffer[_size]=0;
		}
	}
	return *this;
}

bool String::operator == (const String &rhs)
{
	if (_size != rhs._size)
	{
		return false;
	}
	return ::strcmp(_buffer, rhs._buffer) == 0;
}

bool String::operator == (const char *s)
{
	return ::strcmp(_buffer, s) == 0;
}

String& String::operator += (const String &rhs)
{
	if (rhs._buffer)
	{
		String temp(*this);
		_size += rhs._size;
		SAFE_DELETE_ARRAY(_buffer);
		_buffer = new char[_size + 1];
		str_util::strcpy(_buffer, temp._buffer);
		str_util::strcpy(_buffer + temp._size, rhs._buffer);
	}
	return *this;
}

String& String::operator += (const char *s)
{
	if (s)
	{
		String temp(*this);
		_size += str_util::strlen(s);
		SAFE_DELETE_ARRAY(_buffer);
		_buffer = new char[_size + 1];
		str_util::strcpy(_buffer, temp._buffer);
		str_util::strcpy(_buffer + temp._size, s);
	}
	return *this;
}

inline char& String::operator [](int index)
{
	Assert(index >= 0 && index < _size);
	return _buffer[index];
}

//在src中查找dest，从位置offset处开始
int String::find(String &str, int offset)
{
	int i = offset;
	int j = 0;
	while (i < _size && j < str._size)
	{
		if (_buffer[i] == str._buffer[j]) 
		{
			++i;
			++j;
		}
		else
		{
			i = i - j + 1;
			j = 0;
		}
	}
	if (j > str._size - 1)
	{
		return i - str._size;
	}
	else 
	{
		return -1;
	}
}

int* String::get_next()
{
	int *next = new int[_size];
	next[0] = -1;

	int j = 0;
	int k = -1;
	while (j < _size - 1)
	{
		if (k == -1 || _buffer[j] == _buffer[k])
		{
			++j;
			++k;
			//next[j]=k;修正为:
			if (_buffer[j] != _buffer[k])
			{
				next[j] = k;
			}
			else
			{
				next[j] = next[k];
			}
		}
		else
		{
			k = next[k];
		}
	}
	return next;
}

int String::find_kmp(String &str, int offset)
{
	int *next = str.get_next();
	int i = offset;
	int j = 0;
	while (i <= _size - 1 && j <= str._size - 1)
	{
		if (_buffer[i] == str._buffer[j]) 
		{
			++i;
			++j;
		}
		else
		{
			j = next[j];
		}
	}

	SAFE_DELETE_ARRAY(next);
	if (j > str._size - 1)
	{
		return i - str._size;
	}
	else 
	{
		return -1;
	}
}

bool operator == (const String &str1, const String &str2)
{
	if (str1.size() != str2.size())
	{
		return false;
	}
	return ::strcmp(str1.c_str(),str2.c_str()) ? false : true;
}

bool operator == (const String&str, const char *s)
{
	return ::strcmp(str.c_str(), s) ? false : true;
}
