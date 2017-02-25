#ifndef __p2pmsg__h__
#define __p2pmsg__h__

#include <time.h>
#ifdef _LINUX64
#include <sys/time.h>
#include <errno.h>
#endif
#include <malloc.h>
#include <string.h>

//消息类型
enum svr_msg_type 
{
	MSG_NONE  = 0,
	MSG_DATA,
    MSG_STATIC,
    MSG_STOP = 99
};

#ifdef _WIN32
struct TimeVal
{
    time_t tv_sec; 
    int tv_usec; 
};
#elif _LINUX64
#define TimeVal timeval
#endif

//消息结构体
class P2pMsgInfo
{
private:
    svr_msg_type m_msgtype;
    uint32 m_datalen;
    uint8 *m_data;
    void* m_param;
    TimeVal m_recvtv;

public:
    P2pMsgInfo()
    {
        m_msgtype=MSG_NONE;
        m_datalen=0;
        m_data=NULL;
        m_param=NULL;

#ifdef _WIN32 
        //get sec
        time(&m_recvtv.tv_sec);
        //get usec
        SYSTEMTIME systemTime;
        GetLocalTime(&systemTime);
        m_recvtv.tv_usec = systemTime.wMilliseconds * 1000; 
#else 
        gettimeofday(&m_recvtv, NULL);
#endif
    }

    virtual ~P2pMsgInfo(void)
    {	
    }

public:
    int setData(uint8* data, uint32 datalen)
    {
        if(datalen>0){
            m_datalen=datalen;
            m_data=(uint8*)malloc(datalen);
            if(m_data){
                memcpy(m_data, data, datalen);
            }
            else{
                m_datalen=0;
                return -1;
            }
        }
        else{
            m_datalen=0;
            m_data=NULL;
            return -1;
        }
        return 0;
    }

    uint8* getData()
    {
        return m_data;
    }

    uint32 getDataLen()
    {
        return m_datalen;
    }
	
    void* getParam()
    {
        return m_param;
    }

    void setParam(void* param)
    {
        m_param=param;
    }
 
    svr_msg_type getMsgType()
    {
        return m_msgtype;
    }
	
    void setMsgType(svr_msg_type type)
    {
        m_msgtype=type;
    }
	
    void getRecvTv(TimeVal &tv)
    {
        tv.tv_sec = m_recvtv.tv_sec;
        tv.tv_usec = m_recvtv.tv_usec;
    }

    void releaseData()
    {
        if(m_data)
        {
            free(m_data);
            m_data=NULL;
        } 
    }
	
};

#endif  //__p2pmsg__h__

