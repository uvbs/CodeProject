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
	char	_char_name[32];	//玩家名称
	uint16	_level;				//当前等级
	uint	_money;				//金钱
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

	const char* getName() const { return _human_db.getName(); } //玩家名
	const char* getUserId() const { return _human_db.getUserId(); } //账号

	short	getLevel() const { return _human_db.getLevel(); } //玩家等级
	void	setLevel(short level) { _human_db.setLevel(level); }
	
	uint	getMoney() const { return _human_db.getMoney(); }  //获取金钱
	bool	consumeMoney(int money);	//消耗金钱，不足返回false
	bool	addMoney(int money);	//增加金钱
	bool	_setMoney(uint money);	//设置金钱

	//HumanItemContainer*		GetBagContainer() { return &m_BagContainer; }				//获得背包栏
	//HumanItemContainer*		GetEquipContainer() {return &m_EquipContainer; }			//获得装备栏
	bool	executeMail(Mail* pMail);	//执行邮件
	void	onDayChanged() {}
	void	onHourChanged() {}
	void	sendFreshAttrs(); //刷新属性到客户端

public:
	HumanDB*	getHumanDB() {return &_human_db;}
	void	setSMU(HumanSMU* pHumanSMU){ _human_db.setSMU(pHumanSMU);}
	HumanSMU*	getSMU() const {return _human_db.getSMU();}
	
private:
	//HumanItemContainer		m_BagContainer;				//背包栏
	//HumanItemContainer		m_EquipContainer;			//装备栏
	HumanDB	_human_db; //db存储数据
	HumanAttrBackup	_attr_backup; //属性刷新备份
	Timer _dbsave_timer; //存盘计时器
	uint	_last_fresh_day; //天
	uint	_last_fresh_time; //时
};

#endif	//_OBJECT_HUMAN_H
