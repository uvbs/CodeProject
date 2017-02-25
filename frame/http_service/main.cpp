#include "http_service.h"
#include "time_system.h"
#include "log.h"
#include <string>
using std::string;

int main()
{
	TimeSystem::getSinglePtr()->start();
	LogSystem::getSinglePtr()->init();
	const string IP = "127.0.0.1";
	int port = 8888;
	EVHttpServer s;
	if (!s.init(IP.c_str(), port)) {
		return -1;
	}
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "HttpServer listen at %s:%d\n", IP.c_str(), port);
	s.start();
	s.wait();
}

