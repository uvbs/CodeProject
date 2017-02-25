#include "share_memory_api.h"
#if defined (_LINUX64)
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <errno.h>
#elif defined (_WIN32)
#include <Windows.h>
#endif
#include "assert.h"
#include <stdio.h>

namespace ShareMemAPI
{

	SM_Handle	CreateShareMem(SM_Key key, uint size)
	{
		__ENTER_FUNCTION
#if  defined (_LINUX64)
		SM_Handle hd = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666); 
		printf("handle = %d ,key = %d ,size = %u, error: %d \r\n", hd, key, size, errno);
		return hd;

#elif defined (_WIN32)
		char keybuf[64] = {0};
		sprintf(keybuf, "%d", key);
		return CreateFileMapping((HANDLE)0xFFFFFFFFFFFFFFFF, NULL, PAGE_READWRITE, 0, size, (LPCWSTR)keybuf);
#endif
		__LEAVE_FUNCTION
		return SM_Handle(-1);
	}

	SM_Handle	OpenShareMem(SM_Key key, uint size)
	{
		__ENTER_FUNCTION
#if  defined (_LINUX64)
		SM_Handle hd = shmget(key , size,0);
		printf("handle = %d ,key = %d ,error: %d \r\n",hd,key,errno);
		return hd;

#elif defined (_WIN32)
		char keybuf[64];
		memset(keybuf,0,64);
		sprintf(keybuf,"%d",key);
		return OpenFileMapping(FILE_MAP_ALL_ACCESS, true, (LPCWSTR)keybuf);
#endif
		__LEAVE_FUNCTION
		return SM_Handle(-1);
	}

	char*	MapShareMem(SM_Handle handle)
	{
		__ENTER_FUNCTION
#if  defined (_LINUX64)
			return (char*)shmat(handle,0,0);

#elif defined (_WIN32)
			return (char*)MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
#endif
		__LEAVE_FUNCTION
		return 0;
	}

	void	UnMapShareMem(char* memoryPtr)
	{
		__ENTER_FUNCTION
#if  defined (_LINUX64)
			shmdt(memoryPtr);

#elif defined (_WIN32)
			UnmapViewOfFile(memoryPtr);
#endif
		__LEAVE_FUNCTION
	}

	void	CloseShareMem(SM_Handle handle)
	{
		__ENTER_FUNCTION
#if defined (_LINUX64)
			shmctl(handle, IPC_RMID, 0); 
#elif defined (_WIN32)
			CloseHandle(handle);
#endif
		__LEAVE_FUNCTION
	}

}

