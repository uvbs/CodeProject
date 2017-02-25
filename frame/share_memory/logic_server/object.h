/**********************************
** @file: object.h
** @brief:  �������
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
	virtual void		sendFreshAttrs(){}	 //��������ˢ��

	//���ݲ���
	int			GetObjectID(){ return m_ObjectID; }
	void				SetObjectID(int id){ m_ObjectID = id; }
	
	int			GetManagerID() const{ return m_ObjectID;}
	void				SetManagerID(int id){ m_ObjectID = id; }

	int				GetPoolID() const{ return m_PoolID;}
	void				SetPoolID(int id){ m_PoolID = id;}

	//���ε���HeartBeat���ʱ���(����)
	uint				GetLogicTime() const { return (_curr_time-_last_time); }
	uint				GetCurrentTime() const { return _curr_time; }

protected:
	int			m_ObjectID;				//��������id
	int				m_PoolID;				//����ID
	uint	_create_time;			//�߼�����ʱ��
	uint	_last_time;			//��һ���߼�ʱ��
	uint	_curr_time;				//��ǰʱ��
	Timer _attr_fresh_timer;	//����ˢ�¼�ʱ��
};

#endif	//_OBJECT_H

