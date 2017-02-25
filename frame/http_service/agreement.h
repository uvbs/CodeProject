/***************************************************
** @file: agreement.h
** @author: Mr.Chen
** @date: 2016/7/7
** @brief: 协议解析
***************************************************/
#ifndef _AGREEMENT_H
#define _AGREEMENT_H

#include "type.h"
#include "thread.h"
#include "mcHash.h"

class Agreement
{
public:
	typedef mcHashMap<class HashStr, char*> HashArgv;
	void	parser(char* uri);

private:
	void	_loadArgv(HashArgv *hash, char* data);	// 读取Url的各个参数,插入hash
	int	  _type,  _flag;
	HashArgv _argv;
};

#endif // _AGREEMENT_H


