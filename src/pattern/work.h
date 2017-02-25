/********************************************************************
	CREATED:	2012/01/18
	FILENAME:	Work.h
	AUTHOR:		by Mr.Chen
	
	PURPOSE:	Work类，维护一个具体状态子类的实例，这个实例定义Work的当前状态
*********************************************************************/
#ifndef _WORK_H
#define _WORK_H

namespace state_pattern
{

class State;

class Work
{
public:
	Work();

public:
	void setHour(double hour) { _hour = hour; }
	double getHour() { return _hour; }

	void setFinished(bool finish) { _finish = finish; }
	bool isFinished() { return _finish; }

	void changeState(State *pState);
	State* getState() { return _currState;}

public:
	void writeProgram();

private:
	// 维系一个具体状态子类的实例指针
	State *_currState;
	double _hour;
	bool _finish;
};

void test();

} //namespace state_pattern


#endif // _WORK_H
