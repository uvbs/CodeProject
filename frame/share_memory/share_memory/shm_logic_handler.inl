/**********************************
** @file: shm_logic_handler.inl
** @brief:  Ä£°åÌØ»¯
** @autor: by Mr.Chen
** @date: 2016/07/16 13:37
**********************************/
//#ifndef SHM_LOGIC_HANDLE_INL_
//#define SHM_LOGIC_HANDLE_INL_

#include "shm_logic_handler.h"
#include "dbhelper.h"

///###################################
/// ¿¿¿¿¿¿
///###################################
template<>
bool    ShareMemLogicHandler<HumanSMU>::doNormalSave()
{
    __ENTER_FUNCTION
    if (!_shm_pool) { return false; }
    int nPoolSize = _shm_pool->getPoolSize();       
    FullUserData userdata;
    for (int i = 0; i < nPoolSize; i++)
    {
        HumanSMU* pSMU = _shm_pool->getSMUByIndex(i);
        if (!pSMU) { continue;}
        int useStatus = pSMU->getUseStatus(SM_C_READ);
        uint time = TimeSystem::getSinglePtr()->getRunTime();
        if ((useStatus == SM_USE_HOLDDATA && time > pSMU->getTime2Save() && pSMU->isDataChanged())
            || useStatus==SM_USE_READYFREE)
        {
            if (!pSMU->tryCopyDataFromSHM(SM_C_WRITE, &userdata)) { continue; }
            if (!userdata._human_data._guid.isValid()) { continue; }
            bool ret = false;
            if (useStatus == SM_USE_READYFREE)
            {
                if (!pSMU->_SMU_header._free_timer.isSet())
                {
                    ret = db_helper::saveUserData(userdata);
                    pSMU->_SMU_header._free_timer.beginTimer(kUserHoldTime, time);
                }
                else
                {
                    if (pSMU->_SMU_header._free_timer.isReach(time))
                    {
                        _shm_pool->delObj(pSMU);
                        pSMU->_SMU_header._free_timer.endTimer();
                    }
                }
            }
            else
            {
                ret = db_helper::saveUserData(userdata);
            }
            if (ret)
            {
                pSMU->updateOldVer();
                pSMU->setTime2Save(time + kUserSaveInterval);
            }
        }
    }
    return true;
    __LEAVE_FUNCTION
    return false;
}

template<>
bool ShareMemLogicHandler<HumanSMU>::doSaveAll()
{
    __ENTER_FUNCTION
    if (!_shm_pool) { return false; }
    int nPoolSize = _shm_pool->getPoolSize();
    FullUserData userdata;
    for (int i = 0; i < nPoolSize; i++)
    {
        HumanSMU* pSMU = _shm_pool->getSMUByIndex(i);
        if (!pSMU) { continue; }
        int useStatus = pSMU->getUseStatusFast();
        uint time = TimeSystem::getSinglePtr()->getRunTime();
        if (useStatus == SM_USE_READYFREE ||
            useStatus == SM_USE_FREED ||
            useStatus == SM_USE_HOLDDATA)
        {
            if (!pSMU->tryCopyDataFromSHM(SM_C_WRITE, &userdata)) { continue; }
            if (!userdata._human_data._guid.isValid()) { continue; }
            bool ret = db_helper::saveUserData(userdata);
            if (ret)
            {
                pSMU->updateOldVer();
                pSMU->setTime2Save(time + kUserSaveInterval);
                if (useStatus == SM_USE_READYFREE)
                {
                    _shm_pool->delObj(pSMU);                
                }
            }
        }
    }
    return true;
    __LEAVE_FUNCTION
    return false;
}

template<>
bool    ShareMemLogicHandler<HumanSMU>::doClear()
{
    __ENTER_FUNCTION
    if (!_shm_pool) { return false; }
    int nMaxPoolSize = _shm_pool->getPoolSize();
    for (int i = 0; i < nMaxPoolSize; i++)
    {
        HumanSMU* pSMU = _shm_pool->getSMUByIndex(i);
        if (!pSMU) { continue; }
        int useStatus = pSMU->getUseStatus(SM_C_WRITE);
        _shm_pool->delObj(pSMU);
    }
    return true;
    __LEAVE_FUNCTION
    return false;
}

///###################################
/// SerialID¿¿
///###################################
template<>
bool    ShareMemLogicHandler<SerialSMU>::doNormalSave()
{
    __ENTER_FUNCTION
    if (!_shm_pool) { return false; }
    int poolSize = _shm_pool->getPoolSize();
    SerialData serialdata;
    for (int i = 0; i < poolSize; i++)
    {
        SerialSMU* pSMU = _shm_pool->getSMUByIndex(i);
        if (!pSMU) { continue; }
        int useStatus = pSMU->getUseStatus(SM_C_READ);
        uint time = TimeSystem::getSinglePtr()->getRunTime();
        if (useStatus == SM_USE_HOLDDATA && time > pSMU->getTime2Save() && pSMU->isDataChanged())
        {
            if (!pSMU->tryCopyDataFromSHM(SM_C_WRITE, &serialdata)) { continue; }
            bool ret = 1;//myServer.m_pDBInterface->SaveSerialData(serialdata.m_nObjType, serialdata);
            //X_BOOL bRet = DBInterface::GetInstance()->SaveSerialData(serialdata.m_nObjType, serialdata);
            if (ret)
            {
                pSMU->updateOldVer();
                pSMU->setTime2Save(time + kUserSaveInterval);
            }           
        }           
    }
    return true;
    __LEAVE_FUNCTION
    return false;
}

template<>
bool ShareMemLogicHandler<SerialSMU>::doSaveAll()
{
    __ENTER_FUNCTION
    if (!_shm_pool) { return false; }
    int poolSize = _shm_pool->getPoolSize();
    SerialData serialdata;
    for (int i = 0 ; i < poolSize; i++)
    {
        SerialSMU* pSMU = _shm_pool->getSMUByIndex(i);
        if (!pSMU) { continue; }
        int useStatus = pSMU->getUseStatusFast();
        uint time = TimeSystem::getSinglePtr()->getRunTime();
        if (useStatus == SM_USE_HOLDDATA)
        {
            if (!pSMU->tryCopyDataFromSHM(SM_C_WRITE, &serialdata)) { continue; }
            bool ret = 1;//myServer.m_pDBInterface->SaveSerialData(serialdata.m_nObjType, serialdata);
            //X_BOOL bRet = DBInterface::GetInstance()->SaveSerialData(serialdata.m_nObjType, serialdata);
            if (ret)
            {
                pSMU->updateOldVer();
                pSMU->setTime2Save(time + kUserSaveInterval);
            }
        }
    }
    return true;
    __LEAVE_FUNCTION
    return false;
}


///###################################
/// ¿¿¿¿
///###################################
template<>
bool    ShareMemLogicHandler<MailSMU>::doNormalSave()
{
    __ENTER_FUNCTION
    if (!_shm_pool) { return false; }
    int poolSize = _shm_pool->getPoolSize();
    MailBoxData data;
    for (int i = 0; i < poolSize; i++)
    {
        MailSMU* pSMU = _shm_pool->getSMUByIndex(i);
        if (!pSMU || pSMU->getLogicIndex() < 0) { continue; }
        uint time = TimeSystem::getSinglePtr()->getRunTime();
        int useStatus = pSMU->getUseStatus(SM_C_READ);
        if ((useStatus==SM_USE_HOLDDATA && time > pSMU->getTime2Save() && pSMU->isDataChanged()) 
            || useStatus==SM_USE_READYFREE)
        {
            if (!pSMU->tryCopyDataFromSHM(SM_C_WRITE, &data)) { continue; }
            if (!data._owner.isValid()) { continue; }
            bool bRet = false;
            if (useStatus == SM_USE_READYFREE)
            {
                if (!pSMU->_SMU_header._free_timer.isSet())
                {
                    //bRet = myServer.m_pDBInterface->SaveMailBoxData(data.m_OwnerGuid, data);
                    pSMU->_SMU_header._free_timer.beginTimer(kUserHoldTime, time);
                }
                else
                {
                    if (pSMU->_SMU_header._free_timer.isReach(time))
                    {
                        _shm_pool->delObj(pSMU);
                        pSMU->_SMU_header._free_timer.endTimer();
                    }
                }
            }
            else
            {
                //bRet = myServer.m_pDBInterface->SaveMailBoxData(data.m_OwnerGuid, data);
            }
            if (bRet)
            {
                pSMU->updateOldVer();
                pSMU->setTime2Save(time + kUserSaveInterval);
            }               
        }           
    }
    return true;
    __LEAVE_FUNCTION
    return false;
}

template<>
bool    ShareMemLogicHandler<MailSMU>::doSaveAll()
{
    __ENTER_FUNCTION
    if (!_shm_pool) { return false; }
    int poolSize = _shm_pool->getPoolSize();
    MailBoxData data;
    for (int i = 0 ; i < poolSize; i++)
    {
        MailSMU* pSMU = _shm_pool->getSMUByIndex(i);
        if (!pSMU || pSMU->getLogicIndex() < 0) { continue; }
        uint time = TimeSystem::getSinglePtr()->getRunTime();
        int useStatus = pSMU->getUseStatusFast();
        if (useStatus == SM_USE_HOLDDATA || useStatus == SM_USE_READYFREE)
        {
            if (!pSMU->tryCopyDataFromSHM(SM_C_WRITE, &data)) { continue; }
            if (!data._owner.isValid()) { continue; }
            bool ret = 1;//myServer.m_pDBInterface->SaveMailBoxData(data.m_OwnerGuid, data);
            //X_BOOL bRet =DBInterface::GetInstance()->SaveMailBoxData(data.m_OwnerGuid, data);
            pSMU->setTime2Save(time + kUserSaveInterval);
            if (useStatus == SM_USE_READYFREE)
            {
                _shm_pool->delObj(pSMU);                
            }
        }
    }
    return true;
    __LEAVE_FUNCTION
    return false;
}

//#endif

