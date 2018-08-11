/**********************************
** @file: shm_struct.h
** @brief:  
** @autor: by Mr.Chen
** @date: 2016/07/12 23:32
**********************************/
#ifndef _SHM_STRUCT_H
#define _SHM_STRUCT_H

#include "type.h"
#include "shm_def.h"
#include "shm_util.h"
#include "dbstruct.h"
#include "data_struct.h"
#include "myassert.h"

#pragma pack(push, 1)


//�����ڴ��ͷ
struct SMHead
{
	SM_Key _key;
	uint _size; //���������ڴ�����С
	uint _head_ver;	//���������崻�����������
	SMHead() : _key(0), _size(0), _head_ver(0) {}
};

//�����ڴ浥Ԫ���ͷ����Ǹÿ��ʹ�����
struct	SMUHead
{
	int			_use_status;	// �����ڴ��ʹ��״̬�����ߣ����߻�׼������
	char		_flag;		// ��д���
	int			_logic_index;	// ����Ӧ���߼����ݿ�����
	uint		_version;		// ͨ���汾��������ֻ�е����ݷ����仯ʱ�Ŵ���
	uint		_old_version;	
	uint		_save_time;	// ��һ�α�������ʱ��
	BitFlags<uint32>	_update_flags;	// ���Ը���λ���
	Timer	_free_timer;	// �����ʱ��
	SMUHead()
	{
		cleanUp();
	}
	void	cleanUp()
	{
		_use_status	=	SM_USE_FREE;
		_flag			=	SM_FREE;
		_logic_index	=	-1;
		_save_time	=	0;
		_version		=	0;
		_old_version	=	0;
		_update_flags.cleanUp();
		_free_timer.endTimer();
	}
	void	clearVersion()
	{
		_save_time	=	0;
		_version		=	0;
		_old_version	=	0;
		_update_flags.cleanUp();
		_free_timer.endTimer();
	}
}; 

// �����ڴ������Ĵ�������
template<typename T>
struct DBSMU
{
	SMUHead _SMU_header;	//�����ڴ浥Ԫ���ͷ
	T _SMU_data; //�����ڴ������

public:
	DBSMU()
	{
		cleanUp();
	}
	void	cleanUp()
	{
		_SMU_header.cleanUp();
		_SMU_data.cleanUp();
	}
	void	setLogicIndex(uint uIndex)
	{
		_SMU_header._logic_index = uIndex;
	}
	int		getLogicIndex() const
	{
		return _SMU_header._logic_index;
	}
	bool	isDataChanged() const
	{
		return _SMU_header._version > _SMU_header._old_version;
	}
	void	updateOldVer()
	{
		_SMU_header._old_version = _SMU_header._version;
	}
	void	updateVer()
	{
		_SMU_header._version++;
	}
	void	setUpdateFlag(EM_SMUHEAD_ATTRIBUTE_DB offset, bool bitflag)
	{
		_SMU_header._update_flags.updateBits(offset, bitflag);
	}
	bool	isUpdateFlagSet(EM_SMUHEAD_ATTRIBUTE_DB offset)
	{
		return _SMU_header._update_flags.isSetBit(offset);
	}
	BitFlags<uint32>*	getUpdateFlag() {return &_SMU_header._update_flags;}
	void	setUseStatus(int Use, char Type)
	{
		lock(Type);
		_SMU_header._use_status=Use;
		unLock();
	}
	int		getUseStatus(char Type)
	{
		int iRet;
		lock(Type);
		iRet = _SMU_header._use_status;
		unLock();
		return iRet;
	}
	int		getUseStatusFast() const
	{
		return _SMU_header._use_status;
	}
	uint	getTime2Save() const
	{
		return _SMU_header._save_time;
	}
	void	setTime2Save(uint uTime)
	{
		_SMU_header._save_time=uTime;
	}
	char	getLockValueFast() const
	{
		return _SMU_header._flag;
	}
	void	lock(char Type)
	{
		sm_lock(_SMU_header._flag, Type);
	}
	bool	tryLock(char Type)
	{
		return sm_trylock(_SMU_header._flag, Type);
	}
	void	unLock()
	{
		sm_unlock(_SMU_header._flag);
	}
	void	copyDataFromSHM(char Type, T *pDst)
	{
		__ENTER_FUNCTION
		if (NULL == pDst) { return; }
		lock(Type);			
		memcpy(pDst, &_SMU_data, sizeof(T));
		unLock();
		__LEAVE_FUNCTION
	}
	bool	tryCopyDataFromSHM(char Type, T *pDst)
	{
		__ENTER_FUNCTION
		if (NULL == pDst) { return false; }
		if (tryLock(Type))
		{
			memcpy(pDst, &_SMU_data, sizeof(T));
			unLock();
			return true;
		}
		return false;
		__LEAVE_FUNCTION
		return false;
	}	
};


#define HumanSMU DBSMU<FullUserData>
#define	SerialSMU DBSMU<SerialData>
#define MailSMU	DBSMU<MailBoxData>

#pragma pack(pop)


#endif	//_SHM_DEF_H

