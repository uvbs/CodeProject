/**********************************
** @file: dbstruct.h
** @brief:  
** @autor: by Mr.Chen
** @date: 2016/07/15 0:37
**********************************/
#ifndef _DB_STRUCT_H
#define _DB_STRUCT_H

#include "type.h"
#include <string.h>

#pragma pack(push, 1)

const uint64 GUID_MASK  =	0x0FFFFFFFF;
const int MAX_BAG_SIZE = 256;
const int MAX_EQUIP_SIZE = 256;
const int MAX_MAIL_PARAM = 6; //附加参数个数
const int MAX_MAIL_SIZE = 50;	//邮件最大数量

enum EM_OBJ_TYPE
{
	EM_OBJ_TYPE_INVALID = -1,
	EM_OBJ_TYPE_HUMAN,	//玩家
	EM_OBJ_TYPE_ITEM,		//物品
	EM_OBJ_TYPE_MAIL,		//邮件
	EM_OBJ_TYPE_NUM,
};

// 物品类别
enum EM_ITEM_CLASS
{
	EM_ITEM_CLASS_INVALID		=	-1,
	EM_ITEM_CLASS_EQUIP			=	10,		// 装备
	EM_ITEM_CLASS_GEM			=	11,		// 宝石
	EM_ITEM_CLASS_COMMON		=	12,		// 普通物品
	EM_ITEM_CLASS_QUESTITEM	=	13,		// 任务物品
	EM_ITEM_CLASS_NUMBER,
};

//物品容器类型
enum EM_ITEM_CONTAINER_TYPE
{
	EM_ITEM_CONTAINER_INVALID = -1,
	EM_ITEM_CONTAINER_BAG,			//背包栏
	EM_ITEM_CONTAINER_EQUIP,		//装备栏
	EM_ITEM_CONTAINER_NUMBER,
};

// 玩家变化的DB属性枚举
enum EM_ATTR_DB
{
	EM_ATTR_DB_INVALID = -1,
	//////////////////////////////////////////////////////////////////////////
	//基本属性
	//////////////////////////////////////////////////////////////////////////
	EM_ATTR_DB_BASE_BEGIN,	//基础属性开始位置
	EM_ATTR_DB_GUID = EM_ATTR_DB_BASE_BEGIN,	//GUID
	EM_ATTR_DB_USER,	//UserID
	EM_ATTR_DB_NAME,	//角色名字
	EM_ATTR_DB_LEVEL,		//等级
	EM_ATTR_DB_MONEY,	//金钱
	EM_ATTR_DB_BAGSIZE,	//背包栏大小
	EM_ATTR_DB_EQUIPSIZE,	//装备栏大小
	EM_ATTR_DB_BASE_END,				//基础属性结束位置
	//////////////////////////////////////////////////////////////////////////
	//背包栏
	//////////////////////////////////////////////////////////////////////////
	EM_ATTR_DB_BAG_BEGIN = EM_ATTR_DB_BASE_END,		//背包栏开始位置
	EM_ATTR_DB_BAG_END	= EM_ATTR_DB_BAG_BEGIN + MAX_BAG_SIZE,	//背包栏结束位置
	//////////////////////////////////////////////////////////////////////////
	//装备栏
	//////////////////////////////////////////////////////////////////////////
	EM_ATTR_DB_EQUIP_BEGIN = EM_ATTR_DB_BAG_END,		//装备栏开始位置
	EM_ATTR_DB_EQUIP_END	= EM_ATTR_DB_EQUIP_BEGIN + MAX_EQUIP_SIZE,	//装备栏结束位置
	EM_ATTR_DB_NUM,
};

//SMUHead的DB属性枚举
enum EM_SMUHEAD_ATTRIBUTE_DB
{
	EM_SMUHEAD_ATTRIBUTE_DB_INVALID = -1,
	EM_SMUHEAD_ATTRIBUTE_DB_BASE,			//基本属性
	EM_SMUHEAD_ATTRIBUTE_DB_BAG,			//背包栏
	EM_SMUHEAD_ATTRIBUTE_DB_EQUIP,		//装备栏
	EM_SMUHEAD_ATTRIBUTE_DB_NUMBER,
};

//邮件类型
enum EM_TYPE_MAIL
{
	EM_TYPE_MAIL_INVALID=-1,
	EM_TYPE_MAIL_NORMAL,	//普通邮件
	EM_TYPE_MAIL_SYSTEM,	//系统邮件
	EM_TYPE_MAIL_EXE,	//可执行邮件
	EM_TYPE_MAIL_COUNT
};

//邮件状态
enum EM_STATUS_MAIL
{
	EM_STATUS_MAIL_INVALID=-1,
	EM_STATUS_MAIL_UNREAD,				//未读
	EM_STATUS_MAIL_READ,					//已读
	EM_STATUS_MAIL_COUNT
};

//可执行邮件功能类型 保存在第0个参数上
enum EM_MAIL_EXE_TYPE
{
	EM_MAIL_EXE_TYPE_INVALID=-1,
	EM_MAIL_EXE_TYPE_GM,		//GM 数值
	EM_MAIL_EXE_TYPE_GM_ITEM,	//GM 加物品
	EM_MAIL_EXE_TYPE_COUNT,

};

//GUID结构体
struct Guid
{
private:
	//规则：
	//63-32 Bits：ServerGroupID
	//31-0  Bits：SerialID
	uint64	_serialId;	//(64位)

public:
	Guid() : _serialId(-1) {}
	Guid(uint64 v) : _serialId(v) {}
    Guid(const Guid& rhs) { _serialId = rhs._serialId; }
	//由高32位和低32位生成GUID
	Guid(uint32 high, uint32 low)
	{
		_serialId = high;
		_serialId = (_serialId & GUID_MASK) << 32;
		_serialId = _serialId | low;
	}
	void	cleanUp() { _serialId = -1; }
	uint64	value() const { return _serialId; }
	uint32	high() const { return (_serialId >> 32) & GUID_MASK; }
	uint32	low() const { return _serialId & GUID_MASK; }
	//由服务器组号+序列号生成GUID
	void	init(uint srvGroupId, uint serialId, EM_OBJ_TYPE type)
	{
		uint32 high = srvGroupId; //生成高32位
		uint32 low = -1; //生成低32位
		low &= (type << 29);
		low |= serialId;
		*this = Guid(high, low);
		/*---------------------
		注意此算法，当nType达到8的时候，就会出现问题
		当SerialID为0时，nType从0到10时，得到的m_SerialID值如下：
		0
		536870912
		1073741824
		1610612736
		2147483648
		2684354560
		3221225472
		3758096384
		0
		536870912
		1073741824
		-------------------------*/
	}
	//得到Obj类型
	EM_OBJ_TYPE getType() const
	{
		if (_serialId == -1) { return EM_OBJ_TYPE_INVALID; }
		uint32 serialId = low();
		return (EM_OBJ_TYPE)(serialId >> 29);
	}
	//检查
	bool isValid() const { return _serialId != -1; }
	Guid& operator = (const Guid& rhs) {
		if (&rhs == this) {	return *this; }
		_serialId = rhs._serialId;
		return *this;
	}
	bool operator == (Guid& rhs) { return _serialId == rhs._serialId; }
	bool operator == (const Guid& rhs) const { return _serialId == rhs._serialId; }
	bool operator < (Guid& rhs) { return _serialId < rhs._serialId; }
	bool operator < (const Guid& rhs) const { return _serialId < rhs._serialId; }
    bool operator != (Guid& rhs) { return _serialId != rhs._serialId; }
    bool operator != (const Guid& rhs) const { return _serialId == rhs._serialId; }
};

// 玩家属性注册
struct HumanDB_ATTR_REG
{
	HumanDB_ATTR_REG()
	{
		cleanUp();
	}
	void	cleanUp()
	{
		memset(this, 0, sizeof(*this));
	}
	void*	m_AttrPtr[EM_ATTR_DB_NUM];	//角色属性注册
	int		m_AttrSize[EM_ATTR_DB_NUM];	//角色属性大小
};

#pragma pack(pop)

#endif	//_DB_STRUCT_H
