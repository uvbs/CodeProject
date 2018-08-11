#include "humanDB.h"

HumanDB::HumanDB()
{
	cleanUp();
	regAttrs();
}

HumanDB::~HumanDB()
{
	cleanUp();
}

void	HumanDB::cleanUp()
{
	_user_data.cleanUp();
}

void	HumanDB::setGuid(Guid guid)
{
	if (getGuid() != guid)
	{
		_user_data._human_data._guid = guid;
		setDBAttrDirty(EM_ATTR_DB_GUID);
	}
}

void	HumanDB::setLevel(short level)
{
	if (getLevel() != level)
	{
		_user_data._human_data._level = level;
		setDBAttrDirty(EM_ATTR_DB_LEVEL);
	}
}

void	HumanDB::setMoney(uint money)
{
	if (getMoney() != money)
	{
		_user_data._human_data._money = money;
		setDBAttrDirty(EM_ATTR_DB_MONEY);
	}
}

void	HumanDB::setBagSize(int nSize)
{
	if (getBagSize() != nSize)
	{
		_user_data._human_data._bag_size = nSize;
		setDBAttrDirty(EM_ATTR_DB_BAGSIZE);
	}
}

const ItemData* HumanDB::getBagItem(int i) const
{
	if (i >= 0 && i < getBagSize())
	{
		return &_user_data._bag_data._item[i];
	}
	return NULL;
}

void	HumanDB::setEquipContainerSize(int nSize)
{
	if (getEquipContainerSize() != nSize)
	{
		_user_data._human_data._equip_size = nSize;
		setDBAttrDirty(EM_ATTR_DB_EQUIPSIZE);
	}
}

const ItemData* HumanDB::getEquipItem(int i) const
{
	if (i >= 0 && i < getEquipContainerSize())
	{
		return &_user_data._equip_data._item[i];
	}
	return NULL;
}

void	HumanDB::fillFullUserData(const FullUserData* userdata)
{
	MyAssert(userdata);
	memcpy(&_user_data._human_data, &userdata->_human_data, sizeof(_user_data._human_data));
	memcpy(&_user_data._bag_data, &userdata->_bag_data, sizeof(_user_data._bag_data));
	memcpy(&_user_data._equip_data, &userdata->_equip_data, sizeof(_user_data._equip_data));
}

//X_VOID	HumanDB::WriteHumanDB(const	_HUMAN_DB_LOAD* pHumanDB)
//{
//	__ENTER_FUNCTION
//	memcpy(&m_HumanDBLoad, pHumanDB, sizeof(m_HumanDBLoad));
//	__LEAVE_FUNCTION
//}
//
//X_VOID	HumanDB::WriteBagDB(const _BAG_DB_LOAD* pBagDB)
//{
//	__ENTER_FUNCTION
//	memcpy(&m_BagDBLoad, pBagDB, sizeof(m_BagDBLoad));
//	__LEAVE_FUNCTION
//}
//
//X_VOID	HumanDB::WriteEquipDB(const _EQUIP_DB_LOAD* pEquipDB)
//{
//	__ENTER_FUNCTION
//	memcpy(&m_EquipDBLoad, pEquipDB, sizeof(m_EquipDBLoad));
//	__LEAVE_FUNCTION
//}

void	HumanDB::regAttrs()
{
	#define REG_ATTR(type,var)		_regAttr(type, &(var), sizeof(var));
	//////////////////////////////////////////////////////////////////////////
	//基本属性
	//////////////////////////////////////////////////////////////////////////
	REG_ATTR(EM_ATTR_DB_GUID,	_user_data._human_data._guid);
	REG_ATTR(EM_ATTR_DB_USER,	_user_data._human_data._user);
	REG_ATTR(EM_ATTR_DB_NAME,	_user_data._human_data._char_name);
	REG_ATTR(EM_ATTR_DB_LEVEL, _user_data._human_data._level);
	REG_ATTR(EM_ATTR_DB_MONEY,	 _user_data._human_data._money);
	REG_ATTR(EM_ATTR_DB_BAGSIZE,	_user_data._human_data._bag_size);
	REG_ATTR(EM_ATTR_DB_EQUIPSIZE,	_user_data._human_data._equip_size);
	//////////////////////////////////////////////////////////////////////////
	//背包栏
	//////////////////////////////////////////////////////////////////////////
	int iBagIndex = 0;
	for (int i = EM_ATTR_DB_BAG_BEGIN; i < EM_ATTR_DB_BAG_END; i++)
	{
		REG_ATTR((EM_ATTR_DB)i, _user_data._bag_data._item[iBagIndex]);
		iBagIndex++;
	}
	//////////////////////////////////////////////////////////////////////////
	//装备栏
	//////////////////////////////////////////////////////////////////////////
	int iEquipIndex = 0;
	for (int i = EM_ATTR_DB_EQUIP_BEGIN; i < EM_ATTR_DB_EQUIP_END; i++)
	{
		REG_ATTR((EM_ATTR_DB)i, _user_data._equip_data._item[iEquipIndex]);
		iEquipIndex++;
	}
}

void	HumanDB::regSMAttrs()
{
	#define REG_SM_ATTR(type,var)		_regAttr(type, &(var), sizeof(var), true);
	MyAssert(_human_SMU);
	//////////////////////////////////////////////////////////////////////////
	//基本属性
	//////////////////////////////////////////////////////////////////////////
	REG_SM_ATTR(EM_ATTR_DB_GUID, _human_SMU->_SMU_data._human_data._guid);
	REG_SM_ATTR(EM_ATTR_DB_USER, _human_SMU->_SMU_data._human_data._user);
	REG_SM_ATTR(EM_ATTR_DB_NAME, _human_SMU->_SMU_data._human_data._char_name);
	REG_SM_ATTR(EM_ATTR_DB_LEVEL, _human_SMU->_SMU_data._human_data._level);
	REG_SM_ATTR(EM_ATTR_DB_MONEY, _human_SMU->_SMU_data._human_data._money);
	REG_SM_ATTR(EM_ATTR_DB_BAGSIZE, _human_SMU->_SMU_data._human_data._bag_size);
	REG_SM_ATTR(EM_ATTR_DB_EQUIPSIZE, _human_SMU->_SMU_data._human_data._equip_size);
	//////////////////////////////////////////////////////////////////////////
	//背包栏
	//////////////////////////////////////////////////////////////////////////
	int iBagIndex = 0;
	for (int i = EM_ATTR_DB_BAG_BEGIN; i < EM_ATTR_DB_BAG_END; i++)
	{
		REG_SM_ATTR((EM_ATTR_DB)i, _human_SMU->_SMU_data._bag_data._item[iBagIndex]);
		iBagIndex++;
	}
	//////////////////////////////////////////////////////////////////////////
	//装备栏
	//////////////////////////////////////////////////////////////////////////
	int iEquipIndex = 0;
	for (int i = EM_ATTR_DB_EQUIP_BEGIN; i < EM_ATTR_DB_EQUIP_END; i++)
	{
		REG_SM_ATTR((EM_ATTR_DB)i, _human_SMU->_SMU_data._equip_data._item[iEquipIndex]);
		iEquipIndex++;
	}
}

void	HumanDB::_regAttr(EM_ATTR_DB offset, void* arg, int size, bool smu /*= false*/)
{
	MyAssert(offset >= 0 && offset < EM_ATTR_DB_NUM);
	MyAssert(arg);
	MyAssert(size > 0);
	if (!smu)
	{
		_reg_attr.m_AttrPtr[offset] = arg;
		_reg_attr.m_AttrSize[offset] = size;
	}
	else
	{
		_reg_sm_attr.m_AttrPtr[offset] = arg;
		_reg_sm_attr.m_AttrSize[offset] = size;
	}
}

void* 	HumanDB::_getAttr(EM_ATTR_DB offset, bool smu/* = false*/) const
{
	MyAssert(offset >= 0 && offset < EM_ATTR_DB_NUM);
	if (!smu) { return _reg_attr.m_AttrPtr[offset]; }
	else { return _reg_sm_attr.m_AttrPtr[offset]; }
}

int		HumanDB::_getAttrSize(EM_ATTR_DB offset) const
{
	MyAssert(offset >= 0 && offset < EM_ATTR_DB_NUM);
	return _reg_attr.m_AttrSize[offset];
}

void	HumanDB::setDBAttrDirty(EM_ATTR_DB offset, bool flag)
{
	MyAssert(offset >= 0 && offset < EM_ATTR_DB_NUM);
	if (flag) { _dbAttr_dirty_flag.setBit(offset); }
	else { _dbAttr_dirty_flag.clearBit(offset); }
}

bool	HumanDB::getDBAttrDirty(EM_ATTR_DB offset)
{
	MyAssert(offset >= 0 && offset < EM_ATTR_DB_NUM);
	return _dbAttr_dirty_flag.isSetBit(offset);
}

void	HumanDB::setFreeOwnFlag(int Flag)
{
	if (_human_SMU) {
		_human_SMU->setUseStatus(Flag, SM_S_WRITE); 
	}
}

int		HumanDB::getFreeOwnFlag() const
{
	if (_human_SMU) {
		return _human_SMU->getUseStatus(SM_S_WRITE);
	}
	return -1;
}

void	HumanDB::lock()
{
	if (_human_SMU) {
		sm_lock(_human_SMU->_SMU_header._flag, SM_S_WRITE);
	}
}

void	HumanDB::unLock()
{
	if (_human_SMU) {
		sm_unlock(_human_SMU->_SMU_header._flag);
	}
}

void	HumanDB::validateSMU(bool forceAll/* = false*/)
{
	__ENTER_FUNCTION
	if (_human_SMU)
	{
		lock();
		bool bFlag = false;
		for (int i = EM_ATTR_DB_BASE_BEGIN; i < EM_ATTR_DB_NUM; i++)
		{
			if (getDBAttrDirty((EM_ATTR_DB)i) || forceAll) 
			{
				void* attr = _getAttr(static_cast<EM_ATTR_DB>(i));
				void* smu_attr = _getAttr(static_cast<EM_ATTR_DB>(i), true);
				int size = _getAttrSize(static_cast<EM_ATTR_DB>(i));
				memcpy(smu_attr, attr, size);
				setDBAttrDirty((EM_ATTR_DB)i, false);
				bFlag = true;
				EM_SMUHEAD_ATTRIBUTE_DB smudbattr = EM_SMUHEAD_ATTRIBUTE_DB_INVALID;
				if (i >= EM_ATTR_DB_BASE_BEGIN && i < EM_ATTR_DB_BASE_END) {
					smudbattr = EM_SMUHEAD_ATTRIBUTE_DB_BASE;
				}
				else if (i >= EM_ATTR_DB_BAG_BEGIN && i < EM_ATTR_DB_BAG_END) {
					smudbattr = EM_SMUHEAD_ATTRIBUTE_DB_BAG;
				}
				else if (i>=EM_ATTR_DB_EQUIP_BEGIN && i<EM_ATTR_DB_EQUIP_END) {
					smudbattr = EM_SMUHEAD_ATTRIBUTE_DB_EQUIP;
				}
				if (smudbattr != EM_SMUHEAD_ATTRIBUTE_DB_INVALID) {
					_human_SMU->setUpdateFlag(smudbattr, true);
				}
			}
		}
		if (bFlag) {
			_human_SMU->updateVer();
		}
		unLock();
	}
	__LEAVE_FUNCTION
}
