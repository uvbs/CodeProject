/**********************************
** @file: object_human.h
** @brief:  
** @autor: by Mr.Chen
** @date: 2016/07/23 16:19
**********************************/
#ifndef _OBJECT_HUMAN_H
#define _OBJECT_HUMAN_H

#include "object.h"
#include "data_struct.h"
#include "humanDB.h"

struct HumanAttrBackup
{
	char	_char_name[32];	//�������
	uint16	_level;				//��ǰ�ȼ�
	uint	_money;				//��Ǯ
	HumanAttrBackup()
	{
		cleanUp();
	}
	void cleanUp()
	{
		memset(_char_name,0,sizeof(_char_name));
		_level = 0;
		_money = 0;
	}
};

class ObjectHuman : public Object
{
public:
	ObjectHuman() {}
	virtual ~ObjectHuman() {}

	bool	init(const FullUserData* pUserDB);
	void	initAttrBackUp();
	void	cleanUp();
	bool	heartBeat(uint uTime);
	EM_OBJ_TYPE	getObjType() { return EM_OBJ_TYPE_HUMAN; }

	Guid	getGuid() { return _human_db.getGuid();} //GUID
	void	setGuid(Guid guid) {_human_db.setGuid(guid); }

	const char* getName() const { return _human_db.getName(); } //�����
	const char* getUserId() const { return _human_db.getUserId(); } //�˺�

	short	getLevel() const { return _human_db.getLevel(); } //��ҵȼ�
	void	setLevel(short level) { _human_db.setLevel(level); }
	
	uint	getMoney() const { return _human_db.getMoney(); }  //��ȡ��Ǯ
	bool	consumeMoney(int money);	//���Ľ�Ǯ�����㷵��false
	bool	addMoney(int money);	//���ӽ�Ǯ
	bool	_setMoney(uint money);	//���ý�Ǯ

	//HumanItemContainer*		GetBagContainer() { return &m_BagContainer; }				//��ñ�����
	//HumanItemContainer*		GetEquipContainer() {return &m_EquipContainer; }			//���װ����
	bool	executeMail(Mail* pMail);	//ִ���ʼ�
	void	onDayChanged() {}
	void	onHourChanged() {}
	void	sendFreshAttrs(); //ˢ�����Ե��ͻ���

public:
	HumanDB*	getHumanDB() {return &_human_db;}
	void	setSMU(HumanSMU* pHumanSMU){ _human_db.setSMU(pHumanSMU);}
	HumanSMU*	getSMU() const {return _human_db.getSMU();}
	
private:
	//HumanItemContainer		m_BagContainer;				//������
	//HumanItemContainer		m_EquipContainer;			//װ����
	HumanDB	_human_db; //db�洢����
	HumanAttrBackup	_attr_backup; //����ˢ�±���
	Timer _dbsave_timer; //���̼�ʱ��
	uint	_last_fresh_day; //��
	uint	_last_fresh_time; //ʱ
};

#endif	//_OBJECT_HUMAN_H
