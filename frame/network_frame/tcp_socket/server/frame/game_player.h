////////////////////////////////////////////////////////////////////////////////////
// 
// @file: game_player.h
// @author: by Mr.chen
// @date: 2014/5/26	14:36
// @brief: ����
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __GAME_PLAYER_H_
#define __GAME_PLAYER_H_

#include "type.h"
#include "player.h"
#include "timer.h"
#include "string_util.h"
#include "common.h"


class GamePlayer : public Player
{
public:
	GamePlayer(bool isAsSocketCli = true);
	~GamePlayer();

	//��ʼ��
	bool	init() ;
	virtual bool	heartBeat(uint time = 0, int flag = 0);
	virtual void	cleanUp();
	bool	isIdle();

	void setStatus(EM_PLAYER_STATUS status) { _status = status ; }
	EM_PLAYER_STATUS getStatus() { return _status; }

	bool	free(); //�ͷŲ��Ͽ�����
	virtual void	resetKickTime(); //��������Ϣ�߳�

	//�ǿͻ������ӻ��Ƿ���������
	virtual bool	isGamePlayer() { return true; }
	virtual bool	isServerPlayer() { return false; }

	const char*	getUserId() const { return _userId; }
	void setUserId(const char* id) { str_util::strcpy_s(_userId, id, sizeof(_userId)); }

	//Э������ܽ���
	virtual void	encrypt_cs(char* header, int uLen){/*ENCRYPT(header, uLen, GAMESERVER_TO_CLIENT_KEY)*/}
	virtual void	decryptHead_cs(char* header){/*ENCRYPT_HEAD(header, CLIENT_TO_GAMESERVER_KEY)*/}
	virtual void	decrypt_cs(char* header, int uLen){/*ENCRYPT(header, uLen, CLIENT_TO_GAMESERVER_KEY)*/}

private:
	char _userId[MAX_USER_LEN]; //�˺�id
	EM_PLAYER_STATUS _status;	//״̬
	Timer _kickTimer; //�ߵ���Ҽ�ʱ
};

#endif
