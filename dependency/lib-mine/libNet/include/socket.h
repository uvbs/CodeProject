////////////////////////////////////////////////////////////////////////////////////
// 
// @file: socket.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:13
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __SOCKET_H_
#define __SOCKET_H_

#include "type.h"
#include "socketdef.h"

namespace tcp_net {

extern int getSocketErrorCode();
extern char* getSocketErrorDescription();

////////////////////////////////////////////////////////////////////////////////
//	class Socket
////////////////////////////////////////////////////////////////////////////////
class Socket
{
public :
	Socket();
	virtual ~Socket();

public:
	//����socket
	virtual bool	create();
	//�ر�����
	virtual void	close();
	//���ùر���ʱʱ��
	virtual bool	setLinger(uint lingertime) ;
	//���������������ģʽ
	virtual bool	setNonBlocking(bool on = true);
	//����delay
	virtual bool	setNonDelay(bool on = true);
	//�Ƿ�socket�д���
	virtual bool	isSockError() const;
	//��ô�����
	virtual int		getSockError() const;
	//��ý��ܻ�����buffer��С
	virtual int		getReceiveBufferSize() const;
	//���ý��ջ�������С
	virtual bool	setReceiveBufferSize(int size);
	//��÷��ͻ�������С
	virtual int		getSendBufferSize() const;
	//���÷��ͻ�������С
	virtual bool	setSendBufferSize(int size);
	//��������
	virtual int		send(const void* buf, int length, int flags = 0);
	//��������
	virtual int		receive(void* buf, int length, int flags = 0);
	//��������
	uint		available() const;
	//��õ�ַ����
	SOCKADDR_IN* getSocketAddr(){ return &_sockAddr; };
	void		setFd(SOCK s){ _sockfd = s;};
	SOCK	getFd() const { return _sockfd; }
	//�ж�socket�Ƿ���Ч
	bool		isValid() { return _sockfd != INVALID_SOCKET; }
	//�������IP
	char*	getHost() { return inet_ntoa(_sockAddr.sin_addr); }
	unsigned long getHostIP() const { return (unsigned long)(_sockAddr.sin_addr.s_addr); }

protected:
	SOCK _sockfd;  //�׽���
	SOCKADDR_IN _sockAddr; //��ַ����
};

} //namespace tcpnet
using namespace tcp_net;

#endif
