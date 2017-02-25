#include "assert.h"
#include "tcp_server.h"
#include "log.h"
#include "logic.h"


TcpSocketServer::~TcpSocketServer()
{
#if defined(_WIN32)
	WSACleanup();
#endif
	SAFE_DELETE(_pTimeSystem);
	SAFE_DELETE(_pLogSystem);
	SAFE_DELETE(_pPacketFactoryMgr);
	SAFE_DELETE(_pGamePlayerPool);
}

bool	TcpSocketServer::init()
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

	//// ����������
	//_pSceneMgr = SceneMgr::getSinglePtr();
	//Assert(_pSceneMgr);
	//ret = _pSceneMgr->init();
	//Assert(ret);
	//LogSystem::getSinglePtr()->saveLogImmediately(LOG_FILE_SYSTEM, "SceneMgr init OK.");

	// ���ӳ�
	int nMaxPlayerCount = 1024; // read from config
	_pGamePlayerPool = PlayerPool<GamePlayer>::getSinglePtr();
	Assert(_pGamePlayerPool);
	ret = _pGamePlayerPool ->init(nMaxPlayerCount);
	Assert(ret);
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "GamePlayerPool init OK.");
	
	// ������������
	_pPacketFactoryMgr = PacketFactoryMgr::getSinglePtr();
	Assert(_pPacketFactoryMgr);
	ret = _pPacketFactoryMgr->init();
	Assert(ret);
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "PacketFactoryMgr init OK.");

	// �����߳�
	ret = _incomingThread.init(nMaxPlayerCount);
	Assert(ret);
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "IncomingThread init OK.");

	// �߼��߳�
	_pLogicThreadsMgr = LogicThreadMgr::getSinglePtr();
	Assert(_pLogicThreadsMgr);
	ret = _pLogicThreadsMgr->init();
	Assert(ret);
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "LogicThreadsMgr init OK.");

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool	TcpSocketServer::start()
{
	_incomingThread.start();
	_pLogicThreadsMgr->start();

	return true;
}

bool	TcpSocketServer::wait()
{
	_incomingThread.wait();
	_pLogicThreadsMgr->wait();

	return true;
}
