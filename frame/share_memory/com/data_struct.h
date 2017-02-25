/**********************************
** @file: data_struct.h
** @brief:  
** @autor: by Mr.Chen
** @date: 2016/07/15 0:58
**********************************/
#ifndef _DATA_DATA_H
#define _DATA_DATA_H

#include "type.h"
#include "dbstruct.h"

///###################################
/// 物品数据
///###################################
#define GET_ITEM_CLASS(X)		(X/1000000)					//获得物品种类
#define GET_ITEM_TYPE(X)		((X%1000000)/10000)	//获得物品子类型
#define GET_ITEM_INDEX(X)		(X%10000)					//获得物品序列
struct ItemData
{
	enum
	{
		ITEM_DATA_BIND = 1,	//绑定
		ITEM_DATA_COMMON_ATTR_COUNT,	//公共属性个数
	};
	//普通物品
	struct CommonItemData
	{
		enum
		{
			COMMONITEM_DATA_COUNT = ITEM_DATA_COMMON_ATTR_COUNT,
		};
		uint8	 _count;	//当前数量
	};
	//装备
	struct EquipData
	{
		enum
		{
			EQUIP_DATA_BIND_OWNER = ITEM_DATA_COMMON_ATTR_COUNT, 
			EQUIP_DATA_EQUIPUSE,
			EQUIP_DATA_ENHANCE,
		};
		uint64	_bind_owner;	//绑定所有者GUID
		bool		_is_equip_used;	//是否被穿上
		uint8		_star;	//强化星级
	};
	//宝石
	struct GemData
	{
		enum
		{
			GEMITEM_DATA_COUNT = ITEM_DATA_COMMON_ATTR_COUNT,
		};
		uint8		_count;	//当前数量
	};
	//任务物品
	struct QuestItemData
	{
		enum
		{
			QUESTITEM_DATA_COUNT = ITEM_DATA_COMMON_ATTR_COUNT,
		};
		uint8		_count;	//当前数量
	};
	
public:
	ItemData()
	{
		cleanUp();
	}
	int		getItemTableID()	const
	{
		return _itemTableId;
	}
	int		getItemClass()	const
	{
		return GET_ITEM_CLASS(_itemTableId);
	}
	int		getItemType() const
	{
		return GET_ITEM_TYPE(_itemTableId);
	}
	int		getItemIndex()	const
	{
		return GET_ITEM_INDEX(_itemTableId);
	}
	CommonItemData*	getCommonItemData() const
	{
		return static_cast<CommonItemData*>(const_cast<CommonItemData*>(&_common_item_data));
	}
	EquipData*	getEquipData() const
	{
		return	static_cast<EquipData*>(const_cast<EquipData*>(&_equip_data));
	}
	GemData*	  getGemData()	const
	{
		return	static_cast<GemData*>(const_cast<GemData*>(&_gem_data));
	}
	QuestItemData*		getQuestItemData()	const
	{
		return	static_cast<QuestItemData*>(const_cast<QuestItemData*>(&_quest_item_data));
	}
	void	cleanUp()
	{
		memset(this, 0, sizeof(*this));
		_itemTableId = -1;
		_guid.cleanUp();
		_equip_data._bind_owner = -1;
	}

	//判断是否有效
	bool  isValid() const;
	//设置物品数量
	void	setItemCount(int nCount);
	//增加叠放数量
	bool	incCount(int nCount = 1);	
	//减少叠放数量
	bool	decCount(int nCount = 1);
	//获得叠加数量
	int 	getCount() const;
	//读写缓冲
	//X_VOID	Fill(SocketStream& ioStream , EM_TYPE_MSG_MODE mode, X_INT& nSize);
	//属性序列化
	//获得物品的固定属性
	bool	serializeAttr(char* pOut, int& nOutLength, int nBuffLength) const;
	//设置物品的固定属性
	bool	unserializeAttr(char* pIn, int nBuffLength);

public:
	int		_itemTableId;	//物品编号
	bool	_bBind;	//是否绑定
	Guid	_guid;	//物品GUID
	union
	{
		CommonItemData	_common_item_data;	//普通物品
		EquipData	_equip_data;	//装备
		GemData	_gem_data;	//宝石
		QuestItemData	_quest_item_data;	//任务物品
	};
};

///###################################
/// 玩家基础数据
///###################################
struct HumanData
{
	char	_user[32]; //账号
	Guid	_guid;	//guid
	char	_char_name[32];	//角色名
	short	_level;	//等级
	uint	_money;	//金钱
	int		_bag_size;	//背包栏容量
	int		_equip_size;	//装备栏容量
	HumanData()
	{
		cleanUp();														   
	}
	void	cleanUp()
	{
		memset(_user, 0, sizeof(_user));
		_guid.cleanUp();
		memset(_char_name, 0, sizeof(_char_name));
		_level = 0;
		_money = 0;
		_bag_size = 0;
		_equip_size = 0;
	}
};

///###################################
/// 玩家背包栏数据
///###################################
struct BagData
{
	ItemData	_item[MAX_BAG_SIZE];
	BagData()
	{
		cleanUp();
	}
	void	cleanUp()
	{
		for (int i=0; i < MAX_BAG_SIZE; ++i)
		{
			_item[i].cleanUp();
		}
	}
	const ItemData*	getItem(int pos) const
	{
		if (pos >= 0 && pos < MAX_BAG_SIZE)  {
			return &_item[pos];
		}
		return NULL;
	}
};

///###################################
/// 玩家装备栏数据
///###################################
struct EquipData
{
	ItemData	_item[MAX_EQUIP_SIZE];
	EquipData() 
	{
		cleanUp();
	}
	void	cleanUp()
	{
		for (int i = 0; i < MAX_EQUIP_SIZE; ++i)
		{
			_item[i].cleanUp();
		}
	}
	const ItemData*	getItem(int pos) const
	{
		if (pos >=0 && pos<MAX_EQUIP_SIZE)  {
			return &_item[pos];
		}
		return NULL;
	}
};

///###################################
/// 玩家全部存盘数据
///###################################
struct FullUserData
{
	HumanData	_human_data;		//玩家基本属性
	BagData		_bag_data;		//背包栏
	EquipData		_equip_data;		//装备栏
	FullUserData()
	{
		cleanUp();
	}
	void	cleanUp()
	{
		_human_data.cleanUp();
		_bag_data.cleanUp();
		_equip_data.cleanUp();
	}
	const ItemData*	GetItemData(EM_ITEM_CONTAINER_TYPE container, int pos) const
	{
		if (container == EM_ITEM_CONTAINER_BAG) 
		{
			if (pos >= 0 && pos < MAX_BAG_SIZE) {
				return &_bag_data._item[pos];
			}
		}
		else if (container == EM_ITEM_CONTAINER_EQUIP) 
		{
			if (pos >= 0 && pos < MAX_EQUIP_SIZE) {
				return &_equip_data._item[pos];
			}
		}
		return NULL;
	}
};

///###################################
/// SerialID 数据
///###################################
struct SerialData
{
	int _obj_type;
	uint _id;
	SerialData()
	{
		cleanUp();
	}
	SerialData(int type, uint id)	
	{
		_obj_type = type;
		_id = id;
	}
	void	cleanUp()
	{
		_obj_type = EM_OBJ_TYPE_INVALID;
		_id = 0;
	}
};

///###################################
/// 邮件 数据
///###################################
struct Mail
{
	int	 _index;	//存放位置
	EM_TYPE_MAIL	_type;	//邮件类型
	EM_STATUS_MAIL	_status;	//状态
	Guid	_src_guid;		//发信人GUID
	char	_src_name[32];	//发信人名字
	int		_date;	//日期
	int		_hour_min_sec;	//时分秒
	char	_title[32];	//标题
	char	_context[256] ; //正文
	int		_params[MAX_MAIL_PARAM];			//可执行邮件附加参数
	Mail()
	{
		cleanUp();
	}
	void cleanUp()
	{
		_index = -1;	
		_type = EM_TYPE_MAIL_INVALID;	
		_status = EM_STATUS_MAIL_INVALID;	
		_src_guid.cleanUp();
		_date=0;
		_hour_min_sec=0;
		memset(_src_name,0,sizeof(_src_name));
		memset(_title, 0, sizeof(_title));
		memset(_context, 0, sizeof(_context));
		for (int i=0;i < MAX_MAIL_PARAM; ++i)
		{
			_params[i]=0;
		}
	}
};

struct MailBoxData
{
	Guid	_owner;
	int		_mail_num;
	Mail	_mail_list[MAX_MAIL_SIZE];

	MailBoxData()
	{
		cleanUp();
	}
	void cleanUp()
	{
		_owner.cleanUp();
		_mail_num=0;
		for (int i = 0;i < MAX_MAIL_SIZE; ++i)
		{
			_mail_list[i].cleanUp();
		}
	}
};

#endif	//_DATA_DATA_H

