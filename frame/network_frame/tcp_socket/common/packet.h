////////////////////////////////////////////////////////////////////////////////////
// 
// @file: packet.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:25
// @brief: tcp网络包结构
//
////////////////////////////////////////////////////////////////////////////////////

/*
|-----------------------------------------------------------------------------------------------------------------------|
| 包类型编号（2Byte）| 加密标识（1Byte）| 包数据长度（4Byte）|	数据内容|
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
	//获得包id
	virtual Packet_ID	getPacketID() const = 0;
	//获得加密标识
	virtual char	getEncryptFlag() const { return 0; };
	//获得数据大小
	virtual int	getSize() const { return _size; };
	//设置包大小
	virtual void	setSize(int sz) { _size = sz; };
	//包执行
	virtual int	execute(Player* pPlayer) = 0 ;
	//获得数据大小
	virtual bool	fill(SocketStream& ioStream , EM_TYPE_MSG_MODE mode) = 0 ;
	//数据清理
	virtual void	cleanUp() { _size = 0;}
	
protected:
	int _size; //数据大小

private:
	Packet* _pNext; //缓存
};

struct PacketCache 
{
	Packet*	pPacket;		//协议包
	Player_ID	nPlayerId ;	//连接ID
	bool	bRemove;		//删除标记
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
