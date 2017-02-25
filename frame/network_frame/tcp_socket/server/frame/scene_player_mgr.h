////////////////////////////////////////////////////////////////////////////////////
// 
// @file: sceneplayer_mgr.h
// @author: by Mr.Chen
// @date: 2014/11/4	16:13
// @brief: �����߼�������ӹ�����
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
	//��ʼ��
	virtual bool init(int count) ;
    //ɾ��һ����ң������������п���ת�Ƶ���ĳ�����
    virtual bool kickPlayer(GamePlayer* pPlayer);
	virtual void getMaxMinFD(SOCK removedFD, SOCK& maxFD, SOCK& minFD);
	Scene*	getScene() { return _pScene; };	
	void	setScene(Scene* pScene){  _pScene = pScene; }
    
private :
	//����
	Scene* _pScene;	// ����Ƶ����ָ��
};

#endif
