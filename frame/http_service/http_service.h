/***************************************************
** @file: http_service.h
** @author: Mr.Chen
** @date: 2016/7/7
** @brief: 
***************************************************/
#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H

#include "type.h"

class Worker;
// EVHTTP route server is a libevent based multi-thread server
// The master thread creates several worker threads to deal with route requests.
// Each worker thread has an event_base instance for event notification.
class EVHttpServer
{
public:
	EVHttpServer();
	~EVHttpServer();
	bool	init(const char* ip, uint16 port, int timeout_ms = 200);
	void	start();
	void	wait();
	void	destroy();
	//void DestructWorker(Worker* worker);
	
	const char* getEVServerType() const { return "Route Service"; }
	int		getFd() const { return _fd; }
	int		getTimeout() const { return _timeout_ms; } 

private:
	int		_setupServerSocket();
	
private:
	static const int kMaxWorkerCount = 4;
	char _ip[16];
	uint16 _port;
	int _fd; // socket fd
	int	 _timeout_ms;
	int _worker_count;
	Worker** _worker_pool; // worker thread pool
};


#endif // HTTP_SERVER_H
