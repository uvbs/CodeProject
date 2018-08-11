#include "shm_util.h"
#include "shm_def.h"
#include "helper.h"
#include "myassert.h"
#include <stdio.h>

void	sm_lock(char& flag, char type)
{	
	__ENTER_FUNCTION
	while (true)
	{
		if (flag == SM_FREE)
		{
			flag = type;
			if (flag != type)
			{
				sys_util::sleep(1);
				printf("lock fail %s,%d,%s", __FILE__, __LINE__, __FUNCTION__);		
				continue;
			}
			else
			{
				return;
			}
		}
		else
		{
			sys_util::sleep(1);
			continue;
		}		
	}
	__LEAVE_FUNCTION
}

void	sm_unlock(char& flag)
{
	__ENTER_FUNCTION
	while (true)
	{
		if (flag == SM_FREE)
		{
			return;
		}
		flag = SM_FREE;
		if (flag != SM_FREE)
		{
			sys_util::sleep(1);
			printf("unlock fail %s,%d,%s",__FILE__, __LINE__, __FUNCTION__);			
			continue;
		}
		else
		{
			return;
		}
	}
	__LEAVE_FUNCTION
}

bool	sm_trylock(char& flag, char type)
{
	__ENTER_FUNCTION
	int lockTimes=0;
	while (true)
	{
		if (flag == SM_FREE)
		{
			flag = type;
			if (flag != type)
			{
				lockTimes++;
				sys_util::sleep(1);
				if (lockTimes > 10)
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			lockTimes++;
			sys_util::sleep(1);
			if (lockTimes > 10)
			{
				return false;
			}
		}
	}
	__LEAVE_FUNCTION
	return false;
}
