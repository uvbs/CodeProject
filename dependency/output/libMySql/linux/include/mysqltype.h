////////////////////////////////////////////////////////////////////////////////////
// 
// @file: MySQLType.h
// @author: by Mr.Chen
// @date: 2014/11/9	14:24
// @brief: MySQL���Ͷ���
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

// �������
enum EM_RECONNECT_RESULT
{
    EM_RECONNECT_FAILED = -1,	// ����ʧ��
    EM_RECONNECT_NONE,			// δ��������
    EM_RECONNECT_OK,			// �����ɹ�
};

#endif   // __MYSQLTYPE_H_

