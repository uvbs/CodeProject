////////////////////////////////////////////////////////////////////////////////////
// 
// @file: game_player_mgr.h
// @author: by Mr.Chen
// @date: 2014/11/4	14:53
// @brief: ��½���ӹ�����
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

	//��ʼ��
	virtual bool init(int nCout, const char* host, int port) ;
	//���ݽ���
	virtual bool processInputs();
	//��Ϣִ��
	virtual bool processCmds();
	//�����Ӵ���
	bool acceptNewConnection();
	//��������Ϣ
	bool processCacheCmds();
	//֧������ͬ�����������ڲ�ͬ�߳��ڵ���
	bool sendPacket(Packet* pPacket, int PlayerID, bool bRemove=false) ;
	//��ȡ������Ϣ
	bool receivePacket(Packet*& pPacket, int& PlayerID, bool& bRemove) ;
	//���µ��������С
	bool resizePacketCache() ;
	//ɾ����ЧPlayer����Ϣ
	virtual bool removePacket(int PlayerID) ;

	//��Player��Ϣ��������Ͽ�����
	virtual bool kickPlayer(GamePlayer* pPlayer) ;
	virtual void getMaxMinFD(SOCK removedFD,SOCK& maxFD,SOCK& minFD);
	void relive();

private:
	SocketServer* _pSrvSocket ;	//���������ķ�����Socket
	SOCK _srvSocketFd;				
	MLock _lock ;	//��
	PacketCache* _pPacketQ;	//��ǰ����Ϣ�������
	int	 _packetQSize;//��ǰ����Ϣ������г���
	int	 _head;			//ͷ���
	int	 _tail;			//β���

public :
	ThreadId  _tid ;
};

#endif
