////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_tabreaderV2.h
// @author: by Mr.Chen
// @date: 2015/4/22/ 0:32
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "type.h"
#include "serializer.h"
#include "serializer_clear.h"
#include "table.h"
#include "singleton.h"

//定义表SceneDefine.tab行结构
struct TableRowScene
{
	int			m_Sceneid;								//场景ID
	char*	m_Name;									//场景名字
	int			m_Isactive;								//是否激活(1表示激活，0表示不激活)
	int			m_Serverid;								//所在服务器号
	int			m_Scenetype;							//场景类型(0表示普通场景，1表示副
	int			m_LevelMin;								//最低等级限制
	int			m_Canridehorse;						//是否可骑马
	int			m_SceneFiledHasshadow;			//是否有阴影
	int			m_Player;									//玩家
	int			m_Monster;								//怪物
	int			m_Dropbox;								//掉落包
	int			m_Moto;									//交通工具
	int			m_Area;									//事件区域
	int			m_Growpoint;							//生长点
	int			m_Timer;									//定时器
	int			m_Effect;									//特效
	int			m_SpellEffect;							//技能特效对象
	int			m_Xszie;									//场景大小X
	int			m_Zsize;									//场景大小Y
	char*	m_Filename;								//场景文件名
	char*	m_MapFilename;						//地图名
	int			m_BrightenParam;						//加亮配置参数(1--99)
	int			m_CursorSelectBrightenParam;	//光标选择亮度参数
	char*	m_BacksoundFilename;				//背景音乐名
	char*	m_Back2dFilename;					//背景音效
	int			m_SceneFiledScriptId;				//脚本ID
	int			m_AntiStealth;							//反隐距离参数
	char*	m_ImageOnSceneTrans;			//Load背景图
	int			m_PkModeParent;                     //进入此区域将强制切换为对应的PK模式大类：自由模式，和平模式
	IntArray m_PKModeChildArray;              //进入此区域将强制切换为对应的PK模式子类 eg：组队模式，帮会模式等
	int			m_scenePKAttribute;				    //场景默认pk属性
	char*	m_ImageOnEnterScene;			//进入场景时显示的图片

	void mapData(ISerializer& s)
	{
		s << m_Sceneid;
		s << m_Name;
		s << m_Isactive;
		s << m_Serverid;
		s << m_Scenetype;
		s << m_LevelMin;
		s << m_Canridehorse;
		s << m_SceneFiledHasshadow;
		s << m_Player;
		s << m_Monster;
		s << m_Dropbox;
		s << m_Moto;
		s << m_Area;
		s << m_Growpoint;
		s << m_Timer;
		s << m_Effect;
		s << m_SpellEffect;
		s << m_Xszie;
		s << m_Zsize;
		s << m_Filename;
		s << m_MapFilename;
		s << m_BrightenParam;
		s << m_CursorSelectBrightenParam;
		s << m_BacksoundFilename;
		s << m_Back2dFilename;
		s << m_SceneFiledScriptId;
		s << m_AntiStealth;
		s << m_ImageOnSceneTrans; 
		s << m_PkModeParent; 
		s << m_PKModeChildArray;
		s << m_scenePKAttribute;
		s.skipField();
	}
	TableRowScene() { ClearSerializer cs; mapData(cs); }
	bool operator==(const int id) const { return m_Sceneid == id; }
	bool operator<=(const TableRowScene& other) const { return m_Sceneid <= other.m_Sceneid; }
	bool operator<(const int id) const { return m_Sceneid < id; }
};

//audit
struct TableRowAudit
{
	int			m_Id;								//Id
	char*	m_query;							//query

	void mapData(ISerializer& s)
	{
		s << m_Id;
		s << m_query;
	}
	TableRowAudit() { ClearSerializer cs; mapData(cs); }
	bool operator==(const int id) const { return m_Id == id; }
	bool operator<=(const TableRowAudit& other) const { return m_Id <= other.m_Id; }
	bool operator<(const int id) const { return m_Id < id; }
};

//
class TableDBSystem : public Singleton<TableDBSystem>
{
public:
	void load()
	{
		TableSerializer::reload();
		m_TableRowSceneObj.load("./data/SceneDefine.tab");
		m_TableRowAudit1Obj.load("./data/audit1.txt");
		m_TableRowAudit2Obj.load("./data/audit2.txt");
	}
public:
	Table<TableRowScene> m_TableRowSceneObj;	//场景配置 SceneDefine.tab
	Table<TableRowAudit> m_TableRowAudit1Obj;       //统计表
	Table<TableRowAudit> m_TableRowAudit2Obj;       //统计表
};

#define GetTab(ROW) m_##ROW##Obj
#define  SetTab(ROW,Tab) Table<ROW> m_##ROW##Obj; m_##ROW##Obj.load(Tab)

void test_tabreaderV2();
