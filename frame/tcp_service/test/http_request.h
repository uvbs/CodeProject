#ifndef _http_request_h_
#define _http_request_h_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <list>
#include <map>
#include "string_util.h"

using namespace std;

#define    CRLF              "\r\n"
#define    CRLFCRLF          "\r\n\r\n"
#define    MAX_REQLINELEN    4096    


typedef map<string, string, less<string> > HEADERKEYVALUES;
typedef map<string, string, less<string> > PARAMSKEYVALUES;

class httpRequest{
public:
    httpRequest(char * request, unsigned int reqLen) : m_requestStr(request),
        m_reqLen(reqLen), m_content(NULL), m_contentLength(0){
    }
    
    ~httpRequest(){
    }

    int parserUri();
    int parserHeader();
    int parserBody();

    char* getMethod(){return (char*)m_method.c_str();};
    char* getUri(){return (char*)m_uri.c_str();};
    char* getUriName(){return (char*)m_uriname.c_str();};
    char* getVersion(){return (char*)m_version.c_str();};

    const char* getContent(){return m_content;};
    unsigned int getContentLength(){return m_contentLength;};
    //void freeContent();

    HEADERKEYVALUES* getHeaderKeyValue(){return &m_headerKeyValues;};
    PARAMSKEYVALUES* getParamKeyValue(){return &m_paramsKeyValues;};

    const char* getRequestStr(){return m_requestStr;};
    int getType(){return m_type;};

    int getStatusCode()
    {
	return m_nStatusCode;
    };

    char* getStatusDesc()
    {
	return (char*)m_statusDesc.c_str();
    };

protected:
    const char* m_requestStr;
    unsigned int m_reqLen;
    const char* m_content;
    unsigned int m_contentLength;

    string m_requestLine;
    string m_method;
    string m_uri;
    string m_uriname;
    string m_paramsstr;
    string m_version; 
    string m_strStatusCode;
    int    m_nStatusCode;
    string m_statusDesc;

    PARAMSKEYVALUES m_paramsKeyValues;
    HEADERKEYVALUES m_headerKeyValues;
    int m_type;  //类型 0 请求， 1响应
};

#endif
