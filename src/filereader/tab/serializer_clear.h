////////////////////////////////////////////////////////////////////////////////////
// 
// @file: serializer_clear.h
// @author: by Mr.Chen
// @date: 2014/5/15	15:09
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef _SERIALIZER_CLEAR_H
#define _SERIALIZER_CLEAR_H

#include "serializer.h"

namespace file_parser {

	class ClearSerializer : public ISerializer
	{
	public:
		ClearSerializer() {}
		virtual ~ClearSerializer() {}

		ClearSerializer& operator << (int&		value)			{ value = -1; 	return *this; }
		ClearSerializer& operator << (uint&	value)			{ value = -1; 	return *this; }
		ClearSerializer& operator << (int64&	 value)			{ value = -1; 	return *this; }
		ClearSerializer& operator << (uint64& value)			{ value = -1; 	return *this; }
		ClearSerializer& operator << (float&	 value)			{ value = 0.0f; return *this; }
		ClearSerializer& operator << (char*&	 value)			{ value = NULL; return *this; }
		ClearSerializer& operator << (IntArray&	value)		{ return *this; };
		ClearSerializer& operator << (FloatArray& value)		{ return *this; };
		void skipField() {}
		void setCheckColumn(bool check) {}
	};
} // namespace file_parser
using namespace file_parser;

#endif	// __CLEAR_SERIALIZER_H_

