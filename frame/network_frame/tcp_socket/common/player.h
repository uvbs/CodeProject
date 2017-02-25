////////////////////////////////////////////////////////////////////////////////////
// 
// @file: player.h
// @author: by Mr.Chen
// @date: 2014/5/19	12:07
// @brief: 接入端，网络连接
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "type.h"
#include "socketdef.h"
#include "socket.h"
#include "packet.h"
#include "socket_stream.h"

namespace tcp_net {

using tcp_net::Socket;
using tcp_net::Packet;
using tcp_net::SocketStream;
////////////////////////////////////////////////////////////////////////////////
//	class Player
////////////////////////////////////////////////////////////////////////////////
class Player
{
public :
	Player(bool isClient = true) ;
	virtual ~Player() ;
	
	virtual bool	init() { return true; }
	virtual void	cleanUp();

	Socket* getSocket() { return _pSocket; }
	SocketStream* getSocketStream() { return _pSocketStream;}

	//断开与当前玩家的网络连接
	virtual void	disconnect() ;
	//判断当前玩家的网络连接是否有效
	virtual bool isValid() { return _pSocket->isValid(); }
	virtual bool isIdle() { return false; }
	//处理输入
	virtual bool	processInput() ;
	//处理输出
	virtual bool	processOutput() ;
	//处理协议
	virtual bool	processCmd() ;
	//心跳接口，处理所有逻辑
	virtual bool	heartBeat(uint time = 0, int flag = 0) { return true;}
	//发送数据
	virtual bool	sendPacket(Packet* pPacket) ;

	//设置,取管理器中id
	int	 getMgrID(){ return _mgrID ; }
	void setMgrID( int id ){ _mgrID = id ; }
	//读取设置玩家PID的接口，PlayerID 是由player池分配出来的索引值
	Player_ID getPlayerID(){ return _poolID ; } 
	int	 getPoolID(){ return _poolID; }
	void setPoolID( int id ){ _poolID = id ; }

	//重置无消息踢除
	virtual void	resetKickTime() {}
	//是客户端连接还是服务器连接
	virtual bool	isGamePlayer() = 0 ;
	virtual bool	isServerPlayer() = 0 ;
	//Tencent TGW包
	virtual bool	getFlag() const {return false;}
	virtual void	setFlag(bool val) {}

	//协议包加密解密
	virtual void	encrypt_cs(char* header, int len) = 0;
	virtual void	decryptHead_cs(char* header) = 0;
	virtual void	decrypt_cs(char* header, int len) = 0;

protected :
	//网络连接句柄
	Socket* _pSocket;
	//输入输出数据缓存
	SocketStream* _pSocketStream;
	bool _connected;
	//在管理器中的id
	int	 _mgrID;
	//池中位置
	Player_ID _poolID;
};

} //namespace tcp_net
using namespace tcp_net;

#endif
