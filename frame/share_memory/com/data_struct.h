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
/// ��Ʒ����
///###################################
#define GET_ITEM_CLASS(X)		(X/1000000)					//�����Ʒ����
#define GET_ITEM_TYPE(X)		((X%1000000)/10000)	//�����Ʒ������
#define GET_ITEM_INDEX(X)		(X%10000)					//�����Ʒ����
struct ItemData
{
	enum
	{
		ITEM_DATA_BIND = 1,	//��
		ITEM_DATA_COMMON_ATTR_COUNT,	//�������Ը���
	};
	//��ͨ��Ʒ
	struct CommonItemData
	{
		enum
		{
			COMMONITEM_DATA_COUNT = ITEM_DATA_COMMON_ATTR_COUNT,
		};
		uint8	 _count;	//��ǰ����
	};
	//װ��
	struct EquipData
	{
		enum
		{
			EQUIP_DATA_BIND_OWNER = ITEM_DATA_COMMON_ATTR_COUNT, 
			EQUIP_DATA_EQUIPUSE,
			EQUIP_DATA_ENHANCE,
		};
		uint64	_bind_owner;	//��������GUID
		bool		_is_equip_used;	//�Ƿ񱻴���
		uint8		_star;	//ǿ���Ǽ�
	};
	//��ʯ
	struct GemData
	{
		enum
		{
			GEMITEM_DATA_COUNT = ITEM_DATA_COMMON_ATTR_COUNT,
		};
		uint8		_count;	//��ǰ����
	};
	//������Ʒ
	struct QuestItemData
	{
		enum
		{
			QUESTITEM_DATA_COUNT = ITEM_DATA_COMMON_ATTR_COUNT,
		};
		uint8		_count;	//��ǰ����
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

	//�ж��Ƿ���Ч
	bool  isValid() const;
	//������Ʒ����
	void	setItemCount(int nCount);
	//���ӵ�������
	bool	incCount(int nCount = 1);	
	//���ٵ�������
	bool	decCount(int nCount = 1);
	//��õ�������
	int 	getCount() const;
	//��д����
	//X_VOID	Fill(SocketStream& ioStream , EM_TYPE_MSG_MODE mode, X_INT& nSize);
	//�������л�
	//�����Ʒ�Ĺ̶�����
	bool	serializeAttr(char* pOut, int& nOutLength, int nBuffLength) const;
	//������Ʒ�Ĺ̶�����
	bool	unserializeAttr(char* pIn, int nBuffLength);

public:
	int		_itemTableId;	//��Ʒ���
	bool	_bBind;	//�Ƿ��
	Guid	_guid;	//��ƷGUID
	union
	{
		CommonItemData	_common_item_data;	//��ͨ��Ʒ
		EquipData	_equip_data;	//װ��
		GemData	_gem_data;	//��ʯ
		QuestItemData	_quest_item_data;	//������Ʒ
	};
};

///###################################
/// ��һ�������
///###################################
struct HumanData
{
	char	_user[32]; //�˺�
	Guid	_guid;	//guid
	char	_char_name[32];	//��ɫ��
	short	_level;	//�ȼ�
	uint	_money;	//��Ǯ
	int		_bag_size;	//����������
	int		_equip_size;	//װ��������
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
/// ��ұ���������
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
/// ���װ��������
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
/// ���ȫ����������
///###################################
struct FullUserData
{
	HumanData	_human_data;		//��һ�������
	BagData		_bag_data;		//������
	EquipData		_equip_data;		//װ����
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
/// SerialID ����
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
/// �ʼ� ����
///###################################
struct Mail
{
	int	 _index;	//���λ��
	EM_TYPE_MAIL	_type;	//�ʼ�����
	EM_STATUS_MAIL	_status;	//״̬
	Guid	_src_guid;		//������GUID
	char	_src_name[32];	//����������
	int		_date;	//����
	int		_hour_min_sec;	//ʱ����
	char	_title[32];	//����
	char	_context[256] ; //����
	int		_params[MAX_MAIL_PARAM];			//��ִ���ʼ����Ӳ���
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

