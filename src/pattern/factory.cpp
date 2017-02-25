#include "factory.h"

namespace factory_pattern
{
	void test()
	{
		// ¼Ó·¨ÔËËã
		factory_pattern::Operation *pOperAdd = OperationFactory::createOperate('+');
		double result = 0.0;
		if (NULL != pOperAdd)
		{
			pOperAdd->setA(1.0);
			pOperAdd->setB(2.0);
			result = pOperAdd->getResult();
		}
	}

}
