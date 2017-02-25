#include "tpl_method.h"

namespace tpl_method_pattern
{

void test()
{
	AbstractClass *p = new ConcreteClass1();
	p->templateMethod();
	delete p;
	p = new ConcreteClass2();
	p->templateMethod();
}

}
