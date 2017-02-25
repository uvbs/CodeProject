#include "http_request.h"
#include "string_util.h"
#include <string.h>
//using namespace std;

int httpRequest::parserUri()
{
	if (m_requestStr == NULL || str_util::strlen(m_requestStr) == 0)
	{
        return -1;
    }
    const char* pos = ::strstr(m_requestStr,CRLF);
    if (pos == NULL)
	{
        return -2;
    }
    int reqLineLen = pos - m_requestStr;
    char tmpReqLine[MAX_REQLINELEN + 1]={0};
    str_util::strncpy(tmpReqLine, m_requestStr, sizeof(tmpReqLine) - 1);
    m_requestLine = tmpReqLine;

    if (str_util::strncmp(m_requestLine.c_str(), "HTTP", 
		str_util::strlen("HTTP")) == 0)
	{
        m_type = 1;
    }
    else
	{
        m_type = 0;
    }

    vector<string> elements;
    str_util::strToVector(m_requestLine.c_str(), elements, " ");
    if (elements.size() < 3)
	{
        return -3;
    }
    if (m_type == 0)
	{
        m_method=elements[0];
        m_uri=elements[1];
        m_version=elements[2]; 

        vector<string> uris;
		str_util::strToVector(m_uri.c_str(), uris, "?", 2);
        if (uris.size() > 0)
		{
            m_uriname=uris[0]; 
            if (uris.size() > 1)
			{
                m_paramsstr=uris[1];
                vector<string> paramitems;
				str_util::strToVector(m_paramsstr.c_str(), paramitems, "&");
                if (paramitems.size() > 0)
				{
                    for (int i=0; i < paramitems.size(); i++)
					{
                        vector<string> keyvalues;
						str_util::strToVector(paramitems[i].c_str(), keyvalues, 
							"=", 2);
                        if (keyvalues.size() == 2)
						{
                            string key=keyvalues[0];
                            string values= keyvalues[1];
                            m_paramsKeyValues.insert(PARAMSKEYVALUES::value_type(
								key,values));
                        }
                    }
                }
            }   
        }
        return m_type;
    }
    else
	{
        m_version = elements[0];
        m_strStatusCode = elements[1];
        m_nStatusCode = atoi(m_strStatusCode.c_str());
        m_statusDesc = elements[2];
        return m_type;
    }
}

int httpRequest::parserHeader()
{
    if (m_requestStr == NULL || str_util::strlen(m_requestStr) == 0)
	{
        return -1;
    }
    const char* pos = str_util::strstr(m_requestStr,CRLF);
    if (pos == NULL)
	{
        return -2;
    }
    const char* posend = str_util::strstr(pos,CRLFCRLF);
    if(pos == NULL)
	{
        return -3;
    }
    if (posend - pos <= 2)
	{
        return -4;
    }
    const char* posstart = pos + str_util::strlen(CRLF);
    int msgHeaderLen = posend - posstart;
    if (msgHeaderLen == 0)
	{
        return 0;
    }
    char* pTmpMsgHeader = (char*)malloc(msgHeaderLen + 1);
    memset(pTmpMsgHeader, 0, msgHeaderLen + 1);
    memcpy(pTmpMsgHeader, posstart, msgHeaderLen);
    string strMsgHeader = pTmpMsgHeader;

    vector<string> headeritems;
    str_util::strToVector(strMsgHeader.c_str(), headeritems, CRLF);
    if (headeritems.size() > 0)
	{
        for(int i=0; i< headeritems.size(); i++)
		{
            vector<string> keyvalues;
			str_util::strToVector(headeritems[i].c_str(), keyvalues, ":", 2);
            if (keyvalues.size() == 2)
			{
                string key=keyvalues[0];
                string values= keyvalues[1];
                m_headerKeyValues.insert(HEADERKEYVALUES::value_type(key,values));
            }
        }
    }
    free(pTmpMsgHeader);
    return m_headerKeyValues.size();
}

int httpRequest::parserBody()
{    
    if(m_requestStr == NULL || str_util::strlen(m_requestStr) == 0)
	{
        return -1;
    }
    const char* pos = str_util::strstr(m_requestStr, CRLFCRLF);
    if(pos == NULL)
	{
        return -2;
    }
    unsigned int headerlen = pos - m_requestStr + str_util::strlen(CRLFCRLF);
    unsigned int bodylen = m_reqLen - headerlen;
    m_contentLength = bodylen;
    m_content = pos + str_util::strlen(CRLFCRLF);

    return 0;
}

