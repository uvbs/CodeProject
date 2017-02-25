/**********************************
** @file: share_memory_obj.h
** @brief:  共享内存块对象
** @autor: by Mr.Chen
** @date: 2016/07/10 22:54
**********************************/
#ifndef _SHARE_MEM_OBJ_H
#define _SHARE_MEM_OBJ_H

#include "type.h"
#include "shm_def.h"
#include "shm_struct.h"

class ShareMemObj
{
public:
	ShareMemObj();
	~ShareMemObj() {}

	bool	create(SM_Key key, uint size);
	void	destroy();
	bool	attach(SM_Key key, uint size);
	bool	detach()	{return true;}
	char*	getDataPtr() const { return _dataPtr; }
	char*	getSMUPtr(uint size, uint index)
	{
		if (size <= 0 || size * index >= _size) { return NULL; }
		return _dataPtr + size * index;
	}
	uint	getSize() { return _size; }
	uint	getHeadVer() { return ((SMHead*)_header)->_head_ver; }
	void  setHeadVer(uint ver) { ((SMHead*)_header)->_head_ver = ver; }
	
private:
	uint	_size; //整个共享内存区大小(块+头)
	char*	_dataPtr; //共享内存块起始地址
	char*	_header; //共享内存头(共享内存块的管理信息)
	SM_Handle	_hold;
};


#endif	//_SHARE_MEM_OBJ_H

