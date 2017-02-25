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

//�����SceneDefine.tab�нṹ
struct TableRowScene
{
	int			m_Sceneid;								//����ID
	char*	m_Name;									//��������
	int			m_Isactive;								//�Ƿ񼤻�(1��ʾ���0��ʾ������)
	int			m_Serverid;								//���ڷ�������
	int			m_Scenetype;							//��������(0��ʾ��ͨ������1��ʾ��
	int			m_LevelMin;								//��͵ȼ�����
	int			m_Canridehorse;						//�Ƿ������
	int			m_SceneFiledHasshadow;			//�Ƿ�����Ӱ
	int			m_Player;									//���
	int			m_Monster;								//����
	int			m_Dropbox;								//�����
	int			m_Moto;									//��ͨ����
	int			m_Area;									//�¼�����
	int			m_Growpoint;							//������
	int			m_Timer;									//��ʱ��
	int			m_Effect;									//��Ч
	int			m_SpellEffect;							//������Ч����
	int			m_Xszie;									//������СX
	int			m_Zsize;									//������СY
	char*	m_Filename;								//�����ļ���
	char*	m_MapFilename;						//��ͼ��
	int			m_BrightenParam;						//�������ò���(1--99)
	int			m_CursorSelectBrightenParam;	//���ѡ�����Ȳ���
	char*	m_BacksoundFilename;				//����������
	char*	m_Back2dFilename;					//������Ч
	int			m_SceneFiledScriptId;				//�ű�ID
	int			m_AntiStealth;							//�����������
	char*	m_ImageOnSceneTrans;			//Load����ͼ
	int			m_PkModeParent;                     //���������ǿ���л�Ϊ��Ӧ��PKģʽ���ࣺ����ģʽ����ƽģʽ
	IntArray m_PKModeChildArray;              //���������ǿ���л�Ϊ��Ӧ��PKģʽ���� eg�����ģʽ�����ģʽ��
	int			m_scenePKAttribute;				    //����Ĭ��pk����
	char*	m_ImageOnEnterScene;			//���볡��ʱ��ʾ��ͼƬ

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
	Table<TableRowScene> m_TableRowSceneObj;	//�������� SceneDefine.tab
	Table<TableRowAudit> m_TableRowAudit1Obj;       //ͳ�Ʊ�
	Table<TableRowAudit> m_TableRowAudit2Obj;       //ͳ�Ʊ�
};

#define GetTab(ROW) m_##ROW##Obj
#define  SetTab(ROW,Tab) Table<ROW> m_##ROW##Obj; m_##ROW##Obj.load(Tab)

void test_tabreaderV2();
