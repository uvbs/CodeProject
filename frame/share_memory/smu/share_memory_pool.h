/**********************************
** @file: share_memory_pool.h
** @brief:  
** @autor: by Mr.Chen
** @date: 2016/07/10 23:44
**********************************/
#ifndef _SHARE_MEM_POOL_H
#define _SHARE_MEM_POOL_H

#include "share_memory_obj.h"
#include "shm_def.h"
#include "macrodef.h"
#include "myassert.h"


template<typename T>
class ShareMemPool
{
public:
	ShareMemPool() : _ppObj(0), _pRefObjPtr(NULL), _maxSize(0) {}
	~ShareMemPool()
	{
		if (_pRefObjPtr) { _pRefObjPtr->destroy(); }
		SAFE_DELETE(_pRefObjPtr);
		SAFE_DELETE_ARRAY(_ppObj);
	}
	//创建内存池（ShareMemory创建，LogicServer映射）
	virtual bool init(uint count, SM_Key key, SMPOOL_TYPE SMPT)
	{
		__ENTER_FUNCTION
		_pRefObjPtr = new ShareMemObj();
		if (!_pRefObjPtr) { return false; }
		bool ret = _pRefObjPtr->attach(key, sizeof(T) * count + sizeof(SMHead));
		if (!ret)
		{
			if (SMPT == SM_ShareMemory)
			{
				ret = _pRefObjPtr->create(key, sizeof(T) * count + sizeof(SMHead));
				if (!ret) { return false; }
			}
			else { return false; }
		}
		_maxSize = count;
		_ppObj = new T*[_maxSize];
		if (!_ppObj) { return false; }
		//保存每块地址
		for (int i = 0; i < _maxSize; i++)
		{
			T* pSMUObj = reinterpret_cast<T*>(_pRefObjPtr->getSMUPtr(sizeof(T), i));
			_ppObj[i] = pSMUObj;
			if (!_ppObj[i]) { return false; }
			if (SMPT == SM_ShareMemory) {
				_ppObj[i]->cleanUp();
			}
		}
		_key = key;
		return true;
		__LEAVE_FUNCTION
		return false;
	}
	bool finalize()
	{
		if (_pRefObjPtr) {
			_pRefObjPtr->destroy();
		}
		return true;
	}
	T* newObj()
	{
		for (int i = 0;i < _maxSize; i++)
		{
			T* pSMU = _ppObj[i];
			if (pSMU && pSMU->getUseStatusFast() == SM_USE_FREE) {
				return pSMU;
			}
		}
		return NULL;
	}
	T*	getAvailableObj(Guid guid)  { return NULL; }
	void delObj(T* pObj)
	{
		if (pObj) {
			pObj->setUseStatus(SM_USE_FREE, SM_C_WRITE);
		}
	}
	//X_VOID PreDeleteObject(T* pObj)
	//{
	//	__ENTER_FUNCTION
	//	pObj->SetUseStatus(SM_USE_FREED,SM_C_WRITE);
	//	__LEAVE_FUNCTION			
	//}
	T* getSMUByIndex(int i)
	{
		if (i >= _maxSize) {
			return NULL;
		}
		return _ppObj[i];
	}
	int		getPoolSize() { return _maxSize; }
	SM_Key getKey() { return _key; }
	uint getHeadVer()
	{
		MyAssert(_pRefObjPtr);
		return _pRefObjPtr->getHeadVer();
	}
	void setHeadVer(uint ver)
	{
		if (_pRefObjPtr) {
			_pRefObjPtr->setHeadVer(ver);
		}
	}
	void  updateHeadVer()
	{
		uint uVer = getHeadVer();
		setHeadVer(++uVer);
	}
	
public:
	ShareMemObj* getShareMemObj(){return _pRefObjPtr;}
	
private:
	ShareMemObj*	_pRefObjPtr; //用于创建共享内存
	T **_ppObj; //共享内存块地址
	int	 _maxSize; //共享内存块数
	SM_Key	 _key; 
};


#endif	//_SHARE_MEM_POOL_H
