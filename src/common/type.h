////////////////////////////////////////////////////////////////////////////////////
// 
// File: type.h
// Author: by Mr.Chen
// CreateTime: 2014/5/1	0:20
// Desc: 基本数据类型定义
//          
////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __TYPE_H_
#define __TYPE_H_

//定义NULL
#ifndef NULL
#define NULL 0
#endif

typedef char				int8;
typedef unsigned char		uchar;
typedef unsigned char		uint8;
typedef unsigned char		byte;

typedef short				int16;
typedef unsigned short		uint16;

typedef int					int32;
typedef unsigned int		uint;
typedef unsigned int		uint32;

#ifdef _LINUX64
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned int        DWORD;
typedef unsigned int        UINT;
#endif

//typedef wchar_t				wchar;
typedef unsigned long		ulong;

#ifdef _WIN32
//typedef __int64					int64;
//typedef unsigned __int64	uint64;
typedef long long			int64;
typedef unsigned long long	uint64;
#endif

#ifdef _LINUX64
typedef long long			int64;
typedef unsigned long long	uint64;
#endif

#if defined(_WIN32)
typedef unsigned int ThreadId;
#elif defined(_LINUX64)
#include <pthread.h>
typedef pthread_t ThreadId;
#endif

#endif	//__TYPE_H_

