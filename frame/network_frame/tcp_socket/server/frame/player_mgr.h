////////////////////////////////////////////////////////////////////////////////////
// 
// @file: player_mgr.h
// @author: by Mr.Chen
// @date: 2014/11/3	17:15
// @brief: ���ӻ���
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

	//��ʼ��
	virtual bool init() {return true;}
	//����
	virtual void cleanUp();
	virtual void clearSocket();
	//�������
	virtual bool select();
	//���ݽ���
	virtual bool processInputs();
	//���ݷ���
	virtual bool processOutputs();
	//�쳣���Ӵ���
	virtual bool processExceptions();
	//��Ϣִ��
	virtual bool processCmds();
	//�߼�
	virtual bool heartBeat(uint time);
	//��Player���ݼ���ϵͳ��
	virtual bool addPlayer(GamePlayer* pPlayer);
	//ɾ��һ����ң������������п���ת�Ƶ���ĳ�����
	virtual bool removePlayer(int playerId);
	virtual bool removeSocket(GamePlayer* pPlayer);
	virtual void getMaxMinFD(SOCK removedFD,SOCK& maxFD,SOCK& minFD) {}
	//��Player��Ϣ��������Ͽ�����
	virtual bool kickPlayer(GamePlayer* pPlayer) { return true;}
	//������player��Ϣ����,���Ͽ�����
	virtual void kickAllPlayers();
	virtual bool broadCast(Packet* pPacket);

protected:
	fd_set _readFds[EM_SELECT_MAX];	//��fd
	fd_set _writeFds[EM_SELECT_MAX];	//дfd
	fd_set _exceptFds[EM_SELECT_MAX];	//�쳣fd
	timeval _timeout[EM_SELECT_MAX];	//select�ӳٷ���ʱ������
	SOCK _minFd;	//��Сfd
	SOCK _maxFd;//���fd
};

#endif	//__PLAYER_MGR_H_

