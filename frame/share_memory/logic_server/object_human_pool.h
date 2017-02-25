/**********************************
** @file: object_human_pool.h
** @brief:  
** @autor: by Mr.Chen
** @date: 2016/07/23 22:54
**********************************/
#ifndef _OBJECT_HUMAN_POOL_H
#define _OBJECT_HUMAN_POOL_H

#include "macrodef.h"
#include "singleton.h"
#include "object_human.h"
#include "share_memory_pool.h"
#include "lock.h"
#include "hash_table.h"

class ObjectHumanPool : public Singleton<ObjectHumanPool>
{
public:
	ObjectHumanPool();
	~ObjectHumanPool();

	bool init(int count) ;
	ObjectHuman* getObject(const Guid& guid);
	ObjectHuman* allocObject(Guid guid, bool& bNewSMU, ObjectHuman* pObjectHuman = NULL) ;
	bool recycleObject(const Guid& guid) ;

private:
	MLock _lock ;	 //��
	ObjectHuman* _human_pool;	 //�����
	int	 _capacity; //��С
	ShareMemPool<HumanSMU>* _human_sm_pool; //�����
	HashTable<uint64, int>* _id_table;  //���GUID����ҳ�id��ӳ���
};

#endif	//_OBJECT_HUMAN_POOL_H

