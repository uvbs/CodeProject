#ifndef _SCENE_H
#define _SCENE_H

#include "type.h"
#include "scene_player_mgr.h"
#include "singleton.h"

// 场景
class Scene
{
public:
	Scene(int nSceneId);
	virtual ~Scene();

	bool	init(int nPlayerCount);
	void	cleanUp();
	bool	tick(uint uTime);
	bool	heartBeat(uint uTime);

	//处理缓存消息
	bool	processCacheCmds(); 
	bool	sendPacket(Packet* pPacket, Player_ID nPlayerId, bool bRemove = false);	//支持数据同步，即可以在不同线程内调用
	bool	recvPacket(Packet*& pPacket, Player_ID& nPlayerId, bool& bRemove);//读取缓存消息
	bool	resizeCache();
	bool	removePacket(int nPlayerId);	//删除无效Player的消息

	int		getSceneId() const { return _nSceneId; }
	void	setTid(ThreadId id){ _nTid = id;}
	ThreadId	getTid() const { return _nTid; }
	ScenePlayerMgr*	getScenePlayerMgr() { return _pScenePlayerMgr;}
	int	   getPlayerCount() const { MyAssert(_pScenePlayerMgr); return _pScenePlayerMgr->getObjNum(); }
	bool broadCast(Packet* pPacket);

private:
	int	 _nSceneId;
	ThreadId	_nTid;	
	MLock	_lockQ;
	//消息缓存
	static const int kPacketCacheGrow = 1024;
	PacketCache*	 _pPacketCache; //当前的消息缓存队列
	int		_nPacketCacheCurrSize;	//消息缓存队列当前的长度
	int		_nHead ;	//头标记
	int		_nTail ;		//尾标记
	//管理器
	ScenePlayerMgr* _pScenePlayerMgr;	//连接管理器
};

//// 场景管理器
//class SceneMgr : public Singleton<SceneMgr>
//{
//public:
//	SceneMgr();
//	~SceneMgr();
//
//	bool	init();
//	Scene*	getSceneByIndex(int nIndex) const;
//	Scene*	getScene(int nSceneId) const;
//	bool	addScene(Scene* pScene);
//	int		getSceneCount() const { return _nSceneCount; }
//	int		getMinloadScene() const;
//	
//private:
//	static const int kMaxSceneCount = 8;
//	Scene*	_pScenes[kMaxSceneCount];
//	int _nSceneCount;
//};

#endif

