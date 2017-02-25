#include "tcp_server.h"

int main()
{
	TcpSocketServer s;
	s.init();
	s.start();
	s.wait();

	return 0;
}
