////////////////////////////////////////////////////////////////////////////////////
// 
// @file: game_player_mgr.h
// @author: by Mr.Chen
// @date: 2014/11/4	14:53
// @brief: 登陆链接管理器
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __GAME_PLAYER_MGR_H_
#define __GAME_PLAYER_MGR_H_

#include "type.h"
#include "packet.h"
#include "socket_server.h"
#include "socketdef.h"
#include "lock.h"
#include "singleton.h"
#include "thread.h"
#include "game_player.h"
#include "player_mgr.h"

class GamePlayerMgr : public PlayerMgr, public Singleton<GamePlayerMgr>
{
public:
	static const int MAX_PACKET_CACHE_SIZE = 4096;

public :
	GamePlayerMgr();
	virtual ~GamePlayerMgr();

	//初始化
	virtual bool init(int nCout, const char* host, int port) ;
	//数据接受
	virtual bool processInputs();
	//消息执行
	virtual bool processCmds();
	//新连接处理
	bool acceptNewConnection();
	//处理缓存消息
	bool processCacheCmds();
	//支持数据同步，即可以在不同线程内调用
	bool sendPacket(Packet* pPacket, int PlayerID, bool bRemove=false) ;
	//读取缓存消息
	bool receivePacket(Packet*& pPacket, int& PlayerID, bool& bRemove) ;
	//重新调整缓存大小
	bool resizePacketCache() ;
	//删除无效Player的消息
	virtual bool removePacket(int PlayerID) ;

	//将Player信息清除，并断开连接
	virtual bool kickPlayer(GamePlayer* pPlayer) ;
	virtual void getMaxMinFD(SOCK removedFD,SOCK& maxFD,SOCK& minFD);
	void relive();

private:
	SocketServer* _pSrvSocket ;	//用于侦听的服务器Socket
	SOCK _srvSocketFd;				
	MLock _lock ;	//锁
	PacketCache* _pPacketQ;	//当前的消息缓存队列
	int	 _packetQSize;//当前的消息缓存队列长度
	int	 _head;			//头标记
	int	 _tail;			//尾标记

public :
	ThreadId  _tid ;
};

#endif
