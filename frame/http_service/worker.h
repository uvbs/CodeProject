/***************************************************
** @file: worker.h
** @author: Mr.Chen
** @date: 2016/7/6
** @brief: 
***************************************************/
#ifndef _WORKER_H
#define _WORKER_H

#include "type.h"
#include "thread.h"

//fore declare
class EVHttpServer;
struct event_base;
struct evhttp;
struct evhttp_request;

//工作者类
class Worker
{
public:
	Worker() : _base(NULL), _http(NULL), _server(NULL) {}
	bool	init(EVHttpServer* p);
	void	start() { _thread.start(&Worker::run, this); }
	static void*	run(void* arg);
	void	wait();
	bool	do_service();
	static void do_logic(struct evhttp_request* req, void* arg);
    void  processRequest(struct evhttp_request* req);
	
private:
	static const int kMaxReceiveBufferSize = 1024 * 32;
	char _recv_buffer[kMaxReceiveBufferSize];
	struct event_base* _base;
	struct evhttp* _http;
	EVHttpServer* _server;
	Thread _thread;
};

#endif // _WORKER_H

