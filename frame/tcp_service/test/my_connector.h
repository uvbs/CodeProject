#ifndef __my_connector__h__
#define __my_connector__h__

#ifdef _LINUX64
#include <sys/time.h>
#endif
#include <time.h>
#include <event2/event.h>
#include "connector.h"

class MyConnector : public Connector
{
public:
    MyConnector(char* channelID)
    {
		m_valid = true;
        m_bClose = false;
    }

    virtual ~MyConnector(){};

public:
    int onPacket(char* buf, size_t buf_len);
    int onTimeout();
    int onSendTimeout();
    int onClose(int type);
    //int onConnect();

public:
    void setClose()
    {
        m_bClose = true;
    };

    void setInvalid()
    {
        m_valid = false;
    };

    bool getValid()
    {
	return m_valid;
    };
private:
    bool m_bClose;
    int  m_readTimeoutCount;
    bool m_valid;
};
#endif
