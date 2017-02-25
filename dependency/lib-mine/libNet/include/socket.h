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
	//创建socket
	virtual bool	create();
	//关闭连接
	virtual void	close();
	//设置关闭延时时间
	virtual bool	setLinger(uint lingertime) ;
	//设置阻塞或非阻塞模式
	virtual bool	setNonBlocking(bool on = true);
	//禁用delay
	virtual bool	setNonDelay(bool on = true);
	//是否socket有错误
	virtual bool	isSockError() const;
	//获得错误码
	virtual int		getSockError() const;
	//获得接受缓冲区buffer大小
	virtual int		getReceiveBufferSize() const;
	//设置接收缓冲区大小
	virtual bool	setReceiveBufferSize(int size);
	//获得发送缓冲区大小
	virtual int		getSendBufferSize() const;
	//设置发送缓冲区大小
	virtual bool	setSendBufferSize(int size);
	//发送数据
	virtual int		send(const void* buf, int length, int flags = 0);
	//接收数据
	virtual int		receive(void* buf, int length, int flags = 0);
	//现有数据
	uint		available() const;
	//获得地址描述
	SOCKADDR_IN* getSocketAddr(){ return &_sockAddr; };
	void		setFd(SOCK s){ _sockfd = s;};
	SOCK	getFd() const { return _sockfd; }
	//判断socket是否有效
	bool		isValid() { return _sockfd != INVALID_SOCKET; }
	//获得主机IP
	char*	getHost() { return inet_ntoa(_sockAddr.sin_addr); }
	unsigned long getHostIP() const { return (unsigned long)(_sockAddr.sin_addr.s_addr); }

protected:
	SOCK _sockfd;  //套接字
	SOCKADDR_IN _sockAddr; //地址描述
};

} //namespace tcpnet
using namespace tcp_net;

#endif
