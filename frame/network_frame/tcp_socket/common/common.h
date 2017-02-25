/***************************************************
** @file: common.h
** @author: Mr.Chen
** @date: 2016/7/4
** @brief: Server与Client公用的定义
***************************************************/

#ifndef __COM_H_
#define __COM_H_

#include "type.h"

const int MAX_USER_LEN = 32;

//连接状态
enum EM_PLAYER_STATUS
{
	EM_PLAYER_STATUS_INVALID = -1,
	EM_PLAYER_STATUS_IDLE,						//初始状态
	EM_PLAYER_STATUS_CONNECT,				//连接成功，如果N秒内没收后面的消息，则断开此连接
	EM_PLAYER_STATUS_AUTHORIZED,		//认证成功
	EM_PLAYER_STATUS_ENTER,					//准备OK
	EM_PLAYER_STATUS_NORMAL,				//正常逻辑
	EM_PLAYER_STATUS_NUM,
};

#endif	//__COM_H_

