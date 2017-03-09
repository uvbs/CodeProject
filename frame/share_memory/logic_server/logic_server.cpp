#ifdef _WIN32
#include "socket.h"
#endif
#include "logic_server.h"
#include "helper.h"
#include <stdio.h>

//���ļ���
#define FILE_DB_READ(data) \
    readSize = fread(&data, 1, sizeof(data), f);\
    LogSystem::getSinglePtr()->saveLog(LOG_FILE_DEBUG,"Read = %d, sizeof(%s) = %d", readSize, #data, sizeof(data)); \
    if(readSize == 0) \
    Assert(0);\
    totalReadSize += readSize; \

//д�ļ���
#define FILE_DB_WRITE(data) \
    writeSize = fwrite(&data, 1, sizeof(data), f);\
    LogSystem::getSinglePtr()->saveLog(LOG_FILE_DEBUG,"Write = %d, sizeof(%s) = %d", writeSize, #data, sizeof(data)); \
    if(writeSize == 0) \
    Assert(0);\

int LogicServer::_s_serial = 0;

LogicServer::LogicServer() : m_pTimeController(NULL), m_pLogSystem(NULL), 
    m_pHumanPool(NULL) {}

LogicServer::~LogicServer()
{
	SAFE_DELETE(m_pTimeController);
	SAFE_DELETE(m_pLogSystem);
	SAFE_DELETE(m_pHumanPool);
}

bool	LogicServer::init()
{
	// ʱ��ϵͳ
    m_pTimeController = TimeSystem::getSinglePtr();
	Assert(m_pTimeController);
	m_pTimeController->start();
	// ��־ϵͳ
    m_pLogSystem = LogSystem::getSinglePtr();
	Assert(m_pLogSystem);
	bool bRet = m_pLogSystem->init();
	Assert(bRet);
	// ��Ҷ����
	m_pHumanPool = ObjectHumanPool::getSinglePtr();
	Assert(m_pHumanPool);
	bRet = m_pHumanPool->init(1024);
    Assert(bRet);
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "ObjectHumanPool init OK.");

	return true;
}

bool  LogicServer::initUser(const char* user, const char* name, const char* dir, Guid& guid, FullUserData& value)
{
    guid.cleanUp();
    guid.init(1, ++_s_serial, EM_OBJ_TYPE_HUMAN);
    value._human_data._guid = guid;
    str_util::strcpy_s(value._human_data._user, user, sizeof(value._human_data._user));
    str_util::strcpy_s(value._human_data._char_name, name, sizeof(value._human_data._char_name));
    value._human_data._level = 1;
    value._human_data._money = 100;
    value._human_data._bag_size = MAX_BAG_SIZE; //����������
    value._human_data._equip_size = MAX_EQUIP_SIZE; //װ��������
    char file[256] = {0};
    str_util::snprintf(file, sizeof(file), "%s/USER-%llx.db", dir, guid.value());
    size_t writeSize = 0;
    FILE* f = fopen(file, "wb");
    if (f)
    {
        FILE_DB_WRITE(value._human_data);
        FILE_DB_WRITE(value._bag_data);
        FILE_DB_WRITE(value._equip_data);
        fclose(f);
        return true;
    }
    return false;
}

bool	LogicServer::loop()
{
    Guid guid;
    FullUserData data;
    if (!initUser("czfdlut@163.com", "Mr.chen", "./data/tmpdb/", guid, data)) {
        return false;
    }
    bool bNewSMU = false;
    ObjectHuman* pHuman = NULL;
    pHuman = ObjectHumanPool::getSinglePtr()->allocObject(guid, bNewSMU, pHuman);
    Assert(pHuman);
    pHuman->init(&data);

    while (true)
    {
        // logic
        sys_util::sleep(1000);
    }
	return true;
}

bool	LogicServer::exit( )
{
	LogSystem::getSinglePtr()->saveLog(LOG_FILE_INFO, "Exit LogicServer OK.");
	return true;
}