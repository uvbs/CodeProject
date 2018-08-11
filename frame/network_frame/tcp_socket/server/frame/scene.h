#ifndef _SCENE_H
#define _SCENE_H

#include "type.h"
#include "scene_player_mgr.h"
#include "singleton.h"

// ����
class Scene
{
public:
	Scene(int nSceneId);
	virtual ~Scene();

	bool	init(int nPlayerCount);
	void	cleanUp();
	bool	tick(uint uTime);
	bool	heartBeat(uint uTime);

	//��������Ϣ
	bool	processCacheCmds(); 
	bool	sendPacket(Packet* pPacket, Player_ID nPlayerId, bool bRemove = false);	//֧������ͬ�����������ڲ�ͬ�߳��ڵ���
	bool	recvPacket(Packet*& pPacket, Player_ID& nPlayerId, bool& bRemove);//��ȡ������Ϣ
	bool	resizeCache();
	bool	removePacket(int nPlayerId);	//ɾ����ЧPlayer����Ϣ

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
	//��Ϣ����
	static const int kPacketCacheGrow = 1024;
	PacketCache*	 _pPacketCache; //��ǰ����Ϣ�������
	int		_nPacketCacheCurrSize;	//��Ϣ������е�ǰ�ĳ���
	int		_nHead ;	//ͷ���
	int		_nTail ;		//β���
	//������
	ScenePlayerMgr* _pScenePlayerMgr;	//���ӹ�����
};

//// ����������
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

