#include "share_memory_pool.h"

//特化
template<>
HumanSMU* ShareMemPool<HumanSMU>::newObj()
{
	for (int i = 0; i < _maxSize; i++)
	{
		HumanSMU* pSMU = _ppObj[i];
		if (pSMU && pSMU->getUseStatus(SM_S_READ) == SM_USE_FREE) {
			return pSMU;
		}
	}
	return NULL;
}

//特化
template<>
HumanSMU* ShareMemPool<HumanSMU>::getAvailableObj(Guid guid)
{
	__ENTER_FUNCTION
	if (!guid.isValid()) { return NULL; }
	for (int i = 0; i < _maxSize; i++)
	{
		HumanSMU* pSMU = _ppObj[i];
		if (!pSMU) { continue; }
		int useStatus = pSMU->getUseStatus(SM_S_READ);
		if ((SM_USE_READYFREE == useStatus || SM_USE_HOLDDATA == useStatus) && 
			pSMU->_SMU_data._human_data._guid==guid) {
			return pSMU;
		}
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

//特化
template<>
MailSMU* ShareMemPool<MailSMU>::getAvailableObj(Guid guid)
{
	__ENTER_FUNCTION
	
	if (!guid.isValid()) { return NULL; }
	for (int uIndex=0;uIndex<_maxSize;uIndex++)
	{
		MailSMU* pSMU = _ppObj[uIndex];
		if (!pSMU) { continue; }
		int useStatus = pSMU->getUseStatus(SM_S_READ);
		if ((SM_USE_READYFREE == useStatus || SM_USE_HOLDDATA == useStatus) && 
			pSMU->_SMU_data._owner==guid) {
			return pSMU;
		}
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}
