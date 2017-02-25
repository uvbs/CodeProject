#include "socketdef.h"
#include "assert.h"
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
		Assert(0);
	}
#endif

	// ʱ��
	_pTimeSystem = TimeSystem::getSinglePtr();
	Assert(_pTimeSystem);
	_pTimeSystem->start();

	// ��־
	_pLogSystem = LogSystem::getSinglePtr();
	Assert(_pLogSystem);
	ret = _pLogSystem->init(DEFAULT_LOG_CACHE_SIZE);
	Assert(ret);

	// ������������
	_pPacketFactoryMgr = PacketFactoryMgr::getSinglePtr();
	Assert(_pPacketFactoryMgr);
	_pPacketFactoryMgr->init();

	// �߼��߳�
	ret = _logicThreadsMgr.init();
	Assert(ret);
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
