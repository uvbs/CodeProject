/**********************************
** @file: object.h
** @brief:  对象基类
** @autor: by Mr.Chen
** @date: 2016/07/23 16:09
**********************************/
#ifndef _OBJECT_H
#define _OBJECT_H

#include "dbstruct.h"
#include "timer.h"

class Object
{
public:
	Object();
	virtual ~Object();

	virtual bool		init();
	virtual void		cleanUp();
	virtual bool		heartBeat(uint uTime);
	virtual EM_OBJ_TYPE getObjType(){ return EM_OBJ_TYPE_INVALID; }
	virtual void		sendFreshAttrs(){}	 //发送属性刷新

	//数据部分
	int			GetObjectID(){ return m_ObjectID; }
	void				SetObjectID(int id){ m_ObjectID = id; }
	
	int			GetManagerID() const{ return m_ObjectID;}
	void				SetManagerID(int id){ m_ObjectID = id; }

	int				GetPoolID() const{ return m_PoolID;}
	void				SetPoolID(int id){ m_PoolID = id;}

	//两次调用HeartBeat间的时间差(毫秒)
	uint				GetLogicTime() const { return (_curr_time-_last_time); }
	uint				GetCurrentTime() const { return _curr_time; }

protected:
	int			m_ObjectID;				//管理器中id
	int				m_PoolID;				//池中ID
	uint	_create_time;			//逻辑创建时间
	uint	_last_time;			//上一次逻辑时间
	uint	_curr_time;				//当前时间
	Timer _attr_fresh_timer;	//属性刷新计时器
};

#endif	//_OBJECT_H

