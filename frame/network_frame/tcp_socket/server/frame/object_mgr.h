////////////////////////////////////////////////////////////////////////////////////
// 
// @file: object_mgr.h
// @author: by robert
// @date: 2014/5/26	15:20
// @brief: ���������
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __OBJECT_MGR_H_
#define __OBJECT_MGR_H_

#include "type.h"
#include "assert.h"
#include "macrodef.h"

template<class T> 
class ObjectMgr
{
public:
	ObjectMgr()
	{
		_capacity = 0;
		_size = 0;
		_ppObjList = NULL;
	}

	virtual ~ObjectMgr()
	{
		SAFE_DELETE_ARRAY(_ppObjList);
	}

	//��ʼ��
	virtual bool init(int count)
	{
		__ENTER_FUNCTION
		
		Assert(_ppObjList == NULL);
		if (count <= 0 )
		{
			return false;
		}
		_size = 0;
		_capacity = count;
		_ppObjList = new T*[_capacity];
		for (int i = 0; i < _capacity; ++i) 
		{
			_ppObjList[i] = NULL;
		}
		return true;
		__LEAVE_FUNCTION
		return false;
	}

	//����
	virtual void cleanUp()
	{
		_size = 0;
		for (int i = 0; i < _capacity; ++i) 
		{
			_ppObjList[i] = NULL;
		}
	}

	//�߼��ӿ�
	virtual bool heartBeat(uint uTime)
	{
		return true;
	}

	//����
	bool addObj(T* pObject)
	{
		Assert(pObject);
		if (_size >= _capacity)
		{
			return false;
		}
		_ppObjList[_size] = pObject;
		pObject->setMgrID(_size);
		++_size;
		return true;
	}

	//ɾ��
	bool removeObj(T* pObject)
	{
		Assert(pObject);
		if (_size <= 0)
		{
			return false;
		}
		int index = pObject->getMgrID();
		if (index < 0 || index >= _size)
		{
			return false;
		}
		if (_ppObjList[index] != pObject)
		{
			return false;
		}
		//erase
		if (index != _size -1)
		{
			_ppObjList[index] = _ppObjList[_size - 1];
			if (_ppObjList[index] != NULL)
			{
				_ppObjList[index]->setMgrID(index);
			}
		}
		_ppObjList[_size -1] = NULL;
		pObject->setMgrID(-1);
		--_size;

		return true;
	}

	//�������
	int getObjNum() { return _size ; }
	int getCapacity() { return _capacity; }

	//���
	T* getObjByIndex(int index)
	{
		if (index < 0 || index>= _size)
		{
			return NULL;
		}
		return _ppObjList[index];
	}

protected:
	int	 _capacity;
	int _size;
	T** _ppObjList;
};

#endif
