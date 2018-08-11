#ifdef _WIN32
#include "socket.h"
#endif
#include "object_human_pool.h"

WX_IMPLEMENT_SINGLEON(ObjectHumanPool);

ObjectHumanPool::ObjectHumanPool()
{
	_human_sm_pool = new ShareMemPool<HumanSMU>;
	MyAssert(_human_sm_pool);
}

ObjectHumanPool::~ObjectHumanPool()
{
	SAFE_DELETE_ARRAY(_human_pool);
	SAFE_DELETE(_human_sm_pool);
}

bool	ObjectHumanPool::init(int nCount)
{
	MyAssert(nCount > 0);
	_human_pool = new  ObjectHuman[nCount];
	MyAssert(_human_pool);
	int humanKey = 0x1000;
	bool bRet = _human_sm_pool->init(nCount, humanKey, SM_LogicServer);
	MyAssert(bRet);
	_capacity = nCount;
	_id_table = new HashTable<uint64, int>(nCount);
	MyAssert(_id_table);
	return true;
}

ObjectHuman* ObjectHumanPool::getObject(const Guid& guid)
{
	MyAssert(_human_pool);
	if (!guid.isValid()) { return NULL; }
	const int* pID = _id_table->find(guid.value());
	if (!pID) { return NULL; }
	int pos = *pID;
	if (pos < 0 || pos >= _capacity) { return NULL; }
	if (_human_pool[pos].getGuid() != guid) { return NULL; }
	return &_human_pool[pos];
}

ObjectHuman* ObjectHumanPool::allocObject(Guid guid, bool& bNewSmuObj, ObjectHuman* pObjectHuman/*=X_NULL*/)
{
	__ENTER_FUNCTION
	AutoMLock autolock(_lock);
	MyAssert(_human_pool);
	MyAssert(_human_sm_pool);

	bNewSmuObj = false;
	if (NULL== pObjectHuman)
	{
		for (int i = 0;i < _capacity; ++i)
		{
			if (!_human_pool[i].getGuid().isValid())
			{
				_human_pool[i].cleanUp();
				_human_pool[i].SetPoolID(i);
				_human_pool[i].setGuid(guid);
				pObjectHuman = &_human_pool[i];
				_id_table->insert(guid.value(), i);
				break;
			}	
		}
	}
	HumanSMU* pSMU = _human_sm_pool->getAvailableObj(guid);
	if (!pSMU)
	{
		pSMU = _human_sm_pool->newObj();
		bNewSmuObj = true;
	}
	MyAssert(pSMU);
	pSMU->_SMU_header.clearVersion();

	pObjectHuman->setSMU(pSMU);
	pObjectHuman->getHumanDB()->setFreeOwnFlag(SM_USE_HOLDDATA);
	
	return pObjectHuman;
	__LEAVE_FUNCTION
	return NULL ;
}

bool	ObjectHumanPool::recycleObject(const Guid& guid)
{
	AutoMLock autolock(_lock);
	if (!guid.isValid()) { return false; }
	ObjectHuman* pObjectHuman = getObject(guid);
	if (!pObjectHuman) { return false; }
    bool bErase = _id_table->erase(guid.value());
	if (!bErase) { return false; }

	HumanSMU* pSMU = pObjectHuman->getSMU();
	MyAssert(pSMU);
	pObjectHuman->getHumanDB()->setFreeOwnFlag(SM_USE_READYFREE);
	pObjectHuman->cleanUp();
	return true;
}
