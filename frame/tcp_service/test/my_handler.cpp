#include "my_handler.h"
#include "my_connector.h"
//#include "json/json.h"

int MyHandler::packSuccessRes(char *&retBuf, int& retBufLen, const char* body, int bodylen)
{
    int packret = 0;
    char* pBody = (char*)body;
    int nBodyLen = bodylen;
    char headerBuf[1024]={0};
    static const char* httpHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n";
	snprintf(headerBuf, sizeof(headerBuf), "%s", httpHeader);
    
    char tmpContentLen[32]={0};
	snprintf(tmpContentLen, sizeof(tmpContentLen), "%d", nBodyLen);
	::strncat(headerBuf, "Content-Length: ", sizeof(headerBuf));
	::strncat(headerBuf, tmpContentLen, sizeof(headerBuf));
	::strncat(headerBuf, "\r\n", sizeof(headerBuf));
	::strncat(headerBuf, "\r\n", sizeof(headerBuf));

	int headerlen = str_util::strlen(headerBuf);
    retBufLen =headerlen + nBodyLen ;
    retBuf = (char*)malloc(retBufLen);
    if(retBuf != NULL){
        memcpy(retBuf, headerBuf, headerlen);
        memcpy(retBuf+headerlen, pBody, nBodyLen);
        packret = retBufLen;
    }
    else
    {
        printf("packSuccessRes malloc mem fail, need:%d\n",retBufLen);
        packret =  -1;
    }

    return packret;
}

int MyHandler::handleData(ServerClient* client, char* buf, size_t buf_len)
{
    //处理吧嗯本程序为server端的连接上的请求
    httpRequest* httpReq = new httpRequest((char*)buf, buf_len);
    if(httpReq->parserUri() == 0){
        printf("MyHandler::handleData::onPacket : Parser Uri Success Method:%s,UriName:%s\n",
            httpReq->getMethod(), httpReq->getUriName());

        char *retBuf = NULL;
        int  retBufLen = 0;
		if(::strcmp(httpReq->getMethod(), "GET")==0 && ::strcmp(httpReq->getUriName(), "/register")==0){
            //注册
	        this->regUser(client, httpReq);
        }
		else if(::strcmp(httpReq->getMethod(), "GET")==0 && ::strcmp(httpReq->getUriName(), "/delchannel")==0){
	        //删除频道
	        //this->delChannel(client, httpReq);
	    }
    }

    delete httpReq;

    return 0;
}

int MyHandler::handleData(Connector* client, char* buf, size_t buf_len)
{
    //处理本程序为客户端的连接懂行的请求	
    httpRequest* httpReq = new httpRequest((char*)buf, buf_len);
    if(httpReq->parserUri() == 0){

        char *retBuf = NULL;
        int  retBufLen = 0;
		if(::strcmp(httpReq->getMethod(), "POST")==0 && ::strcmp(httpReq->getUriName(), "/push")==0){
	    //push数据
	    //static_cast<MyConnector*>(client)->resetReadTimeoutCount();
            this->pushData(client, httpReq);
        }
    }
    else if(httpReq->parserUri() == 1) {
        this->handleResp(client, httpReq);
    }

    delete httpReq; 
    return 0;
}

int MyHandler::pushData(Connector* client, httpRequest* httpReq)
{
    string strBlockID="";
    string strStart="";
    string strSize="";
    int nBlockID = 0;
    int nStart = 0;
    int nSize = 0;

    PARAMSKEYVALUES* pParamKeyValue = httpReq->getParamKeyValue();
    PARAMSKEYVALUES::iterator itParam= pParamKeyValue->find("blockid");
    if(itParam != pParamKeyValue->end()){
        strBlockID = itParam->second;
        nBlockID = atoi(strBlockID.c_str());
    }
    else{
        printf("MyHandler::pushData not param blockid:%s\n",
            httpReq->getUri());
        return -1;
    }

    itParam= pParamKeyValue->find("start");
    if(itParam != pParamKeyValue->end()){
        strStart = itParam->second;
        nStart = atoi(strStart.c_str());
    }
    else{
        printf("MyHandler::pushData not param start:%s\n",
            httpReq->getUri());
        return -1;
    }

    itParam= pParamKeyValue->find("size");
    if(itParam != pParamKeyValue->end()){
        strSize = itParam->second;
        nSize = atoi(strSize.c_str());
    }
    else{
        printf("MyHandler::pushData not param size:%s\n",
            httpReq->getUri());
        return -1;
    }

    char channelID[64] = {0};
    //strncpy(channelID, static_cast<MyConnector*>(client)->getChannelID(), 64);
	::strncpy(channelID,"abcdefghijk123", 64);
    
    if(static_cast<MyConnector*>(client)->getValid() == false){
	printf( "MyHandler::pushData channelID:%s will close\n",
	    channelID);
	return -1;
    }

    if(nBlockID%100 == 0){
        printf("MyHandler::pushData channelID:%s, blockID:%d, start:%d, size:%d\n",
            channelID, nBlockID, nStart, nSize);
    }

    if(httpReq->parserBody()== 0){
        if(httpReq->getContentLength() != nSize){
            printf("MyHandler::pushData ContentLength:%d != size:%d\n",
                httpReq->getContentLength(), nSize);
            return -1;
        }
    }

    //Channel *channel = gusvr.getChannel(channelID);
    //if(channel != NULL){
    //    channel->writeData(httpReq->getContent(), nSize, nBlockID, nStart);
    //}

    char* retBuf=NULL;
    int retBufLen = 0;
	if(packSuccessRes(retBuf, retBufLen, "req=pushdata", str_util::strlen("req=pushdata")) > 0){
	    client->send_msg(retBuf, retBufLen);
	    free(retBuf);
    }
    
    return 0;
}

int MyHandler::handleResp(Connector* client, httpRequest* httpReq)
{
    if(httpReq->getStatusCode() != 200){
	//注册频道失败    
	printf("handleResp status is not 200, %d %s\n",
	    httpReq->getStatusCode(), httpReq->getStatusDesc());
	//static_cast<MyConnector*>(client)->setClose();
	client->close();
        return 0;	
    }	

  /*  if(httpReq->parserBody()== 0){
        string respData =  string(httpReq->getContent(), httpReq->getContentLength());
        
        json_object* resp_obj = json_tokener_parse(respData.c_str());
        if(is_error(resp_obj) || resp_obj == NULL){
            com_writelog(COMLOG_NOTICE, "handleResp resp:%s not json",
                respData.c_str());

            return -1;
        }

        if(!json_object_is_type(resp_obj, json_type_object)){
            com_writelog(COMLOG_WARNING, "handleResp param resp is not object, resp:%s",
                respData.c_str());
            json_object_put(resp_obj);
            return -1;
        }

        struct json_object * channelid_obj, * blocksize_obj;
        channelid_obj  = json_object_object_get(resp_obj, "fid");
        blocksize_obj   = json_object_object_get(resp_obj, "blocksize");

        if(NULL == channelid_obj || is_error(channelid_obj)){
            com_writelog(COMLOG_WARNING, "handleResp resp data err, not fid, resp:%s",
                respData.c_str());
            json_object_put(resp_obj);
            return -1;
        }

        if(NULL == blocksize_obj || is_error(blocksize_obj)){
            com_writelog(COMLOG_WARNING, "handleResp resp data err, not blocksize, resp:%s",
                respData.c_str());
            json_object_put(resp_obj);
            return -1;
        }

        
        if(!json_object_is_type(channelid_obj, json_type_string)){
            com_writelog(COMLOG_WARNING, "handleResp resp fid is not string, resp:%s",
                respData.c_str());
            json_object_put(resp_obj);
            return -1;
        }

        std::string channelid = json_object_get_string(channelid_obj);

        if(!json_object_is_type(blocksize_obj, json_type_int)){
            com_writelog(COMLOG_WARNING, "handleResp resp blocksize is not int, resp:%s",
                respData.c_str());
            json_object_put(resp_obj);
            return -1;
        }

       int blocksize = json_object_get_int(blocksize_obj);
       json_object_put(resp_obj);

       if(blocksize <= 0){
            com_writelog(COMLOG_WARNING, "handleResp resp blocksize <= 0, resp:%s",
                respData.c_str());
            return -1;        
       }

       if(strncmp(channelid.c_str(), static_cast<MyConnector*>(client)->getChannelID(), HEX_CHANNELID_LEN) != 0)
       {
           com_writelog(COMLOG_WARNING, "handleResp resp channelid:%s is not equal connector channelid:%s",
                channelid.c_str(), static_cast<MyConnector*>(client)->getChannelID());
           return -1;
       }

       com_writelog(COMLOG_NOTICE, "handleResp resp channelid:%s, blocksize:%d",
           channelid.c_str(), blocksize); 
      
       Channel *pCh = gusvr.getChannel(channelid);
       if(pCh == NULL)
	   {	       
           unsigned char bincid[CHANNELID_LEN] = {0};
           hex2bin(channelid, bincid, CHANNELID_LEN+1);
           int sectionCount = gusvr.getSectionCount();
           int blockCount = gusvr.getBlockCount();
           Channel *ch = new Channel(bincid, sectionCount, blockCount, blocksize, gusvr.getSessionTimeout());
           gusvr.addChannel(ch);
           gusvr.UpdateHashToSP();
       }

       MyConnector* pCo = this->getConnector(channelid);
       if(pCo == NULL){
	   this->addConnector(channelid, static_cast<MyConnector*>(client));          
       }

    }*/
    return 0;
}

int MyHandler::regUser(ServerClient* client, httpRequest* httpReq)
{
    PARAMSKEYVALUES* pParamKeyValue = httpReq->getParamKeyValue();

    std::string strName;
    std::string strSex;

    PARAMSKEYVALUES::iterator itParam= pParamKeyValue->find("name");
    if(itParam != pParamKeyValue->end()){
        strName = itParam->second;
    }
    itParam= pParamKeyValue->find("sex");
    if(itParam != pParamKeyValue->end()){
        strSex = itParam->second;
    }

    printf("register ok, name=%s,sex=%s\n", strName.c_str(),strSex.c_str());

    char* retBuf=NULL;
    int retBufLen = 0;
	if(packSuccessRes(retBuf, retBufLen, "req=register&state=success", str_util::strlen("req=register&state=success")) > 0)
    {
        client->send_msg(retBuf, retBufLen);
        free(retBuf);
        client->close();  
        return 0;
    }
    else
    {
        return -1;
    }
}
