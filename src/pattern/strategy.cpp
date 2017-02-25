
#include "strategy.h"

namespace strategy_pattern
{
    void test()
    {
		Duck* pMallard = new MallardDuck();
		if (pMallard)
		{
			pMallard->performFly();
			pMallard->performQuack();
			pMallard->display();
		}

		Duck* pModel = new ModelDuck();
		if (pModel)
		{
			pModel->performFly();
			pModel->setFlyBehavior(new FlyRocketPowered());
			pModel->performFly();
		}
    }

} //namespace strategy_pattern

