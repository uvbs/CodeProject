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
	/*����ShareMem �ڴ���
	*	
	*	key   ����ShareMem �Ĺؼ�ֵ
	*
	*  Size  ������С
	*
	*	���� ��ӦShareMem����ֵ
	*/
	SM_Handle	CreateShareMem(SM_Key Key, uint size);


	/*��ShareMem �ڴ���
	*	
	* key   ��ShareMem �Ĺؼ�ֵ
	* 
	* Size  �򿪴�С
	*
	* ����  ��ӦShareMem ����ֵ
	*/
	SM_Handle	OpenShareMem(SM_Key Key, uint Size);


	/*ӳ��ShareMem �ڴ���
	*	
	*	handle ӳ��ShareMem �ı���ֵ
	*
	*  ���� ShareMem ������ָ��
	*/
	char*	MapShareMem(SM_Handle handle);


	/*�ر�ӳ�� ShareMem �ڴ���
	*
	*	MemoryPtr			ShareMem ������ָ��
	*	
	*  
	*/	
	void		UnMapShareMem(char* MemoryPtr);


	/*	�ر�ShareMem
	* 	handle  ��Ҫ�رյ�ShareMem ����ֵ
	*/
	void		CloseShareMem(SM_Handle handle);

}


#endif	//_SHARE_MEMORY_API_H

