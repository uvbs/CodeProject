#include "state.h"
#include "work.h"
#include <iostream>
using namespace std;

namespace state_pattern
{

/**
	上午工作状态
*/
void ForenoonSate::writeProgram(Work *pWk)
{
	if (pWk)
	{
		if (pWk->getHour() < 12)
		{
			cout << "当前时间：" << pWk->getHour() << "点，上午工作，精神百倍!" << endl;
		}
		else
		{
			// 切换到下个状态
			pWk->changeState(new NoonState());
			pWk->writeProgram();
		}
	}
}


/**
	中午工作状态
*/
void NoonState::writeProgram(Work *pWk)
{
	if (pWk)
	{
		if (pWk->getHour() < 13)
		{
			cout << "当前时间：" << pWk->getHour() << "点，饿了，午饭，犯困，午休!" << endl;
		}
		else
		{
			// 切换到下个状态
			pWk->changeState(new AfternoonState());
			pWk->writeProgram();
		}
	}
}


/**
	下午工作状态
*/
void AfternoonState::writeProgram(Work *pWk)
{
	if (pWk)
	{
		if (pWk->getHour() < 17)
		{
			cout << "当前时间：" << pWk->getHour() << "点，下午状态还不错，继续努力!" << endl;
		}
		else
		{
			// 切换到下个状态
			pWk->changeState(new EveningState());
			pWk->writeProgram();
		}
	}
}


/**
	晚间工作状态
*/
void EveningState::writeProgram(Work *pWk)
{
	if (pWk)
	{
		if (pWk->isFinished())
		{
			// 切换到下个状态
			pWk->changeState(new RestState());
			pWk->writeProgram();
		}
		else
		{
			if (pWk->getHour() < 21)
			{
				cout << "当前时间：" << pWk->getHour() << "点，加班哦，疲累至极!" << endl;
			}
			else
			{
				// 切换到下个状态
				pWk->changeState(new SleepingState());
				pWk->writeProgram();
			}
		}
	}
}


/**
	睡眠状态
*/
void SleepingState::writeProgram(Work *pWk)
{
	if (pWk)
	{
		cout << "当前时间："<< pWk->getHour() <<"点，不行啦，睡着啦!" << endl;
	}
}


/**
	下班休息状态
*/
void RestState::writeProgram(Work *pWk)
{
	if (pWk)
	{
		cout << "当前时间："<< pWk->getHour() <<"点，下班回家啦!" << endl;
	}
}


} // namespace state_pattern
