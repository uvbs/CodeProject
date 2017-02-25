/**********************************
** @file: shm_util.h
** @brief:  
** @autor: by Mr.Chen
** @date: 2016/07/11 23:59
**********************************/
#ifndef _SHM_UTIL_H
#define _SHM_UTIL_H

void	sm_lock(char&	flag, char type);
void	sm_unlock(char& flag);
bool	sm_trylock(char& flag, char type);

#endif	//_SHM_UTIL_H

