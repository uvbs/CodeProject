#include "state.h"
#include "work.h"
#include <iostream>
using namespace std;

namespace state_pattern
{

/**
	���繤��״̬
*/
void ForenoonSate::writeProgram(Work *pWk)
{
	if (pWk)
	{
		if (pWk->getHour() < 12)
		{
			cout << "��ǰʱ�䣺" << pWk->getHour() << "�㣬���繤��������ٱ�!" << endl;
		}
		else
		{
			// �л����¸�״̬
			pWk->changeState(new NoonState());
			pWk->writeProgram();
		}
	}
}


/**
	���繤��״̬
*/
void NoonState::writeProgram(Work *pWk)
{
	if (pWk)
	{
		if (pWk->getHour() < 13)
		{
			cout << "��ǰʱ�䣺" << pWk->getHour() << "�㣬���ˣ��緹������������!" << endl;
		}
		else
		{
			// �л����¸�״̬
			pWk->changeState(new AfternoonState());
			pWk->writeProgram();
		}
	}
}


/**
	���繤��״̬
*/
void AfternoonState::writeProgram(Work *pWk)
{
	if (pWk)
	{
		if (pWk->getHour() < 17)
		{
			cout << "��ǰʱ�䣺" << pWk->getHour() << "�㣬����״̬����������Ŭ��!" << endl;
		}
		else
		{
			// �л����¸�״̬
			pWk->changeState(new EveningState());
			pWk->writeProgram();
		}
	}
}


/**
	��乤��״̬
*/
void EveningState::writeProgram(Work *pWk)
{
	if (pWk)
	{
		if (pWk->isFinished())
		{
			// �л����¸�״̬
			pWk->changeState(new RestState());
			pWk->writeProgram();
		}
		else
		{
			if (pWk->getHour() < 21)
			{
				cout << "��ǰʱ�䣺" << pWk->getHour() << "�㣬�Ӱ�Ŷ��ƣ������!" << endl;
			}
			else
			{
				// �л����¸�״̬
				pWk->changeState(new SleepingState());
				pWk->writeProgram();
			}
		}
	}
}


/**
	˯��״̬
*/
void SleepingState::writeProgram(Work *pWk)
{
	if (pWk)
	{
		cout << "��ǰʱ�䣺"<< pWk->getHour() <<"�㣬��������˯����!" << endl;
	}
}


/**
	�°���Ϣ״̬
*/
void RestState::writeProgram(Work *pWk)
{
	if (pWk)
	{
		cout << "��ǰʱ�䣺"<< pWk->getHour() <<"�㣬�°�ؼ���!" << endl;
	}
}


} // namespace state_pattern
