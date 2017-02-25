////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_mysql.h
// @author: by Mr.Chen
// @date: 2015/5/25	11:22
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef _IDB_INTERFACE_H
#define _IDB_INTERFACE_H

#include "type.h"
#include "database.h"
#include "singleton.h"
#include "timer.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

class IDBInterface
{
public:
    static const int MAX_SQL_LEN = 2048;
    static const int MAX_IP_LEN = 24;

    IDBInterface();
    ~IDBInterface();

    // DB����
public:
    //��ʼ��
    virtual bool init(const char* ip, uint port, const char* user, const char* pwd,const char* dbname,const char* encoding);
    //�߼�����
    virtual bool heartbeat(uint uTime) ;

protected:
    //����DB
    bool	connect();
    //ʧȥ���Ӻ�����
    bool	reconnect();
public:
    //�Ͽ�����
    bool disconnect();
    //
    CDataBase*	getDataBase() const { return _pDataBase; }

private:
    //[MySqlDB]
    char	_ip[MAX_IP_LEN];	// ���ݿ�IP
    uint	_port;					// ���ݿ�˿�
    char	_user[32];			    // ���ݿ��û���
    char	_pwd[32];			    // ���ݿ�����
    char	_dbname[32];	    // ���ݿ�
    char _encoding[32];      //���뷽ʽ
    Timer _reconnTimer;		//������ʱ��
    CDataBase*	_pDataBase;		// ���ݿ�����ӿ�ָ��
};

bool test_mysql();

#endif  //_DB_INTERFACE_H
