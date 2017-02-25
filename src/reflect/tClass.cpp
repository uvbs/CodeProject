#include "tClass.h"
#include "tNode.h"

namespace reflect {

	tNode*	tClass::createObject(const char* szName)	//!< ��������һ��ʵ���ĺ���
	{
		if(pfnCreateObject == NULL) return NULL;
		tNode *pObject = (*pfnCreateObject)();
		pObject->_name = szName;
		return pObject;
	}

} // namespace reflect
