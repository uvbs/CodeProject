////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_tabreaderV2.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 0:34
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_tabreaderV2.h"
#include <iostream>
using std::cout;
using std::endl;

WX_IMPLEMENT_SINGLEON(TableDBSystem);

void test_tabreaderV2()
{
	TableDBSystem::getSinglePtr()->load();
	const TableRowScene* pRow = &TableDBSystem::getSinglePtr()->m_TableRowSceneObj.getRowByIndex(0);
	if (pRow)
	{
		std::cout<< pRow->m_Back2dFilename << std::endl;
	}

	SetTab(TableRowScene,"./data/SceneDefine.tab"); //m_TableRowSceneObj;
	pRow = &(GetTab(TableRowScene).getRowByIndex(0));
	if (pRow)
	{
		std::cout<< pRow->m_Back2dFilename << std::endl;
	}

}
