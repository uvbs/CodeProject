////////////////////////////////////////////////////////////////////////////////////
// 
// @file: sceneplayer_mgr.h
// @author: by Mr.Chen
// @date: 2014/11/4	16:13
// @brief: 进入逻辑后的连接管理器
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __SCENE_PLAYER_MGR_H_
#define __SCENE_PLAYER_MGR_H_

#include "type.h"
#include "player_mgr.h"
#include "lock.h"
#include "game_player.h"

class Scene;
class ScenePlayerMgr : public PlayerMgr
{
public :
	//初始化
	virtual bool init(int count) ;
    //删除一个玩家（并不断网，有可能转移到别的场景）
    virtual bool kickPlayer(GamePlayer* pPlayer);
	virtual void getMaxMinFD(SOCK removedFD, SOCK& maxFD, SOCK& minFD);
	Scene*	getScene() { return _pScene; };	
	void	setScene(Scene* pScene){  _pScene = pScene; }
    
private :
	//场景
	Scene* _pScene;	// 所在频道的指针
};

#endif
