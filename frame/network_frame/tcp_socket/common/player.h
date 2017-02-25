////////////////////////////////////////////////////////////////////////////////////
// 
// @file: player.h
// @author: by Mr.Chen
// @date: 2014/5/19	12:07
// @brief: ����ˣ���������
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

	//�Ͽ��뵱ǰ��ҵ���������
	virtual void	disconnect() ;
	//�жϵ�ǰ��ҵ����������Ƿ���Ч
	virtual bool isValid() { return _pSocket->isValid(); }
	virtual bool isIdle() { return false; }
	//��������
	virtual bool	processInput() ;
	//�������
	virtual bool	processOutput() ;
	//����Э��
	virtual bool	processCmd() ;
	//�����ӿڣ����������߼�
	virtual bool	heartBeat(uint time = 0, int flag = 0) { return true;}
	//��������
	virtual bool	sendPacket(Packet* pPacket) ;

	//����,ȡ��������id
	int	 getMgrID(){ return _mgrID ; }
	void setMgrID( int id ){ _mgrID = id ; }
	//��ȡ�������PID�Ľӿڣ�PlayerID ����player�ط������������ֵ
	Player_ID getPlayerID(){ return _poolID ; } 
	int	 getPoolID(){ return _poolID; }
	void setPoolID( int id ){ _poolID = id ; }

	//��������Ϣ�߳�
	virtual void	resetKickTime() {}
	//�ǿͻ������ӻ��Ƿ���������
	virtual bool	isGamePlayer() = 0 ;
	virtual bool	isServerPlayer() = 0 ;
	//Tencent TGW��
	virtual bool	getFlag() const {return false;}
	virtual void	setFlag(bool val) {}

	//Э������ܽ���
	virtual void	encrypt_cs(char* header, int len) = 0;
	virtual void	decryptHead_cs(char* header) = 0;
	virtual void	decrypt_cs(char* header, int len) = 0;

protected :
	//�������Ӿ��
	Socket* _pSocket;
	//����������ݻ���
	SocketStream* _pSocketStream;
	bool _connected;
	//�ڹ������е�id
	int	 _mgrID;
	//����λ��
	Player_ID _poolID;
};

} //namespace tcp_net
using namespace tcp_net;

#endif
