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

    // DB连接
public:
    //初始化
    virtual bool init(const char* ip, uint port, const char* user, const char* pwd,const char* dbname,const char* encoding);
    //逻辑处理
    virtual bool heartbeat(uint uTime) ;

protected:
    //连接DB
    bool	connect();
    //失去连接后重连
    bool	reconnect();
public:
    //断开连接
    bool disconnect();
    //
    CDataBase*	getDataBase() const { return _pDataBase; }

private:
    //[MySqlDB]
    char	_ip[MAX_IP_LEN];	// 数据库IP
    uint	_port;					// 数据库端口
    char	_user[32];			    // 数据库用户名
    char	_pwd[32];			    // 数据库密码
    char	_dbname[32];	    // 数据库
    char _encoding[32];      //编码方式
    Timer _reconnTimer;		//重连计时器
    CDataBase*	_pDataBase;		// 数据库操作接口指针
};

bool test_mysql();

#endif  //_DB_INTERFACE_H
