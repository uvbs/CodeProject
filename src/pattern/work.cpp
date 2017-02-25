#include "work.h"
#include "state.h"
#include "macrodef.h"

namespace state_pattern
{

Work::Work() 
{
	_currState = new ForenoonSate();
}

void Work::changeState(State *pState)
{
	SAFE_DELETE(_currState);
	_currState = pState;
}

void Work::writeProgram()
{
	if (_currState)
	{
		_currState->writeProgram(this);
	}
}

void test()
{
	Work *pWk = new Work();
	pWk->setHour(9);
	pWk->writeProgram();

	pWk->setHour(10);
	pWk->writeProgram();

	pWk->setHour(12);
	pWk->writeProgram();

	pWk->setHour(13);
	pWk->writeProgram();

	pWk->setHour(14);
	pWk->writeProgram();

	pWk->setHour(17);
	pWk->setFinished(false);
	pWk->writeProgram();

	pWk->setHour(19);
	pWk->writeProgram();

	pWk->setHour(22);
	pWk->writeProgram();

}

} // namespace state_pattern
