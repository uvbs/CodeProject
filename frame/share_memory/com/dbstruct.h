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
const int MAX_MAIL_PARAM = 6; //���Ӳ�������
const int MAX_MAIL_SIZE = 50;	//�ʼ��������

enum EM_OBJ_TYPE
{
	EM_OBJ_TYPE_INVALID = -1,
	EM_OBJ_TYPE_HUMAN,	//���
	EM_OBJ_TYPE_ITEM,		//��Ʒ
	EM_OBJ_TYPE_MAIL,		//�ʼ�
	EM_OBJ_TYPE_NUM,
};

// ��Ʒ���
enum EM_ITEM_CLASS
{
	EM_ITEM_CLASS_INVALID		=	-1,
	EM_ITEM_CLASS_EQUIP			=	10,		// װ��
	EM_ITEM_CLASS_GEM			=	11,		// ��ʯ
	EM_ITEM_CLASS_COMMON		=	12,		// ��ͨ��Ʒ
	EM_ITEM_CLASS_QUESTITEM	=	13,		// ������Ʒ
	EM_ITEM_CLASS_NUMBER,
};

//��Ʒ��������
enum EM_ITEM_CONTAINER_TYPE
{
	EM_ITEM_CONTAINER_INVALID = -1,
	EM_ITEM_CONTAINER_BAG,			//������
	EM_ITEM_CONTAINER_EQUIP,		//װ����
	EM_ITEM_CONTAINER_NUMBER,
};

// ��ұ仯��DB����ö��
enum EM_ATTR_DB
{
	EM_ATTR_DB_INVALID = -1,
	//////////////////////////////////////////////////////////////////////////
	//��������
	//////////////////////////////////////////////////////////////////////////
	EM_ATTR_DB_BASE_BEGIN,	//�������Կ�ʼλ��
	EM_ATTR_DB_GUID = EM_ATTR_DB_BASE_BEGIN,	//GUID
	EM_ATTR_DB_USER,	//UserID
	EM_ATTR_DB_NAME,	//��ɫ����
	EM_ATTR_DB_LEVEL,		//�ȼ�
	EM_ATTR_DB_MONEY,	//��Ǯ
	EM_ATTR_DB_BAGSIZE,	//��������С
	EM_ATTR_DB_EQUIPSIZE,	//װ������С
	EM_ATTR_DB_BASE_END,				//�������Խ���λ��
	//////////////////////////////////////////////////////////////////////////
	//������
	//////////////////////////////////////////////////////////////////////////
	EM_ATTR_DB_BAG_BEGIN = EM_ATTR_DB_BASE_END,		//��������ʼλ��
	EM_ATTR_DB_BAG_END	= EM_ATTR_DB_BAG_BEGIN + MAX_BAG_SIZE,	//����������λ��
	//////////////////////////////////////////////////////////////////////////
	//װ����
	//////////////////////////////////////////////////////////////////////////
	EM_ATTR_DB_EQUIP_BEGIN = EM_ATTR_DB_BAG_END,		//װ������ʼλ��
	EM_ATTR_DB_EQUIP_END	= EM_ATTR_DB_EQUIP_BEGIN + MAX_EQUIP_SIZE,	//װ��������λ��
	EM_ATTR_DB_NUM,
};

//SMUHead��DB����ö��
enum EM_SMUHEAD_ATTRIBUTE_DB
{
	EM_SMUHEAD_ATTRIBUTE_DB_INVALID = -1,
	EM_SMUHEAD_ATTRIBUTE_DB_BASE,			//��������
	EM_SMUHEAD_ATTRIBUTE_DB_BAG,			//������
	EM_SMUHEAD_ATTRIBUTE_DB_EQUIP,		//װ����
	EM_SMUHEAD_ATTRIBUTE_DB_NUMBER,
};

//�ʼ�����
enum EM_TYPE_MAIL
{
	EM_TYPE_MAIL_INVALID=-1,
	EM_TYPE_MAIL_NORMAL,	//��ͨ�ʼ�
	EM_TYPE_MAIL_SYSTEM,	//ϵͳ�ʼ�
	EM_TYPE_MAIL_EXE,	//��ִ���ʼ�
	EM_TYPE_MAIL_COUNT
};

//�ʼ�״̬
enum EM_STATUS_MAIL
{
	EM_STATUS_MAIL_INVALID=-1,
	EM_STATUS_MAIL_UNREAD,				//δ��
	EM_STATUS_MAIL_READ,					//�Ѷ�
	EM_STATUS_MAIL_COUNT
};

//��ִ���ʼ��������� �����ڵ�0��������
enum EM_MAIL_EXE_TYPE
{
	EM_MAIL_EXE_TYPE_INVALID=-1,
	EM_MAIL_EXE_TYPE_GM,		//GM ��ֵ
	EM_MAIL_EXE_TYPE_GM_ITEM,	//GM ����Ʒ
	EM_MAIL_EXE_TYPE_COUNT,

};

//GUID�ṹ��
struct Guid
{
private:
	//����
	//63-32 Bits��ServerGroupID
	//31-0  Bits��SerialID
	uint64	_serialId;	//(64λ)

public:
	Guid() : _serialId(-1) {}
	Guid(uint64 v) : _serialId(v) {}
    Guid(const Guid& rhs) { _serialId = rhs._serialId; }
	//�ɸ�32λ�͵�32λ����GUID
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
	//�ɷ��������+���к�����GUID
	void	init(uint srvGroupId, uint serialId, EM_OBJ_TYPE type)
	{
		uint32 high = srvGroupId; //���ɸ�32λ
		uint32 low = -1; //���ɵ�32λ
		low &= (type << 29);
		low |= serialId;
		*this = Guid(high, low);
		/*---------------------
		ע����㷨����nType�ﵽ8��ʱ�򣬾ͻ��������
		��SerialIDΪ0ʱ��nType��0��10ʱ���õ���m_SerialIDֵ���£�
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
	//�õ�Obj����
	EM_OBJ_TYPE getType() const
	{
		if (_serialId == -1) { return EM_OBJ_TYPE_INVALID; }
		uint32 serialId = low();
		return (EM_OBJ_TYPE)(serialId >> 29);
	}
	//���
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

// �������ע��
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
	void*	m_AttrPtr[EM_ATTR_DB_NUM];	//��ɫ����ע��
	int		m_AttrSize[EM_ATTR_DB_NUM];	//��ɫ���Դ�С
};

#pragma pack(pop)

#endif	//_DB_STRUCT_H
