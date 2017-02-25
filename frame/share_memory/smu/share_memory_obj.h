/**********************************
** @file: share_memory_obj.h
** @brief:  �����ڴ�����
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
	uint	_size; //���������ڴ�����С(��+ͷ)
	char*	_dataPtr; //�����ڴ����ʼ��ַ
	char*	_header; //�����ڴ�ͷ(�����ڴ��Ĺ�����Ϣ)
	SM_Handle	_hold;
};


#endif	//_SHARE_MEM_OBJ_H

