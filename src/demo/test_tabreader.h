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

#pragma pack(push) //�������״̬
#pragma pack(8) //����8�ֽڶ���

//�����SceneDefine.tab�нṹ
struct tDBC_FILE_SCENE_DEFINE
{
	int	 _nSceneId;			//����ID
	char* _pSceneName;		//��������
	int	 _bActive;				//�Ƿ񼤻�
	int	 _nServerId;			//���д˳�����LOGIC����SERVERID
	int	 _nSceneType; 		//��������(0��ʾ��ͨ������1��ʾ����)
	int _nSceneLevel;		//������ͼ�������
	int	 _bCanRide;			//�Ƿ�����
	int	 _bHasShadow;		//�Ƿ�����Ӱ
	int	 _nPlayer;				//���
	int	 _nMonster;			//��
	int	 _nDropbox;			//�����
	int _nBus;					//��ͨ����
	int	 _nArea;					//�¼�����
	int	 _nGrowPoint;			//������	
	int _nTimer;				//��ʱ��
	int	 _nEffect;				//��Ч
	int	 _nSpellEffectObj;	//������Ч����
	int _nXSize;				//����X���ֵ
	int	 _nZSize;				//����Z���ֵ
	char* _pSceneFileName;	//�����ļ�����
	char* _pMapName;			//������ͼ��
	int	 _nBrightParam;		//����Actor���Ȳ���
	int	 _nCursorSelBrightParam;	//���ѡ�����Ȳ���
	char* _pBackSoundName;			//�������������ļ���
	char* _pBack2DSoundName;		//����������Ч�ļ���
	int	 _nScriptId;				//�ű�id
	int	 _nFYDist;					//�����������
	char* _pSceneTransImg;		//����Load����ͼ		
	int	 _nPKModeParent;		//���������ǿ���л�Ϊ��Ӧ��PKģʽ���ࣺ����ģʽ����ƽģʽ
	char* _nPKModeChild;			//���������ǿ���л�Ϊ��Ӧ��PKģʽ���� eg�����ģʽ�����ģʽ��
	int	 _nScenePKAttr;			//����Ĭ��pk����
	char* _nSceneEnterImg;		//���볡��ʱ��ʾ��ͼƬ
};

#pragma pack(pop)  //�ָ�����״̬

void test_tabreader();
