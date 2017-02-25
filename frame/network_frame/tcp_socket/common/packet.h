////////////////////////////////////////////////////////////////////////////////////
// 
// @file: packet.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:25
// @brief: tcp������ṹ
//
////////////////////////////////////////////////////////////////////////////////////

/*
|-----------------------------------------------------------------------------------------------------------------------|
| �����ͱ�ţ�2Byte��| ���ܱ�ʶ��1Byte��| �����ݳ��ȣ�4Byte��|	��������|
|------------------------------------------------------------------------------------------------------------------------|
*/

#ifndef __PACKET_H_
#define __PACKET_H_

#include "type.h"
#include "socketdef.h"
#include "socket_stream.h"

namespace tcp_net {

class Player;
////////////////////////////////////////////////////////////////////////////////
//	class Packet
////////////////////////////////////////////////////////////////////////////////
class Packet
{
	friend class PacketFactory;
public:
	Packet() : _pNext(NULL), _size(0) {}
	virtual ~Packet() {}
	//��ð�id
	virtual Packet_ID	getPacketID() const = 0;
	//��ü��ܱ�ʶ
	virtual char	getEncryptFlag() const { return 0; };
	//������ݴ�С
	virtual int	getSize() const { return _size; };
	//���ð���С
	virtual void	setSize(int sz) { _size = sz; };
	//��ִ��
	virtual int	execute(Player* pPlayer) = 0 ;
	//������ݴ�С
	virtual bool	fill(SocketStream& ioStream , EM_TYPE_MSG_MODE mode) = 0 ;
	//��������
	virtual void	cleanUp() { _size = 0;}
	
protected:
	int _size; //���ݴ�С

private:
	Packet* _pNext; //����
};

struct PacketCache 
{
	Packet*	pPacket;		//Э���
	Player_ID	nPlayerId ;	//����ID
	bool	bRemove;		//ɾ�����
	PacketCache() : pPacket(NULL), nPlayerId(-1), bRemove(false) {}
	void cleanUp()
	{
		pPacket = NULL;
		nPlayerId = -1;
		bRemove = false;
	}
};

} //namespace tcp_net

using namespace tcp_net;

#endif
