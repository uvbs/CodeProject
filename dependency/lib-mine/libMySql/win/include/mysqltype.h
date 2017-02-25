////////////////////////////////////////////////////////////////////////////////////
// 
// @file: MySQLType.h
// @author: by Mr.Chen
// @date: 2014/11/9	14:24
// @brief: MySQL类型定义
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __MYSQLTYPE_H_
#define __MYSQLTYPE_H_

#include "type.h"

typedef struct st_mysql			MYSQL;
typedef struct st_mysql_res		MYSQL_RES;
typedef struct st_mysql_stmt	MYSQL_STMT;
typedef struct st_mysql_bind	MYSQL_BIND;

struct DBBlob
{
	int		m_Length;
	void*	m_pData;
};

// 重连结果
enum EM_RECONNECT_RESULT
{
    EM_RECONNECT_FAILED = -1,	// 重连失败
    EM_RECONNECT_NONE,			// 未发生重连
    EM_RECONNECT_OK,			// 重连成功
};

#endif   // __MYSQLTYPE_H_

