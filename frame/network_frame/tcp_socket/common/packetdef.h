////////////////////////////////////////////////////////////////////////////////////
// 
// @file: packetdef.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:27
// @brief: 消息包ID定义
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __PACKET_DEFINE_H_
#define __PACKET_DEFINE_H_

enum PACKET_ID
{
	PACKET_INVALID = -1,

	PACKET_CS_AUTHORIZE,
	PACKET_SC_RET_AUTHORIZE,
	PACKET_CS_ENTER_SCENE,
	PACKET_SC_RET_ENTER_SCENE,
	PACKET_CS_HEARTBEAT,

	PACKET_NUMBER,
};


#endif
