#include "socketdef.h"
#include "macrodef.h"
#if defined(_LINUX64)
#include <netinet/tcp.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#elif defined(_WIN32)
#include <windows.h>
#endif
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/http_struct.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include "http_service.h"
#include "worker.h"
#include "string_util.h"
#include "log.h"

volatile sig_atomic_t g_bMainLoop = 1;

static void LoopInterrupt(int)
{
	g_bMainLoop = 0;
}

EVHttpServer::EVHttpServer() : _port(0XFFFF), _worker_count(0), _timeout_ms(0) 
{
	_ip[0] = 0;
}

EVHttpServer::~EVHttpServer()
{
	for (int i = 0; i < _worker_count; ++i)
	{
		SAFE_DELETE(_worker_pool[i]);
	}
	SAFE_DELETE_ARRAY(_worker_pool);
}

bool EVHttpServer::init(const char* ip, unsigned short port, int timeout_ms/* = 200*/)
{
	str_util::strcpy_s(_ip, ip, sizeof(_ip));
	_port = port;
	_timeout_ms = timeout_ms;
	_worker_count = kMaxWorkerCount;
	_worker_pool =  new Worker*[_worker_count];
	Assert(_worker_pool);
	for (int i = 0; i < _worker_count; ++i)
	{
		Worker* worker = new Worker();
		Assert(worker);
		if (!worker->init(this))
		{
			fprintf(stderr, "Could not init worker.\n");
			fflush(stderr);
		}
		_worker_pool[i] = worker;
	}

#if defined(_WIN32)
	WSADATA wsaData; // ³õÊ¼»¯ÍøÂç
	unsigned short wVersionRequested = MAKEWORD(2, 2);
	int err = WSAStartup(wVersionRequested, &wsaData); 
	if (err != 0)
	{
		Assert(0);
	}
#endif

	// set up socket
	_fd = _setupServerSocket();
	//assert(server->fd >= 0);
	if (_fd < 0)
	{
		fprintf(stderr, "Could not set up server socket.\n");
		fflush(stderr);
		return false;
	}

	// register signals
	// Ctrl + C or kill -2 pid
	signal(SIGINT, (void (*)(int))LoopInterrupt);
	// kill pid or kill -15 pid
	signal(SIGTERM, (void (*)(int))LoopInterrupt);

#ifdef SIGHUP
	signal(SIGHUP, SIG_IGN);
#endif

#ifdef SIGBREAK
	// Ctrl + Break in Windows
	signal(SIGBREAK, (void (*)(int))LoopInterrupt);
#endif

#ifdef SIGPIPE
	// ignore the SIGPIPE when Linux socket was closed abnormally
	signal(SIGPIPE, SIG_IGN);
#endif
	//signal(SIGSEGV,(void (*)(int))SaveCurrentBacktrace);

	return true;
}

int EVHttpServer::_setupServerSocket()
{
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port);
	memset(&(addr.sin_addr), 0, sizeof(addr.sin_addr));
	addr.sin_addr.s_addr = inet_addr(_ip);

	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0)
	{
		Assert(0);
		return -1;
	}

	int reuse = 1;
	int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));
	if (ret < 0)
	{
#ifdef _WIN32
		closesocket(fd);
#else
		close(fd);
#endif
		Assert("Could not setsockopt.");
		return -1;
	}

	int enable = 1;
	ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char*)&enable, sizeof(enable));
	if (ret < 0)
	{
#ifdef _WIN32
		closesocket(fd);
#else
		close(fd);
#endif
		Assert("Could not set up TCP_NODELAY to socket");
		return -1;
	}

	ret = evutil_make_socket_nonblocking(fd);
	//assert(ret >= 0);
	if (ret < 0)
	{
#ifdef _WIN32
		closesocket(fd);
#else
		close(fd);
#endif
		Assert("Could not make socket nonblocking.\n");
		return -1;
	}

	ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
	//assert(ret >= 0);
	if (ret < 0)
	{
#ifdef _WIN32
		closesocket(fd);
#else
		close(fd);
#endif
		Assert("Could not bind to socket.\n");
		return -1;
	}

	ret = listen(fd, SOMAXCONN);
	//assert(ret >= 0);
	if (ret < 0)
	{
#ifdef _WIN32
		closesocket(fd);
#else
		close(fd);
#endif
		Assert("Could not listen to socket.\n");
		return -1;
	}

	return fd;
}

/*static void SaveCurrentBacktrace(int signo)
{
  if(signo == SIGSEGV)
  {
    void *array[20];
    char **strings;
    size_t size;
    size_t i;
    size = backtrace(array, 20);
    //strings = backtrace_symbols(array, size);
    FILE* bt_file = fopen("./backtrace","a+");
    char bt_buf[256];
    time_t timep;
    time(&timep);
	if(NULL != bt_file)
    {
      snprintf(bt_buf,256,"############[%s]############",ctime(&timep));
	  int tmplen = strlen(bt_buf);
      for(int i=0;i<tmplen-1;i++)
        if(bt_buf[i]=='\r' || bt_buf[i]=='\n')
          bt_buf[i]=' ';
      fwrite(bt_buf,strlen(bt_buf),1,bt_file);
      fwrite("\n",1,1,bt_file);
    }
    for(i=0;i<size;i++)
    {
      if(NULL != bt_file)
      {
        snprintf(bt_buf,256,"#%d   %p\n",i,array[i]);
        fwrite(bt_buf,strlen(bt_buf),1,bt_file);
      }
    }
    if(NULL != bt_file)
    {
      fflush(bt_file);
      fclose(bt_file);
    }
    signal(signo,SIG_DFL);
    raise(signo);
  }
}*/

void EVHttpServer::wait()
{
	for (int i = 0; i < _worker_count; ++i)
	{
		if (_worker_pool[i]) {
			_worker_pool[i]->wait();
		}
	}
}

//void EVHttpServer::DestructWorker(Worker* worker)
//{
//#if defined(WIN32) || defined(WIN64)
//  worker->active = false;
//#else
//  int ret = pthread_cancel(worker->td);
//  if (ret != 0)
//  {
//    fprintf(stderr, "Could not cancel worker.\n");
//    fflush(stderr);
//  }
//
//  void* res;
//  ret = pthread_join(worker->td, &res);
//  if (ret != 0)
//  {
//    fprintf(stderr, "Could not join worker.\n");
//    fflush(stderr);
//  }
//  evhttp_free(worker->_http);
//  event_base_free(worker->_base);
//  free(worker);
//#endif
//}

void EVHttpServer::destroy()
{
#if defined(_WIN32)
	shutdown(_fd, 0x2);
	//closesocket(server->fd);
	WSACleanup();
#else
	close(_fd);
#endif
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "%s shutdown.", getEVServerType());
}

void EVHttpServer::start()
{
	// start workers
	g_bMainLoop = 1;
	for (int i = 0; i < _worker_count; ++i)
	{
		if (_worker_pool[i]) {
			_worker_pool[i]->start();
		}
	}
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "%s startup. ", getEVServerType());
	//  int ms = 2000;
	//  while (g_bMainLoop)
	//  {
	//#ifdef _WIN32
	//	Sleep(ms);
	//#else
	//	usleep(ms * 1000);
	//#endif
	//  }
}
