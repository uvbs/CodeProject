////////////////////////////////////////////////////////////////////////////////////
// 
// @file: socketdef.h
// @author: by Mr.Chen
// @date: 2014/5/9	15:17
// @brief: socket相关类型定义
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __SOCKET_DEF_H_
#define __SOCKET_DEF_H_

#include "type.h"

#if defined(_WIN32)

#ifndef FD_SETSIZE
#define FD_SETSIZE 4096
#endif
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <MSWSock.h>
#include <io.h>

typedef SOCKET SOCK;

#elif defined(_LINUX64)

#ifndef FD_SETSIZE
#define FD_SETSIZE 4096
#endif
#ifndef __FD_SETSIZE
#define __FD_SETSIZE 4096
#endif

#include <netinet/in.h>
#include <arpa/inet.h>
typedef int SOCK;
typedef	struct sockaddr_in SOCKADDR_IN;

const int INVALID_SOCKET = -1;
const int SOCKET_ERROR = -1;

#endif	// _LINUX64

typedef  uint64 IP_t;				// IP
typedef int16	 Packet_ID;			// 定义网络包id
typedef int Player_ID;			// 连接id

//socket error step
enum EM_SOCKET_ERROR_STEP
{
	EM_SOCKET_ERROR_STEP0 = -1,
	EM_SOCKET_ERROR_STEP1 = -2,
	EM_SOCKET_ERROR_STEP2 = -3,
	EM_SOCKET_ERROR_STEP3 = -4,
	EM_SOCKET_ERROR_STEP4 = -5,
	EM_SOCKET_ERROR_STEP5 = -6,
	EM_SOCKET_ERROR_STEP6 = -7,
	EM_SOCKET_ERROR_STEP7 = -8,
	EM_SOCKET_ERROR_STEP8 = -9,
	EM_SOCKET_ERROR_STEP9 = -10,
	EM_SOCKET_ERROR_STEP10 = -11,
	EM_SOCKET_ERROR_STEP11 = -12,
	EM_SOCKET_ERROR_STEP12 = -13,
	EM_SOCKET_ERROR_STEP13 = -14,
	EM_SOCKET_ERROR_STEP14 = -15,
	EM_SOCKET_ERROR_STEP15 = -16,
	EM_SOCKET_ERROR_STEP16 = -17,
	EM_SOCKET_ERROR_STEP17 = -18,
};

enum EM_TYPE_MSG_MODE
{
	EM_TYPE_MSG_MODE_INVALID	= -1,
	EM_TYPE_MSG_MODE_WRITE,
	EM_TYPE_MSG_MODE_READ,
	EM_TYPE_MSG_MODE_NUMBER,
};

//消息处理的返回值
enum PACKET_EXE
{
	PACKET_EXE_ERROR = -1,		//包处理出错
	PACKET_EXE_BREAK ,			//不再进行下面包处理
	PACKET_EXE_CONTINUE ,		//继续下面处理
	PACKET_EXE_NOTREMOVE ,		//不将包移除
};

//select模型下使用
enum EM_SELECT
{
	EM_SELECT_BAK = 0,	//完整句柄
	EM_SELECT_USE,		//用于select调用句柄
	EM_SELECT_MAX,		//数量
};

const int DEFAULT_SOCKETINPUT_BUFFERSIZE = 256 * 1024; //接收缓存长度
const int MAX_SOCKETINPUT_BUFFERSIZE = 512 * 1024;		//最大接收缓存长度
const int DEFAULT_SOCKETOUTPUT_BUFFERSIZE	= 1024 * 1024;	//发送缓存长度
const int  MAX_SOCKETOUTPUT_BUFFERSIZE = 2 * 1024 * 1024;//最大发送缓存长度

const int MAX_IP_SIZE = 24;	 //IP地址的字符最大长度
const int SOCKET_ERROR_SIZE = 256;	 //网络错误
const int SOCKET_ERROR_UNKNOWN = ~0;	//未知socket error
const int SOCKET_ERROR_WOULD_BLOCK = -100;	//特殊错误(可能依然是正常)

const int  PACKET_HEADER_SIZE = sizeof(Packet_ID) + sizeof(char) + sizeof(int);
const int PACKET_COUNT_PER_TICK = 12;					//每帧可以执行的消息数量上限

#endif	// __SOCKET_DEF_H_
