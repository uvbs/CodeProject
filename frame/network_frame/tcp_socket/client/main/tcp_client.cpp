#include "socketdef.h"
#include "myassert.h"
#include "tcp_client.h"
#include "log.h"
#include "logic.h"
#include "packet_factory_mgr.h"

TcpSocketClient::~TcpSocketClient()
{
#if defined(_WIN32)
	WSACleanup();
#endif
	SAFE_DELETE(_pTimeSystem);
	SAFE_DELETE(_pLogSystem);
}

bool	TcpSocketClient::init()
{
	__ENTER_FUNCTION

	bool ret = false;
	//�����ʼ��
#if defined(_WIN32)

	WSADATA wsaData;
	uint16 wVersionRequested = MAKEWORD(2, 2);
	int err = WSAStartup(wVersionRequested, &wsaData); 
	if (err != 0)
	{
		MyAssert(0);
	}
#endif

	// ʱ��
	_pTimeSystem = TimeSystem::getSinglePtr();
	MyAssert(_pTimeSystem);
	_pTimeSystem->start();

	// ��־
	_pLogSystem = LogSystem::getSinglePtr();
	MyAssert(_pLogSystem);
	ret = _pLogSystem->init(DEFAULT_LOG_CACHE_SIZE);
	MyAssert(ret);

	// ������������
	_pPacketFactoryMgr = PacketFactoryMgr::getSinglePtr();
	MyAssert(_pPacketFactoryMgr);
	_pPacketFactoryMgr->init();

	// �߼��߳�
	ret = _logicThreadsMgr.init();
	MyAssert(ret);
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "LogicThreadsMgr init OK.");

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool	TcpSocketClient::start()
{
	_logicThreadsMgr.start();

	return true;
}

bool	TcpSocketClient::wait()
{
	_logicThreadsMgr.wait();
	return true;
}
