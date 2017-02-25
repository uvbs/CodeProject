/**********************************
** @file: humanDB.h
** @brief:  
** @autor: by Mr.Chen
** @date: 2016/07/23 16:45
**********************************/
#ifndef _HUMAN_DB_H
#define _HUMAN_DB_H

#include "dbstruct.h"
#include "shm_def.h"
#include "shm_struct.h"
#include "bitset.h"

class HumanDB
{
public:
	HumanDB();
	~HumanDB();
	void	cleanUp();

	Guid getGuid() const { return _user_data._human_data._guid; }
	void setGuid(Guid guid);

	const char* getName() const { return _user_data._human_data._char_name; }
	const char* getUserId() const { return _user_data._human_data._user; }

	short	getLevel() const { return _user_data._human_data._level; }
	void	setLevel(short level);

	uint	getMoney() const { return _user_data._human_data._money; }
	void	setMoney(uint money);

	int		getBagSize() const { return _user_data._human_data._bag_size; }
	void	setBagSize(int size);
	const ItemData* getBagItem(int i) const;

	int 	getEquipContainerSize() const {return _user_data._human_data._equip_size; }
	void	setEquipContainerSize(int size);
	const ItemData* getEquipItem(int i) const;

public:
	void	fillFullUserData(const FullUserData* userdata);
	//��������
	void	setDBAttrDirty(EM_ATTR_DB offset, bool flag = true);
	bool	getDBAttrDirty(EM_ATTR_DB offset);
	//SMU���
	void	lock();
	void	unLock();
	void	setFreeOwnFlag(int Flag);
	int		getFreeOwnFlag() const;
	//����
	void	validateSMU(bool bForceAll = false);
	void	setSMU(HumanSMU* pHumanSMU){ Assert(pHumanSMU); _human_SMU = pHumanSMU; regSMAttrs();}
	HumanSMU* getSMU() const {return _human_SMU;}
	
protected:
	void	regAttrs();  //ע������DB����
	void	regSMAttrs();  //ע������ShareMem����
	void	_regAttr(EM_ATTR_DB offset, void* arg, int size, bool smu = false);//ע�ᵥ������
	void*	_getAttr(EM_ATTR_DB offset, bool smu = false) const;
	int		_getAttrSize(EM_ATTR_DB offset) const;
	
private:
	BitSet<EM_ATTR_DB_NUM> _dbAttr_dirty_flag;	//DB���Դ��̱��
	FullUserData _user_data;
	HumanSMU* _human_SMU; //SMUָ��	
	HumanDB_ATTR_REG	 _reg_attr;	 //��ɫ����ע������
	HumanDB_ATTR_REG	 _reg_sm_attr;	//��ɫ������ShareMem��ע������
};


#endif	//_HUMAN_DB_H
