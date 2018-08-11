#include "data_struct.h"
#include "myassert.h"

// 获得物品的固定属性
bool	ItemData::serializeAttr(char* pOut, int& nOutLength, int nBuffLength) const
{
	__ENTER_FUNCTION
	MyAssert(pOut);
	char* pBuff = pOut;
	//serialize itemtableid
	*((int*)pBuff)	=	_itemTableId;
	pBuff	 += sizeof(_itemTableId);
	//serialize bind
	*((bool*)pBuff)	=	_bBind;
	pBuff	+= sizeof(_bBind);
	//serialize guid
	*((uint64*)pBuff)	=	_guid.value();
	pBuff	+= sizeof(_guid.value());
	//serialize other attr
	int	 itemClass = getItemClass();
	switch(itemClass) 
	{
	case EM_ITEM_CLASS_EQUIP:
		{
			*((uint64*)pBuff)	=	getEquipData()->_bind_owner;
			pBuff	+= sizeof(uint64);
			*((bool*)pBuff)	=	getEquipData()->_is_equip_used;
			pBuff	+= sizeof(bool);
			*((uint8*)pBuff) =	getEquipData()->_star;
			pBuff	+= sizeof(uint8);
		}
		break;
	case EM_ITEM_CLASS_GEM:
		{
			*((uint8*)pBuff)	=	getGemData()->_count;
			pBuff	+= sizeof(uint8);
		}
		break;
	case EM_ITEM_CLASS_COMMON:
		{
			*((uint8*)pBuff)	=	getCommonItemData()->_count;
			pBuff	+= sizeof(uint8);
		}
		break;
	case EM_ITEM_CLASS_QUESTITEM:
		{
			*((uint8*)pBuff)	=	getQuestItemData()->_count;
			pBuff	+= sizeof(uint8);
		}
		break;
	default:
		{
			MyAssert(0);
		}
	}
	nOutLength = (int)(pBuff - pOut);
	MyAssert(nBuffLength >= nOutLength);
	return true;
	__LEAVE_FUNCTION
	return false;
}

// 设置物品固定属性
bool	ItemData::unserializeAttr(char* pIn, int nBuffLength)
{
	__ENTER_FUNCTION
	MyAssert(pIn);
	char* pBuff = pIn;

	//unserialize itemtableid
	_itemTableId = *((int*)pBuff);
	pBuff	 += sizeof(_itemTableId);
	//unserialize bind
	_bBind = *((bool*)pBuff);
	pBuff	+= sizeof(_bBind);
	//unserialize guid
	_guid = *((uint64*)pBuff);
	pBuff	+= sizeof(_guid.value());
	//unserialize other attr
	int itemClass = getItemClass();
	switch(itemClass) 
	{
	case EM_ITEM_CLASS_EQUIP:
		{
			getEquipData()->_bind_owner = *((uint64*)pBuff);
			pBuff	+= sizeof(uint64);
			getEquipData()->_is_equip_used = *((bool*)pBuff);
			pBuff	+= sizeof(bool);
			getEquipData()->_star = *((uint8*)pBuff);
			pBuff	+= sizeof(uint8);
		}
		break;
	case EM_ITEM_CLASS_GEM:
		{
			getGemData()->_count = *((uint8*)pBuff);
			pBuff	+= sizeof(uint8);
		}
		break;
	case EM_ITEM_CLASS_COMMON:
		{
			getCommonItemData()->_count = *((uint8*)pBuff);
			pBuff	+= sizeof(uint8);
		}
		break;
	case EM_ITEM_CLASS_QUESTITEM:
		{
			getQuestItemData()->_count = *((uint8*)pBuff);
			pBuff	+= sizeof(uint8);
		}
		break;
	default:
		{
			MyAssert(0);
		}
	}
	MyAssert(nBuffLength >= (pBuff - pIn));
	return true;
	__LEAVE_FUNCTION
	return false;
}

bool  ItemData::isValid() const
{
	int itemClass = getItemClass();
	switch (itemClass)
	{
	case EM_ITEM_CLASS_COMMON:
	case EM_ITEM_CLASS_EQUIP:
	case EM_ITEM_CLASS_GEM:
	case EM_ITEM_CLASS_QUESTITEM:
		{
			return true;
		}
	default:
		break;
	}
	return false;
}

void	ItemData::setItemCount(int nCount)	
{
	__ENTER_FUNCTION
	MyAssert(nCount > 0);
	int itemClass = getItemClass();
	switch(itemClass)
	{
	case EM_ITEM_CLASS_COMMON:
		{
			getCommonItemData()->_count = nCount;
		}
		break;
	case EM_ITEM_CLASS_EQUIP:
		{
		}
		break;
	case EM_ITEM_CLASS_GEM:
		{
			getGemData()->_count = nCount;
		}
		break;
	case EM_ITEM_CLASS_QUESTITEM:
		{
			getQuestItemData()->_count = nCount;
		}
		break;
	default:
		{
			MyAssert(0);
		}
	}
	__LEAVE_FUNCTION
}

//增加个数
bool	ItemData::incCount(int nCount /* =1 */)
{
	__ENTER_FUNCTION
	MyAssert(nCount > 0);
	int itemClass = getItemClass();
	switch(itemClass)
	{
	case EM_ITEM_CLASS_COMMON:
		{
			getCommonItemData()->_count += nCount;
		}
		break;
	case EM_ITEM_CLASS_EQUIP:
		{
		}
		break;
	case EM_ITEM_CLASS_GEM:
		{
			getGemData()->_count += nCount;
		}
		break;
	case EM_ITEM_CLASS_QUESTITEM:
		{
			getQuestItemData()->_count += nCount;
		}
		break;
	default:
		{
			MyAssert(0);
		}
	}
	return true;
	__LEAVE_FUNCTION
	return false;
}

//减少个数
bool	ItemData::decCount(int nCount /* =1 */)
{
	__ENTER_FUNCTION
	MyAssert(nCount > 0);
	int itemClass = getItemClass();
	switch(itemClass)
	{
	case EM_ITEM_CLASS_COMMON:
		{
			getCommonItemData()->_count -= nCount;
		}
		break;
	case EM_ITEM_CLASS_EQUIP:
		{
		}
		break;
	case EM_ITEM_CLASS_GEM:
		{
			getGemData()->_count -= nCount;
		}
		break;
	case EM_ITEM_CLASS_QUESTITEM:
		{
			getQuestItemData()->_count -= nCount;
		}
		break;
	default:
		{
			MyAssert(0);
		}
	}
	return true;
	__LEAVE_FUNCTION
	return false;
}

int		ItemData::getCount() const
{
	__ENTER_FUNCTION
	int itemClass = getItemClass();
	switch (itemClass)
	{
	case EM_ITEM_CLASS_COMMON:
		{
			return getCommonItemData()->_count;
		}
		break;
	case EM_ITEM_CLASS_EQUIP:
		{
			return 1;
		}
		break;
	case EM_ITEM_CLASS_GEM:
		{
			return getGemData()->_count;
		}
		break;
	case EM_ITEM_CLASS_QUESTITEM:
		{
			return getQuestItemData()->_count;
		}
		break;
	default:
		{
			MyAssert(0);
		}
	}
	__LEAVE_FUNCTION
	return 0;
}
