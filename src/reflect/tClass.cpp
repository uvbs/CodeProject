#include "tClass.h"
#include "tNode.h"

namespace reflect {

	tNode*	tClass::createObject(const char* szName)	//!< 用于生成一个实例的函数
	{
		if(pfnCreateObject == NULL) return NULL;
		tNode *pObject = (*pfnCreateObject)();
		pObject->_name = szName;
		return pObject;
	}

} // namespace reflect
