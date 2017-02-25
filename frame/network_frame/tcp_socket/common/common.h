/***************************************************
** @file: common.h
** @author: Mr.Chen
** @date: 2016/7/4
** @brief: Server��Client���õĶ���
***************************************************/

#ifndef __COM_H_
#define __COM_H_

#include "type.h"

const int MAX_USER_LEN = 32;

//����״̬
enum EM_PLAYER_STATUS
{
	EM_PLAYER_STATUS_INVALID = -1,
	EM_PLAYER_STATUS_IDLE,						//��ʼ״̬
	EM_PLAYER_STATUS_CONNECT,				//���ӳɹ������N����û�պ������Ϣ����Ͽ�������
	EM_PLAYER_STATUS_AUTHORIZED,		//��֤�ɹ�
	EM_PLAYER_STATUS_ENTER,					//׼��OK
	EM_PLAYER_STATUS_NORMAL,				//�����߼�
	EM_PLAYER_STATUS_NUM,
};

#endif	//__COM_H_

