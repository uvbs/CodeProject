////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_tabreader.h
// @author: by Mr.Chen
// @date: 2015/4/22/ 0:24
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "type.h"
#include "dbc_system.h"

#pragma pack(push) //保存对齐状态
#pragma pack(8) //设置8字节对齐

//定义表SceneDefine.tab行结构
struct tDBC_FILE_SCENE_DEFINE
{
	int	 _nSceneId;			//场景ID
	char* _pSceneName;		//场景名字
	int	 _bActive;				//是否激活
	int	 _nServerId;			//运行此场景的LOGIC程序SERVERID
	int	 _nSceneType; 		//场景类型(0表示普通场景，1表示副本)
	int _nSceneLevel;		//场景最低级别限制
	int	 _bCanRide;			//是否可骑乘
	int	 _bHasShadow;		//是否有阴影
	int	 _nPlayer;				//玩家
	int	 _nMonster;			//怪
	int	 _nDropbox;			//掉落包
	int _nBus;					//交通工具
	int	 _nArea;					//事件区域
	int	 _nGrowPoint;			//生长点	
	int _nTimer;				//定时器
	int	 _nEffect;				//特效
	int	 _nSpellEffectObj;	//技能特效对象
	int _nXSize;				//场景X最大值
	int	 _nZSize;				//场景Z最大值
	char* _pSceneFileName;	//场景文件名字
	char* _pMapName;			//场景地图名
	int	 _nBrightParam;		//场景Actor亮度参数
	int	 _nCursorSelBrightParam;	//光标选择亮度参数
	char* _pBackSoundName;			//场景背景音乐文件名
	char* _pBack2DSoundName;		//场景背景音效文件名
	int	 _nScriptId;				//脚本id
	int	 _nFYDist;					//反隐距离参数
	char* _pSceneTransImg;		//场景Load背景图		
	int	 _nPKModeParent;		//进入此区域将强制切换为对应的PK模式大类：自由模式，和平模式
	char* _nPKModeChild;			//进入此区域将强制切换为对应的PK模式子类 eg：组队模式，帮会模式等
	int	 _nScenePKAttr;			//场景默认pk属性
	char* _nSceneEnterImg;		//进入场景时显示的图片
};

#pragma pack(pop)  //恢复对齐状态

void test_tabreader();
