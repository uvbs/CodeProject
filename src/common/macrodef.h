////////////////////////////////////////////////////////////////////////////////////
// 
// @file: macrodef.h
// @author: by Mr.Chen
// @date: 2014/5/11	01:59
// @brief: 各种宏定义
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __MACRODEF_H_
#define __MACRODEF_H_

#include "type.h"

#ifdef _WIN32
#define		tvsnprintf		_vsnprintf
#define		tstricmp		_stricmp
#define		tsnprintf		_snprintf
#endif

#ifdef _LINUX64
#define		tvsnprintf		vsnprintf
#define		tstricmp		strcasecmp
#define		tsnprintf		snprintf
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if( (x)!=NULL ){ delete(x); (x)=NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if( (x)!=NULL ){ delete[](x); (x)=NULL; }
#endif

#ifndef Max
#define Max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef Min
#define Min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#ifdef _WIN32
#define __MYTRY try
#define __MYCATCH catch(...)
#endif

#ifdef _LINUX64
#define __MYTRY try
#define __MYCATCH catch(...)
#endif

#ifdef _WIN32
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#ifdef __SGI_STL_PORT
	#define HASH_MAP  std::hash_map
#else
#ifdef _WIN32
	#define HASH_MAP stdext::hash_map
#else
	#define HASH_MAP __gnu_cxx::hash_map
#endif
#endif

#define PORT       8800
#define MSGSIZE    1024
#define SERVER_ADDRESS "127.0.0.1"
const int  MAX_FILE_NAME_LEN = 256;

#endif
