#include "object_human.h"
#include "string_util.h"
#include "time_system.h"

bool	ObjectHuman::init(const FullUserData* pUserDB)
{
	Assert(pUserDB);
	bool bRet = Object::init();
	Assert(bRet);
	_human_db.fillFullUserData(pUserDB);
	initAttrBackUp();
	_human_db.validateSMU(true);
	return true;
}

void	ObjectHuman::initAttrBackUp()
{
	if (getName())
	{
		str_util::strcpy_s(_attr_backup._char_name, getName(), sizeof(_attr_backup._char_name));
	}
	_attr_backup._level = getLevel();
	_attr_backup._money = getMoney();
}

void ObjectHuman::cleanUp()
{
	_human_db.cleanUp();
	_dbsave_timer.endTimer();
}

bool	ObjectHuman::heartBeat(uint uTime)
{
	if (!_dbsave_timer.isSet())
	{
		_dbsave_timer.beginTimer(30 * 1000, uTime);
	}
	else if (_dbsave_timer.isReach(uTime))
	{
		_human_db.validateSMU();
	}

	bool bRet = Object::heartBeat(uTime);
	Assert(bRet);

	TimeSystem* pTimeSystem = TimeSystem::getSinglePtr();
	Assert(pTimeSystem);
	if (_last_fresh_day != pTimeSystem->getToday())
	{
		_last_fresh_day = pTimeSystem->getToday();
		onDayChanged();
	}
	if (_last_fresh_time != pTimeSystem->getCurrHourMinute() / 100)
	{
		_last_fresh_time = pTimeSystem->getCurrHourMinute() / 100;
		onHourChanged();
	}
	return true;
}

//ÊôÐÔË¢ÐÂ
void	ObjectHuman::sendFreshAttrs()
{
	//bool freshFlag = false;
	//GCRetHumanDetailAttrPacket detailPacket;
	//detailPacket.SetGuid(GetGuid());
	//if (m_AttributeBackup._level != GetLevel())
	//{
	//	detailPacket.SetLevel(GetLevel());
	//	m_AttributeBackup._level = GetLevel();
	//	freshFlag = true;
	//}
	//if (m_AttributeBackup._money != GetMoney())
	//{
	//	detailPacket.SetGold(GetGold());
	//	m_AttributeBackup._money = GetGold();
	//	freshFlag = true;
	//}
	//if (freshFlag)
	//{
 //		SendPacketToClient(&detailPacket);
	//}
}

bool	ObjectHuman::_setMoney(uint money)
{
	if (money >= uint(-1) - 1)
	{
		money = uint(-1) - 1;
	}
	_human_db.setMoney(money);
	return true;
}

bool	ObjectHuman::consumeMoney(int money)
{
	 if (money < 0) { return false; }
	 uint currentMoney = getMoney();
	 if (currentMoney < money) { return false; }
	 currentMoney -= money;
	 _setMoney(currentMoney);
	 return true;
}

bool	ObjectHuman::addMoney(int money)
{
	if (money < 0) { return false; }
	uint currentMoney = getMoney();
	if (currentMoney >= uint(-1) - 1) { return true; }
	if (money > uint(-1) - 1 - currentMoney)
	{
		_setMoney(uint(-1) - 1);
		return true;
	}
	currentMoney += money;
	_setMoney(currentMoney);

	return true;
}

bool ObjectHuman::executeMail(Mail* pMail)
{
	Assert(pMail);
	EM_MAIL_EXE_TYPE mailExeType =   (EM_MAIL_EXE_TYPE)pMail->_params[0];
	switch (mailExeType)
	{
	case  EM_MAIL_EXE_TYPE_GM:
		{
			addMoney(pMail->_params[1]);
		}
		break;
	case EM_MAIL_EXE_TYPE_GM_ITEM:
		{
		}
		break;
	default:
		break;
	}
	pMail->_params[0] = EM_MAIL_EXE_TYPE_INVALID;

	return true;
}
