/**********************************
** @file: share_memory_api.h
** @brief:  
** @autor: by Mr.Chen
** @date: 2016/07/10 22:00
**********************************/
#ifndef _SHARE_MEM_API_H
#define _SHARE_MEM_API_H

#include "type.h"
#include "shm_def.h"

namespace ShareMemAPI
{
	/*创建ShareMem 内存区
	*	
	*	key   创建ShareMem 的关键值
	*
	*  Size  创建大小
	*
	*	返回 对应ShareMem保持值
	*/
	SM_Handle	CreateShareMem(SM_Key Key, uint size);


	/*打开ShareMem 内存区
	*	
	* key   打开ShareMem 的关键值
	* 
	* Size  打开大小
	*
	* 返回  对应ShareMem 保持值
	*/
	SM_Handle	OpenShareMem(SM_Key Key, uint Size);


	/*映射ShareMem 内存区
	*	
	*	handle 映射ShareMem 的保持值
	*
	*  返回 ShareMem 的数据指针
	*/
	char*	MapShareMem(SM_Handle handle);


	/*关闭映射 ShareMem 内存区
	*
	*	MemoryPtr			ShareMem 的数据指针
	*	
	*  
	*/	
	void		UnMapShareMem(char* MemoryPtr);


	/*	关闭ShareMem
	* 	handle  需要关闭的ShareMem 保持值
	*/
	void		CloseShareMem(SM_Handle handle);

}


#endif	//_SHARE_MEMORY_API_H

