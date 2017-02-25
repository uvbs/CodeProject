////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_tabreader.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 0:27
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_tabreader.h"
#include <iostream>
using std::cout;
using std::endl;

void test_tabreader()
{
	enum
	{
		DBC_FILE_SCENE_DEFINE = 0,
	};

	//资源配置数组
	_DATABASE_DEFINE  dbFileList[] = 
	{
		{DBC_FILE_SCENE_DEFINE,	"./data/SceneDefine.tab"},
	};

	DBCSystem::getSinglePtr()->openDatabases(dbFileList,sizeof(dbFileList)/sizeof(dbFileList[0]));
	const DBC* pSceneDefineDataFile = DBCSystem::getSinglePtr()->getDatabase(DBC_FILE_SCENE_DEFINE);
	if (pSceneDefineDataFile)
	{
		int nRecordNum = pSceneDefineDataFile->getRecordsNum();
		std::cout<< "RecordNum=" << nRecordNum << std::endl;
/*
		#define off(TYPE,MEMBER) (int)&(((TYPE*)0)->MEMBER)
		int off1 = off(tDBC_FILE_SCENE_DEFINE,m_nSceneID);
		int off2 = off(tDBC_FILE_SCENE_DEFINE,m_szSceneName);
		int off3 = off(tDBC_FILE_SCENE_DEFINE,m_bIsActive);
		int off4 = off(tDBC_FILE_SCENE_DEFINE,m_nServerID);
		int off5 = off(tDBC_FILE_SCENE_DEFINE,m_nSceneType);
		int off6 = off(tDBC_FILE_SCENE_DEFINE,m_nSceneLevel);
		int off7 = off(tDBC_FILE_SCENE_DEFINE,m_bIsCanRide);
		int off8 = off(tDBC_FILE_SCENE_DEFINE,m_bIsHaveShadow);
		int size = sizeof(tDBC_FILE_SCENE_DEFINE);
*/
		const FIELD*  pdbc = pSceneDefineDataFile->searchPosition(0,24);
		if (pdbc)
		{
			std::cout<< "SoundName=" << pdbc->pString << std::endl;
		}
		const FIELD* pdbcfile = pSceneDefineDataFile->searchLineNumEQ(0);
		//const tDBC_FILE_SCENE_DEFINE* pSceneDefine = (const tDBC_FILE_SCENE_DEFINE*)pdbcfile;
		//Segmentation fault on linux 64
		const tDBC_FILE_SCENE_DEFINE* pSceneDefine = reinterpret_cast<const tDBC_FILE_SCENE_DEFINE*>(pdbcfile);
		if (pSceneDefine)
		{
			//std::cout<< "SoundName=" << pSceneDefine->_pBack2DSoundName << std::endl;
		}
	}

}
