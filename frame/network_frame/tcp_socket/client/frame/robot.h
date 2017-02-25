/***************************************************
** @file: robot.h
** @author: Mr.Chen
** @date: 2016/7/4
** @brief: ������
***************************************************/
#ifndef _ROBOT_H
#define _ROBOT_H

#include "player.h"
#include "timer.h"
#include "common.h"

class Robot: public Player
{
public:
	Robot(int userid);
	~Robot();

	bool	init();
	void	cleanUp();

	bool	select();
	bool	processInputs();
	bool	processOutputs();
	bool	processExceptions();
	bool	processCmds();

	bool	connectServer();
	bool	useSocket();
	bool	removeSocket();
	bool	sendCloseMsg();

	bool	heartBeat(uint uTime);
	virtual bool	isGamePlayer()		{ return true; }
	virtual bool	isServerPlayer()		{ return false; }
	const char*	getUserId() const { return _userId; }

	virtual void	encrypt_cs(char* header, int len) {}
	virtual void	decryptHead_cs(char* header) {}
	virtual void	decrypt_cs(char* header, int len) {}

public:
	//�����߼�
	void	testLogic(uint time);
	void	testTick(uint uTime);
	void	testChat(uint uTime);
	void	testClose(uint uTime);
	
public:
	EM_PLAYER_STATUS	_status; //���״̬
	char	_userId[MAX_USER_LEN]; //�˺�id

private:
	static const int kSendBufferSize = 1024*1024;
	static const int kRecvBufferSize = 1024*1024;

	fd_set	_readFds[EM_SELECT_MAX];		//��fd
	fd_set	_writeFds[EM_SELECT_MAX];		//дfd
	fd_set	_exceptFds[EM_SELECT_MAX];	//�쳣fd
	timeval	_timeout[EM_SELECT_MAX];		//select�ӳٷ���ʱ������
	SOCK	_minFd; //��Сfd
	SOCK	_maxFd; //���fd
	int			_fdSize;  //FD����

	bool		_bConnected;	//�Ƿ��Ѿ�����Server
	Timer	_connectTimer;	//��������
	Timer	_tickTimer;
	Timer	_chatTimer;
	Timer	_closeTimer;
};

#endif	//_ROBOT_H

