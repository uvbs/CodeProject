////////////////////////////////////////////////////////////////////////////////////
// 
// @file: player_mgr.h
// @author: by Mr.Chen
// @date: 2014/11/3	17:15
// @brief: 连接基类
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __PLAYER_MGR_H_
#define __PLAYER_MGR_H_

#include "type.h"
#include "socketdef.h"
#include "packet.h"
#include "object_mgr.h"
#include "lock.h"
#include "assert.h"
#include "macrodef.h"


class GamePlayer;
class PlayerMgr : public ObjectMgr<GamePlayer>
{
public :
	PlayerMgr();
	virtual ~PlayerMgr() {}

	//初始化
	virtual bool init() {return true;}
	//清理
	virtual void cleanUp();
	virtual void clearSocket();
	//网络侦测
	virtual bool select();
	//数据接受
	virtual bool processInputs();
	//数据发送
	virtual bool processOutputs();
	//异常连接处理
	virtual bool processExceptions();
	//消息执行
	virtual bool processCmds();
	//逻辑
	virtual bool heartBeat(uint time);
	//将Player数据加入系统中
	virtual bool addPlayer(GamePlayer* pPlayer);
	//删除一个玩家（并不断网，有可能转移到别的场景）
	virtual bool removePlayer(int playerId);
	virtual bool removeSocket(GamePlayer* pPlayer);
	virtual void getMaxMinFD(SOCK removedFD,SOCK& maxFD,SOCK& minFD) {}
	//将Player信息清除，并断开连接
	virtual bool kickPlayer(GamePlayer* pPlayer) { return true;}
	//将所有player信息清理,并断开连接
	virtual void kickAllPlayers();
	virtual bool broadCast(Packet* pPacket);

protected:
	fd_set _readFds[EM_SELECT_MAX];	//读fd
	fd_set _writeFds[EM_SELECT_MAX];	//写fd
	fd_set _exceptFds[EM_SELECT_MAX];	//异常fd
	timeval _timeout[EM_SELECT_MAX];	//select延迟返回时间配置
	SOCK _minFd;	//最小fd
	SOCK _maxFd;//最大fd
};

#endif	//__PLAYER_MGR_H_

