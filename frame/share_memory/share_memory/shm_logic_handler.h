/**********************************
** @file: shm_logic_handler.h
** @brief:  把共享内存块数据落地
** @autor: by Mr.Chen
** @date: 2016/07/13 23:52
**********************************/
#ifndef _SHM_LOGIC_HANDLE_H
#define _SHM_LOGIC_HANDLE_H

#include "share_memory_pool.h"
#include "shm_def.h"
#include "assert.h"
#include "time_system.h"
#include "dbhelper.h"

template <class T>
class ShareMemLogicHandler
{
public:
	ShareMemLogicHandler() : _shm_pool(NULL), _old_check_time(0),
        _old_ver(0), _cmd_type(CMD_UNKNOW) {}
	~ShareMemLogicHandler() {}
	
public:
	virtual bool	init(ShareMemPool<T>* pSMUPool)
	{
		__ENTER_FUNCTION
		if (!pSMUPool) { return false; }
		_shm_pool = pSMUPool;
		_shm_pool->setHeadVer(0);//此处是否会有bug,以后待证
		_old_check_time = TimeSystem::getSinglePtr()->getRunTime();
		_old_ver = 0;
		return doPostInit();
		__LEAVE_FUNCTION
		return false;
	}
	bool heartBeat()
	{
		__ENTER_FUNCTION
		Assert(_shm_pool);
		uint uTime = TimeSystem::getSinglePtr()->getRunTime();
		if ((uTime - _old_check_time) > MAX_SERVER_IDLE_TIME) // 关闭了LogicServer
		{
			_old_check_time = uTime;
			uint ver = _shm_pool->getHeadVer();
			if (ver == _old_ver && _old_ver > 0)
			{
				_old_ver = 0;
				bool bRet = doSaveAll();
				//清理对应的数据标志
				//DoClear();
				_shm_pool->setHeadVer(0);
				return bRet;
			}
			_old_ver = ver;
		}
		switch (_cmd_type)
		{
		case CMD_SAVE_ALL: { return doSaveAll(); } break;
		case CMD_CLEAR_ALL: { return doClear(); } break;
		case CMD_UNKNOW: { return doNormalSave(); } break;
		default: { return false; }
		}
		__LEAVE_FUNCTION
		return false;
	}

public:
    void  setCmdType(SM_COMMANDS cmd) { _cmd_type = cmd; }
    SM_COMMANDS getCmdType() { return _cmd_type; }
	bool	doSaveAll() { return true; }
	bool	doNormalSave() { return true; }
	bool	doPostInit() { return true; }
	bool	doClear() { return true; }
	
private:
	ShareMemPool<T>*	_shm_pool;
	uint	_old_check_time;
	uint	_old_ver;
    SM_COMMANDS _cmd_type;
};


///////////////////////////////////////////////////////////////
#include "shm_logic_handler.inl"
////////////////////////////////////////////////////////////////


#endif	//_SHM_LOGIC_HANDLE_H

