/********************************************************************
	CREATED:	2012/01/18
	FILENAME:	State.h
	AUTHOR:	By Mr.chen
	
	PURPOSE:	定义各个状态类
*********************************************************************/
#ifndef _STATE_H
#define _STATE_H

namespace state_pattern
{

class Work;

/**
	状态虚基类
*/
class State
{
public:
	virtual void writeProgram(Work*) = 0;
};


/**
	上午工作状态
*/
class ForenoonSate : public State
{
public:
	virtual void writeProgram(Work*);
};


/**
	中午工作状态
*/
class NoonState : public State
{
public:
	virtual void writeProgram(Work*);
};


/**
	下午工作状态
*/
class AfternoonState : public State
{
public:
	virtual void writeProgram(Work*);
};


/**
	晚间工作状态
*/
class EveningState : public State
{
public:
	virtual void writeProgram(Work*);
};


/**
	睡眠状态
*/
class SleepingState : public State
{
public:
	virtual void writeProgram(Work*);
};


/**
	下班休息状态
*/
class RestState : public State
{
public:
	virtual void writeProgram(Work*);
};

} // namespace state_pattern

#endif //_STATE_H
