#include "myassert.h"
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
	//网络初始化
#if defined(_WIN32)

	WSADATA wsaData;
	uint16 wVersionRequested = MAKEWORD(2, 2);
	int err = WSAStartup(wVersionRequested, &wsaData); 
	if (err != 0)
	{
		MyAssert(0);
	}
#endif

	// 时间
	_pTimeSystem = TimeSystem::getSinglePtr();
	MyAssert(_pTimeSystem);
	_pTimeSystem->start();

	// 日志
	_pLogSystem = LogSystem::getSinglePtr();
	MyAssert(_pLogSystem);
	ret = _pLogSystem->init(DEFAULT_LOG_CACHE_SIZE);
	MyAssert(ret);

	//// 场景管理器
	//_pSceneMgr = SceneMgr::getSinglePtr();
	//MyAssert(_pSceneMgr);
	//ret = _pSceneMgr->init();
	//MyAssert(ret);
	//LogSystem::getSinglePtr()->saveLogImmediately(LOG_FILE_SYSTEM, "SceneMgr init OK.");

	// 连接池
	int nMaxPlayerCount = 1024; // read from config
	_pGamePlayerPool = PlayerPool<GamePlayer>::getSinglePtr();
	MyAssert(_pGamePlayerPool);
	ret = _pGamePlayerPool ->init(nMaxPlayerCount);
	MyAssert(ret);
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "GamePlayerPool init OK.");
	
	// 包工厂管理器
	_pPacketFactoryMgr = PacketFactoryMgr::getSinglePtr();
	MyAssert(_pPacketFactoryMgr);
	ret = _pPacketFactoryMgr->init();
	MyAssert(ret);
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "PacketFactoryMgr init OK.");

	// 接入线程
	ret = _incomingThread.init(nMaxPlayerCount);
	MyAssert(ret);
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "IncomingThread init OK.");

	// 逻辑线程
	_pLogicThreadsMgr = LogicThreadMgr::getSinglePtr();
	MyAssert(_pLogicThreadsMgr);
	ret = _pLogicThreadsMgr->init();
	MyAssert(ret);
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
