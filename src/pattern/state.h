/********************************************************************
	CREATED:	2012/01/18
	FILENAME:	State.h
	AUTHOR:	By Mr.chen
	
	PURPOSE:	�������״̬��
*********************************************************************/
#ifndef _STATE_H
#define _STATE_H

namespace state_pattern
{

class Work;

/**
	״̬�����
*/
class State
{
public:
	virtual void writeProgram(Work*) = 0;
};


/**
	���繤��״̬
*/
class ForenoonSate : public State
{
public:
	virtual void writeProgram(Work*);
};


/**
	���繤��״̬
*/
class NoonState : public State
{
public:
	virtual void writeProgram(Work*);
};


/**
	���繤��״̬
*/
class AfternoonState : public State
{
public:
	virtual void writeProgram(Work*);
};


/**
	��乤��״̬
*/
class EveningState : public State
{
public:
	virtual void writeProgram(Work*);
};


/**
	˯��״̬
*/
class SleepingState : public State
{
public:
	virtual void writeProgram(Work*);
};


/**
	�°���Ϣ״̬
*/
class RestState : public State
{
public:
	virtual void writeProgram(Work*);
};

} // namespace state_pattern

#endif //_STATE_H
