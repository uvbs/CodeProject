#include "socketdef.h"
#include "tcp_client.h"

int main()
{
	TcpSocketClient s;
	s.init();
	s.start();
	s.wait();

	return 0;
}
