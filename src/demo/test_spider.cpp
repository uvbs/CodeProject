#include "socketdef.h"
#include "test_spider.h"
#include "macrodef.h"
#include "assert.h"
#include "string_util.h"
#include "log.h"
#include "http_client.h"
#include "json.h"
#include "test_tinyxml.h"
#include "ini_reader.h"
#include "variable_system.h"

WX_IMPLEMENT_SINGLEON(DBInterface);
bool DBInterface::init(char* ip, uint port, char* user, char* pwd, char* dbname, char* encoding)
{
    __ENTER_FUNCTION
    if(!IDBInterface::init(ip,port,user,pwd,dbname,encoding))
        return false;
    _persons.reserve(150000);
    return true;
    __LEAVE_FUNCTION
    return false;
}

bool DBInterface::loadPersonData(const char* sql)
{
    __ENTER_FUNCTION
    enum
    {
        DB_PERSON_ID = 0,
        DB_NAME_CHS,
        DB_NAME_ENG,
        DB_NAME_CHS_OTHER,
        DB_NAME_ENG_OTHER,
        DB_ITEM_ID,
        DB_SUBLEMMA_ID,
        DB_NEW_SUBLEMMA_ID,
    };

    CDataBase* pDataBase = getDataBase();
    Assert(pDataBase);
    // 读表
    char SQLString[MAX_SQL_LEN] = {0};
    str_util::snprintf(SQLString,MAX_SQL_LEN,sql,PERSON_TABLE);
    bool bResult = pDataBase->execute(SQLString,str_util::strlen(SQLString));
    Assert(bResult);
    CDBResult* pResult = pDataBase->getResult();
    Assert(pResult);

    int count = 0;
    _persons.clear();
    while(pResult->next())
    {
        stPersonDetail person;
        person._id = pResult->getFieldAsInt(DB_PERSON_ID);
        const char* name_chs = pResult->getFieldAsString(DB_NAME_CHS);
        if (name_chs)
            person._name_chs = name_chs;
        const char* name_eng = pResult->getFieldAsString(DB_NAME_ENG);
        if (name_eng)
            person._name_eng = name_eng;
        const char* name_chs_other = pResult->getFieldAsString(DB_NAME_CHS_OTHER);
        if (name_chs_other)
            person._name_chs_other = name_chs_other;
        const char* name_eng_other = pResult->getFieldAsString(DB_NAME_ENG_OTHER);
        if (name_eng_other)
            person._name_eng_other = name_eng_other;
        person._itemId = pResult->getFieldAsInt(DB_ITEM_ID);
        person._subLemmaId = pResult->getFieldAsInt(DB_SUBLEMMA_ID);
        person._newSubLemmaId = pResult->getFieldAsInt(DB_NEW_SUBLEMMA_ID);
        _persons.push_back(person);
        count++;
        //printf("personid=%d, count=%d\n", person.person_id, count);
    }

    LogSystem::getSinglePtr()->cacheLog(LOG_FILE_DEBUG, "load person ok , count=%d", count);

    return true;
    __LEAVE_FUNCTION
    return false;
}

bool DBInterface::savePersonData(QTYPE qtype, std::list<stPersonResult>& _dataQ)
{
    //char tmpString[1024*512] = {0};
    char sqlString[1024*4] = {0};
    bool firstFlag = true;
    CDataBase* pDataBase = getDataBase();
    Assert(pDataBase);
    std::list<stPersonResult>::iterator iter=_dataQ.begin();
    bool ret = false;
    for (; iter != _dataQ.end(); ++iter)
    {
        if (qtype == BK_Q)
        {
            str_util::snprintf(sqlString,sizeof(sqlString),UPDATE_PERSON_BK_SQL, PERSON_TABLE,
                iter->_bk_itemid, iter->_bk_subLemmaId, iter->_bk_new_subLemmaId, iter->_id);
        }
        else if (qtype == TP_Q)
        {
            str_util::snprintf(sqlString,sizeof(sqlString),UPDATE_PERSON_TP_SQL, PERSON_TABLE,
                iter->_tp_json.c_str(), iter->_id); 
            //discard sql
            if (str_util::strlen(sqlString) >= 1024*3)
            {
                LogSystem::getSinglePtr()->cacheLog(LOG_FILE_FATAL,
					"Save json fail,person_id=%d", iter->_id);
                continue;
            }    
        }
        //str_util::Strncat(tmpString,sqlString,str_util::Strlen(sqlString));
	    ret = pDataBase->execute(sqlString, str_util::strlen(sqlString));
    }
    return ret;
}

bool DBInterface::saveRankData(vector<Person>& data)
{
    char tmpString[1024*8] = {0};
    char sqlString[1024*2] = {0};

    CDataBase* pDataBase = getDataBase();
    Assert(pDataBase);

    str_util::snprintf(tmpString,sizeof(tmpString),DEL_PERSON_RANK_SQL, RANK_TABLE);
    bool ret = pDataBase->execute(tmpString, str_util::strlen(tmpString));
    if (!ret)
    {
        return false;
    }
    memset(tmpString,0,sizeof(tmpString));
    str_util::snprintf(tmpString,sizeof(tmpString),INSERT_PERSON_RANK_SQL, RANK_TABLE);
    for (int i=0; i<data.size(); ++i)
    {
        str_util::snprintf(sqlString,sizeof(sqlString), "(%d, \'%s\', %d, \'%s\' ),", 
            data[i].rank, data[i].name.c_str(), data[i].searchCnt, data[i].image.c_str());
        str_util::strcat(tmpString, sqlString);
    }
    tmpString[str_util::strlen(tmpString)-1] = 0;
    ret = pDataBase->execute(tmpString, str_util::strlen(tmpString));
    return ret;

}
////////////////////////////////////////////////////////////////////////////////////
WX_IMPLEMENT_SINGLEON(SpiderMgr);
bool SpiderMgr::init(int app_Id, const char* api_key, const char* secret_key)
{
    _app_Id = app_Id;
    _api_key = api_key;
    _secret_key = secret_key;
    _running = true;
    curl_global_init(CURL_GLOBAL_ALL);

    //////////////////////////////////////////////////////////////////
    //read config
    IniReader file("./config/spider.ini");
    char value[64] = {0};
    //////////////////////////////////////////////////////////////////
    //read BK config
    //dbname
    file.readString("BK","dbname", value, sizeof(value));
    CVariableSystem::getSinglePtr()->setStringValue("bk_dbname", value);
    //ip
    file.readString("BK","ip",value,sizeof(value));
    CVariableSystem::getSinglePtr()->setStringValue("bk_ip", value);
    //port
    int port = file.readInt("BK","port");
    CVariableSystem::getSinglePtr()->setIntValue("bk_port", port);
    //user
    file.readString("BK","user",value,sizeof(value));
    CVariableSystem::getSinglePtr()->setStringValue("bk_user", value);
    //pwd
    file.readString("BK","pwd",value,sizeof(value));
    CVariableSystem::getSinglePtr()->setStringValue("bk_pwd", value);
    //encoding
    file.readString("BK","encoding",value,sizeof(value));
    CVariableSystem::getSinglePtr()->setStringValue("bk_encoding", value);
    //////////////////////////////////////////////////////////////////
    //read RANK config
    //dbname
    file.readString("RANK","dbname", value, sizeof(value));
    CVariableSystem::getSinglePtr()->setStringValue("rank_dbname", value);
    //ip
    file.readString("RANK","ip",value,sizeof(value));
    CVariableSystem::getSinglePtr()->setStringValue("rank_ip", value);
    //port
    port = file.readInt("RANK","port");
    CVariableSystem::getSinglePtr()->setIntValue("rank_port", port);
    //user
    file.readString("RANK","user",value,sizeof(value));
    CVariableSystem::getSinglePtr()->setStringValue("rank_user", value);
    //pwd
    file.readString("RANK","pwd",value,sizeof(value));
    CVariableSystem::getSinglePtr()->setStringValue("rank_pwd", value);
    //encoding
    file.readString("RANK","encoding",value,sizeof(value));
    CVariableSystem::getSinglePtr()->setStringValue("rank_encoding", value);
    //rank file
    file.readString("RANK","file",value,sizeof(value));
    CVariableSystem::getSinglePtr()->setStringValue("rank_file", value);
    
    return true;
}

bool SpiderMgr::release()
{
    curl_global_cleanup();
    return true;
}

Spider* SpiderMgr::getSpider(int index)
{
    if (index<0 || index>=SPIDER_THREAD_NUM)
    {
        return NULL;
    }
    return &_spider[index];
}

bool SpiderMgr::getBKToken()
{
    //https://openapi.baidu.com/oauth/2.0/token?grant_type=client_credentials&client_id={ API Key}&client_secret={ Secret Key}
    const char* token_url= "https://openapi.baidu.com/oauth/2.0/token?grant_type=client_credentials&client_id=%s&client_secret=%s";
    char resp_buf[MAX_BUF_LEN] = {0};
    //request for tokens
    if(_requestToken(token_url, resp_buf, MAX_BUF_LEN) <0) {
        return false;
    }
    json_object* root_obj = json_tokener_parse(resp_buf);
    if(is_error(root_obj) || root_obj == NULL) {
        return false;
    }
    if(!json_object_is_type(root_obj, json_type_object)){
        json_object_put(root_obj);
        return false;
    }
    struct json_object* sub_obj_access_token  = json_object_object_get(root_obj, "access_token");
    struct json_object* sub_obj_session_key  = json_object_object_get(root_obj, "session_key");
    struct json_object* sub_obj_scope  = json_object_object_get(root_obj, "scope");
    struct json_object* sub_obj_refresh_token  = json_object_object_get(root_obj, "refresh_token");
    struct json_object* sub_obj_session_secret  = json_object_object_get(root_obj, "session_secret");
    struct json_object* sub_obj_expires_in  = json_object_object_get(root_obj, "expires_in");
    //validate
    if(NULL == sub_obj_access_token || is_error(sub_obj_access_token)){
        json_object_put(root_obj);
        return false;
    }
    if(!json_object_is_type(sub_obj_access_token, json_type_string)){
        json_object_put(root_obj);
        return false;
    }
    //validate
    if(NULL == sub_obj_refresh_token || is_error(sub_obj_refresh_token)){
        json_object_put(root_obj);
        return false;
    }
    if(!json_object_is_type(sub_obj_refresh_token, json_type_string)){
        json_object_put(root_obj);
        return false;
    }
    //get strings
    const char* access_token = json_object_get_string(sub_obj_access_token);
    const char* refresh_token = json_object_get_string(sub_obj_refresh_token);
    if (!access_token || !refresh_token) {
        return false;
    }
    _access_token = access_token;
    _refresh_token = refresh_token;
    return true;
}

int SpiderMgr::_requestToken(const char* token_url, char* resp_buf, int resp_buf_len)
{
    char req_buf[1024] = {0};
    //request and response buffer
    if(_h.init(resp_buf_len,5,5) < 0)
        return false;
    str_util::snprintf(req_buf,sizeof(req_buf),token_url,_api_key.c_str(),_secret_key.c_str());
    int ret = _h.request(req_buf,NULL,0);
    if(ret !=0)
        return -1;

    size_t ret_len = 0;
    uint code = 0;
    char* res = _h.getResponse(&code,&ret_len);
    if(code != 200 )
        return -1;

    memcpy(resp_buf,res,ret_len);
    if(ret_len <= resp_buf_len){
        resp_buf[ret_len] = '\0';
        return ret_len;
    }
    else {
        resp_buf[resp_buf_len-1] = '\0';
        return resp_buf_len;
    }
}

void SpiderMgr::startBK()
{
    for (int i=0; i<SPIDER_THREAD_NUM; ++i)
    {
        _spider[i].start(i, &Spider::dwloadBK);
        //_spider[i].start(i, &Spider::dwloadBKMultiPerson);
    }
    _param._spiderMgr = this;
    _param._func = &SpiderMgr::updateBK;
    _updata_thr.start(SpiderMgr::run_update, &_param);
}

void SpiderMgr::startTP()
{
    for (int i=0; i<SPIDER_THREAD_NUM; ++i)
    {
        _spider[i].start(i, &Spider::dwloadTP);
    }
    _param._spiderMgr = this;
    _param._func = &SpiderMgr::updateTP;
    _updata_thr.start(SpiderMgr::run_update, &_param);
}

void SpiderMgr::waitBK()
{
    for (int i=0; i<SPIDER_THREAD_NUM; ++i)
    {
        _spider[i].wait();
    }
    _running = false;
    _updata_thr.wait();
    flushPersonQ(BK_Q);
}

void SpiderMgr::waitTP()
{
    for (int i=0; i<SPIDER_THREAD_NUM; ++i)
    {
        _spider[i].wait();
    }
    _running = false;
    _updata_thr.wait();
    flushPersonQ(TP_Q);
}

void* SpiderMgr::run_update(void* param)
{
    stThrParam* p = (stThrParam*)param;
    if(p && p->_func && p->_spiderMgr)
    {
        //evoke function use obj
        (p->_spiderMgr->*(p->_func))();
    }
    return NULL;
}

bool SpiderMgr::addToPersonQ(std::list<stPersonResult>& result)
{
    AutoMLock lock(_lockQ);
    _dataQ.insert(_dataQ.end(),result.begin(),result.end());
    return true;
}

void SpiderMgr::flushPersonQ(QTYPE qtype)
{
    AutoMLock lock(_lockQ);
    if(_dataQ.size() <= 0) 
        return;
    bool ret = DBInterface::getSinglePtr()->savePersonData(qtype, _dataQ);
    if(ret)
    {
        _dataQ.clear();
    }
}

void SpiderMgr::updateBK()
{
    while(_running)
    {
        flushPersonQ(BK_Q);
    }
}

void SpiderMgr::updateTP()
{
    while(_running)
    {
        flushPersonQ(TP_Q);
    }
}

///////////////////////////////////////////////////////////////////////////////
bool Spider::init()
{
    _buffer[0] = 0;
    return true;
}

bool Spider::getBKIdMulti(const char* bk_key, vector<int>& list, vector<int>& newlist)
{
    const char* card_url="https://openapi.baidu.com/rest/2.0/baike/openapi/BaikeLemmaInfoApi?access_token=%s&format=json&appid=%d&bk_key=%s&bk_length=%d";
    _buffer[0] = 0;
    list.clear();

    if(_requestBKId(card_url,bk_key,400, _buffer, MAX_BUF_LEN) < 0) {
        return false;
    }
    json_object* personList = json_tokener_parse(_buffer);
    if(is_error(personList) || personList == NULL){
        return false;
    }
    if(!json_object_is_type(personList, json_type_array)){
        json_object_put(personList);
        return false;
    }
    int personCnt = json_object_array_length(personList);
    for (int i=0; i<personCnt; ++i)
    {
        struct json_object* person = json_object_array_get_idx(personList, i);
        if (!person || is_error(person))
            continue;
        struct json_object* sublemmaId  = json_object_object_get(person, "sublemmaId");
        if (!sublemmaId || is_error(sublemmaId))
            continue;
        struct json_object* newlemmaId  = json_object_object_get(person, "newlemmaId");
        if (!newlemmaId || is_error(newlemmaId))
            continue;
        if(!json_object_is_type(sublemmaId, json_type_int))
            continue;
        if(!json_object_is_type(newlemmaId, json_type_int))
            continue;
        int id = json_object_get_int(sublemmaId);
        int newid = json_object_get_int(newlemmaId);
        list.push_back(id);
        newlist.push_back(newid);
    }
    
    return true;
}

bool Spider::getBKId(const char* bk_key, int& id, int& subLemmaId, int& newSubLemmaId)
{
    const char* card_url="https://openapi.baidu.com/rest/2.0/baike/openapi/BaikeLemmaCardApi?access_token=%s&format=json&appid=%d&bk_key=%s&bk_length=%d";
    _buffer[0] = 0;
    if(_requestBKId(card_url,bk_key,400, _buffer, MAX_BUF_LEN) < 0) {
        return false;
    }
    json_object* person_obj = json_tokener_parse(_buffer);
    if(is_error(person_obj) || person_obj == NULL){
        return false;
    }
    if(!json_object_is_type(person_obj, json_type_object)){
        json_object_put(person_obj);
        return false;
    }
    struct json_object* sub_obj_id  = json_object_object_get(person_obj, "id");
    struct json_object* sub_obj_subLemmaId  = json_object_object_get(person_obj, "subLemmaId");
    struct json_object* sub_obj_new_subLemmaId  = json_object_object_get(person_obj, "newLemmaId");
    //validate
    if(NULL == sub_obj_id || is_error(sub_obj_id)){
        json_object_put(person_obj);
        return false;
    }
    if(!json_object_is_type(sub_obj_id, json_type_int)){
        json_object_put(person_obj);
        return false;
    }
    //validate
    if(NULL == sub_obj_subLemmaId || is_error(sub_obj_subLemmaId)){
        json_object_put(person_obj);
        return false;
    }
    if(!json_object_is_type(sub_obj_subLemmaId, json_type_int)){
        json_object_put(person_obj);
        return false;
    }
    //validate
    if(NULL == sub_obj_new_subLemmaId || is_error(sub_obj_new_subLemmaId)){
        json_object_put(person_obj);
        return false;
    }
    if(!json_object_is_type(sub_obj_new_subLemmaId, json_type_int)){
        json_object_put(person_obj);
        return false;
    }
    //get integers
    id = json_object_get_int(sub_obj_id);
    subLemmaId = json_object_get_int(sub_obj_subLemmaId);
    newSubLemmaId = json_object_get_int(sub_obj_new_subLemmaId);
    return true;
}

bool Spider::getTP(int id, string& from, vector<stTPJsonItem>& relations)
{
    const char* relation_url = "http://kgapi-bj.baidu.com/1";
    _buffer[0] = 0;
    relations.clear();
    if(_requestTP(relation_url, id, _buffer, MAX_BUF_LEN) < 0) {
        return false;
    }
    //parse
    json_object* resp = json_tokener_parse(_buffer);
    if(is_error(resp) || resp == NULL){
        printf ("format error !\n");
    }
    //get result json
    struct json_object* result  = json_object_object_get(resp, "result");
    if(NULL == result || is_error(result)){
        json_object_put(resp);
        return false;
    }
    struct json_object* retString  = json_object_object_get(result, "_ret");
    if(NULL == retString || is_error(retString)){
        json_object_put(result);
        return false;
    }
    //care!!
    if(!json_object_is_type(retString, json_type_string)){
        json_object_put(result);
        return false;
    }
    const char* retStringVal = json_object_get_string(retString);
    json_object* retObjs = json_tokener_parse(retStringVal);
    if(NULL == retObjs || is_error(retObjs)){
        json_object_put(retString);
        return false;
    }
    if(!json_object_is_type(retObjs, json_type_array)){
        json_object_put(retString);
        return false;
    }
    //for each obj
    int objCnt = json_object_array_length(retObjs); 
    for (int i=0; i<objCnt; ++i)
    {
        stTPJsonItem child;
        json_object* obj = json_object_array_get_idx(retObjs, i);
        if(NULL == obj || is_error(obj)){
            continue;
        }
        /////////////////////////////////////////////////////////
        //get fromId
        struct json_object* fromIds  = json_object_object_get(obj, "fromId");
        if(NULL == fromIds || is_error(fromIds)){
            json_object_put(obj);
            continue;
        }
        if(!json_object_is_type(fromIds, json_type_array)){
            json_object_put(obj);
            continue;
        }
        json_object* fromId = json_object_array_get_idx(fromIds, 0);
        if(!json_object_is_type(fromId, json_type_string)){
            json_object_put(obj);
            return false;
        }
        const char* fromIdVal = json_object_get_string(fromId);
        printf("fromId=%d\n", str_util::strToInt(fromIdVal));
        if (id != str_util::strToInt(fromIdVal))
        {
            continue;
        }
        /////////////////////////////////////////////////////////
        //get fromName
        struct json_object* fromNames  = json_object_object_get(obj, "fromName");
        if(NULL == fromNames || is_error(fromNames)){
            json_object_put(obj);
            continue;
        }
        if(!json_object_is_type(fromNames, json_type_array)){
            json_object_put(obj);
            continue;
        }
        json_object* fromName= json_object_array_get_idx(fromNames, 0);
        if(!json_object_is_type(fromName, json_type_string)){
            json_object_put(obj);
            return false;
        }
        const char* fromNameVal = json_object_get_string(fromName);
        str_util::utf8ToGbk(fromNameVal, from);
        printf("fromName=%s\n", from.c_str());
        //from = fromNameVal;
        /////////////////////////////////////////////////////////
        //get toId, toName, relation
        struct json_object* originalJson  = json_object_object_get(obj, "original_kgjson");
        if(NULL == originalJson || is_error(originalJson)){
            json_object_put(obj);
            continue;
        }
        if(!json_object_is_type(originalJson, json_type_string)){
            json_object_put(obj);
            continue;
        }
        const char* originalJsonVal = json_object_get_string(originalJson);
        json_object* jsonObj = json_tokener_parse(originalJsonVal);
        if(NULL == jsonObj || is_error(jsonObj)){
            json_object_put(obj);
            continue;
        }
        struct json_object* attrs  = json_object_object_get(jsonObj, "AVP_LIST");
        if(NULL == attrs || is_error(attrs)){
            json_object_put(obj);
            continue;
        }
        //for each obj
        int attrCnt = json_object_array_length(attrs);
        for (int j=0; j<attrCnt; ++j)
        {
            json_object* attr = json_object_array_get_idx(attrs, j);
            if(NULL == attr || is_error(attr)){
                json_object_put(attrs);
                continue;
            }
            struct json_object* attrName  = json_object_object_get(attr, "ATTRIBUTE");
            if(NULL == attrName || is_error(attrName)){
                json_object_put(attrs);
                continue;
            }
            if(!json_object_is_type(attrName, json_type_string)){
                json_object_put(attrs);
                continue;
            }
            const char* attrNameVal = json_object_get_string(attrName);
            //get relation
            if (str_util::strncmp(attrNameVal,"name", str_util::strlen(attrNameVal))==0)
            {
                struct json_object* attrValue  = json_object_object_get(attr, "VALUE");
                if(NULL == attrValue || is_error(attrValue)){
                    json_object_put(attrs);
                    continue;
                }
                const char* attrValueVal = json_object_get_string(attrValue);
                str_util::utf8ToGbk(attrValueVal,child.relation);
                printf("relation=%s\n", child.relation.c_str());
                //child.relation = attrValueVal;
            }
            //get toId
            else if (str_util::strncmp(attrNameVal,"toId", str_util::strlen(attrNameVal))==0)
            {
                struct json_object* attrValue  = json_object_object_get(attr, "VALUE");
                if(NULL == attrValue || is_error(attrValue)){
                    json_object_put(attrs);
                    continue;
                }
                const char* attrValueVal = json_object_get_string(attrValue);
                printf("toId=%s\n", attrValueVal);
                child.toId = str_util::strToInt(attrValueVal);
            }
            //get toName
            else if (str_util::strncmp(attrNameVal,"toName", str_util::strlen(attrNameVal))==0)
            {
                struct json_object* attrValue  = json_object_object_get(attr, "VALUE");
                if(NULL == attrValue || is_error(attrValue)){
                    json_object_put(attrs);
                    continue;
                }
                const char* attrValueVal = json_object_get_string(attrValue);
                //child.toName = attrValueVal;
                str_util::utf8ToGbk(attrValueVal,child.toName);
                printf("toName=%s\n", child.toName.c_str());
            }
        }
        relations.push_back(child);
    }
    return true;
}

int Spider::_requestBKId(const char* card_url, const char* bk_key, int bk_len, char* resp_buf, int resp_buf_len)
{
    char req_buf[1024] = {0};
    str_util::snprintf(req_buf, sizeof(req_buf), card_url, SpiderMgr::getSinglePtr()->_access_token.c_str(),
        SpiderMgr::getSinglePtr()->_app_Id, bk_key, bk_len);
    if(_h.init(resp_buf_len,5,5) < 0)
        return -1;
    int ret = _h.request(req_buf,NULL,0);
    if(ret !=0)
        return -1;
    
    size_t ret_len = 0;
    uint code = 0;
    char* res = _h.getResponse(&code,&ret_len);
    if(code != 200 )
        return -1;

    memcpy(resp_buf, res, ret_len);
    if(ret_len <= resp_buf_len){
        resp_buf[ret_len] = '\0';
        return ret_len;
    }
    else {
        resp_buf[resp_buf_len-1] = '\0';
        return resp_buf_len;
    }
}

int Spider::_requestTP(const char* relation_url, int id, char* resp_buf, int resp_buf_len)
{
    char req_buf[512] = {0};
    str_util::snprintf(req_buf, sizeof(req_buf), relation_url);
    char param_buf[512] = {0};
    str_util::snprintf(param_buf, sizeof(param_buf), "g.has('fromId',MATCH,'%d').limit(8).with('*')", id);

    json_object *request = json_object_new_object();
    if (request)
    {
        json_object_object_add(request,"method", json_object_new_string("GrammarQueryService"));
        //json_object_object_add(root,"method", json_object_new_string("CommonQueryService"));
        json_object* params = json_object_new_array();
        if (params)
        {
            json_object *param0 = json_object_new_object();
            if (param0)
            {
                json_object_object_add(param0, "tn", json_object_new_string("chenzhifeng02"));
                json_object_object_add(param0, "token", json_object_new_string("07776537880003870720"));
                json_object_object_add(param0, "resource_name", json_object_new_string("person-relation"));
                json_object_object_add(param0, "query", json_object_new_string(param_buf));
                /*json_object_object_add(param0, "query", json_object_new_string("g.has('fromId',MATCH,'2848601').limit(1).with('*')"));
                json_object_object_add(param0, "action", json_object_new_string("getentities"));
                json_object_object_add(param0, "query_type", json_object_new_int(1));*/
                json_object_array_add(params,param0);
            }
            json_object_object_add(request, "params", params);
        }
    }
    const char* jsonstr = json_object_to_json_string(request);
    //cout << jsonstr << endl;
    char postdata[512] = {0};
    memcpy(postdata, jsonstr, str_util::strlen(jsonstr));

    if(_h.init(resp_buf_len,5,5) < 0)
        return -1;

   int ret = _h.request(req_buf, postdata, str_util::strlen(postdata));
   if(ret !=0)
        return -1;

    size_t ret_len = 0;
    uint code = 0;
    char* res = _h.getResponse(&code,&ret_len);
    if(code != 200 )
        return -1;
    memcpy(resp_buf, res, ret_len);
    if(ret_len <= resp_buf_len){
        resp_buf[ret_len] = '\0';
        return  ret_len;
    }
    else {
        resp_buf[resp_buf_len-1] = '\0';
        return resp_buf_len;
    }
}

void Spider::start(int index, CbFunc f)
{
    _param._spider = this;
    _param._threadIndex = index;
    _param._func = f;
    _dwload_thr.start(&Spider::run_dwload, &_param);
}

void Spider::wait()
{
    _dwload_thr.wait();
}

void* Spider::run_dwload(void* param)
{
    stThrParam* p = (stThrParam*)param;
    if(p && p->_func && p->_spider)
    {
        //evoke function use obj
        (p->_spider->*(p->_func))(p->_threadIndex);
    }
    return NULL;
}

void Spider::dwloadBKMultiPerson(int index)
{
    if(!_persons)
        return;

    int count = _persons->size();
    int step = count / SPIDER_THREAD_NUM;
    int start = index * step;
    int end = (index + 1) * step;
    if (index == SPIDER_THREAD_NUM-1)
        end = count;

    char buff[3072] = {0};
    for (int i = start; i < end; ++i)
    {
        vector<int> list, newlist;
        if(!getBKIdMulti((*_persons)[i]._name_chs.c_str(), list, newlist))
            continue;
        if (list.size() <= 1)
            continue;
        if (list.size() != newlist.size())
            continue;
       
        memset(buff,0,sizeof(buff));
        str_util::snprintf(buff, sizeof(buff), "videoId=%d, name=%s, bkId=", 
               (*_persons)[i]._id, (*_persons)[i]._name_chs.c_str());

        char temp[512] = {0};
        for (int j=0; j<list.size(); ++j)
        {
            str_util::snprintf(temp,sizeof(temp), "(%d,%d),", list[j], newlist[j]);
            str_util::strcat(buff, temp);
        }
        LogSystem::getSinglePtr()->cacheLog(LOG_FILE_DEBUG, buff);
        if (i % 20 == 0)
        {
            LogSystem::getSinglePtr()->flushLog(LOG_FILE_DEBUG);
        }
    }
    LogSystem::getSinglePtr()->flushLog(LOG_FILE_DEBUG);
}

void Spider::dwloadBK(int index)
{
    if(!_persons)
        return;

    int count = _persons->size();
    int step = count / SPIDER_THREAD_NUM;
    int start = index * step;
    int end = (index + 1) * step;
    if (index == SPIDER_THREAD_NUM-1)
        end = count;
    for (int i = start; i < end; ++i)
    {
        int itemId = -1;
        int subLemmaId = -1;
        int newSubLemmaId = -1;
        if(!getBKId((*_persons)[i]._name_chs.c_str(), itemId, subLemmaId, newSubLemmaId))
        {
            LogSystem::getSinglePtr()->cacheLog(LOG_FILE_FATAL,
				"get bk_id error,bk_key=%s", (*_persons)[i]._name_chs.c_str());
            continue;
        }
        stPersonResult result;
        result._id = (*_persons)[i]._id;
        result._bk_itemid = itemId;
        result._bk_subLemmaId = subLemmaId;
        result._bk_new_subLemmaId = newSubLemmaId;
        _personQ.push_back(result);
        if (_personQ.size() % 100 == 0)
        {
            if(SpiderMgr::getSinglePtr()->addToPersonQ(_personQ))
                _personQ.clear();
        }
    }

    //copy left items to list
    if (_personQ.size()>0)
    {
        if(SpiderMgr::getSinglePtr()->addToPersonQ(_personQ))
            _personQ.clear();
    }
}

void Spider::dwloadTP(int index)
{
    if(!_persons)
        return;

    int count = _persons->size();
    int step = count / SPIDER_THREAD_NUM;
    int start = index * step;
    int end = (index + 1) * step;
    if (index == SPIDER_THREAD_NUM-1)
        end = count;
    for (int i = start; i < end; ++i)
    {
        //get persons
        string fromName;
        vector<stTPJsonItem> relations;
        if(!getTP((*_persons)[i]._newSubLemmaId, fromName, relations))
        {
            LogSystem::getSinglePtr()->cacheLog(LOG_FILE_FATAL,
				"get tupu error,bk_id=%d", (*_persons)[i]._newSubLemmaId);
            continue;
        }
        //generate json
        string json;
        if(genJson((*_persons)[i]._newSubLemmaId, fromName, relations, json))
        {
            stPersonResult result;
            result._id = (*_persons)[i]._id;
            result._tp_json = json;
            _personQ.push_back(result);
        }
        //add
        if (_personQ.size() % 100 == 0)
        {
            if(SpiderMgr::getSinglePtr()->addToPersonQ(_personQ))
                _personQ.clear();
        }
    }
    //copy left items to list
    if (_personQ.size()>0)
    {
        if(SpiderMgr::getSinglePtr()->addToPersonQ(_personQ))
            _personQ.clear();
    }
}

int Spider::getVideoId(int fromId)
{
    if (!_persons)
        return -1;
    vector<stPersonDetail>::iterator iter=_persons->begin();
    for (; iter != _persons->end(); ++iter)
    {
        if(iter->_newSubLemmaId == fromId)
            return iter->_id;
    }
    return -1;
}

bool Spider::genJson(int fromId, string& fromName, vector<stTPJsonItem>& relations, string& json)
{
    json_object *root = json_object_new_object();
    if (root)
    {
        json_object_object_add(root,"fromId", json_object_new_int(getVideoId(fromId)));
        json_object_object_add(root,"fromName", json_object_new_string(fromName.c_str()));
        json_object* args = json_object_new_array();
        if (args)
        {
            vector<stTPJsonItem>::iterator iter=relations.begin();
            for (; iter != relations.end(); ++iter)
            {
                json_object *arg0 = json_object_new_object();
                if (arg0)
                {
                    json_object_object_add(arg0, "toId", json_object_new_int(getVideoId(iter->toId)));
                    json_object_object_add(arg0, "toName", json_object_new_string(iter->toName.c_str()));
                    json_object_object_add(arg0, "relation", json_object_new_string(iter->relation.c_str()));
                    json_object_array_add(args,arg0);
                }
            }
            json_object_object_add(root, "relations", args);
        }
        //
        const char* jsonstr = json_object_to_json_string(root);
        json = jsonstr;
        //todo: 转成gbk
        return true;//str_util::Utf8ToGbk(jsonstr, json);
    }
    return false;
}

#if defined _LINUX64

#define USE_OPENSSL
#include <stdio.h>  
#include <pthread.h>  
#include <curl/curl.h>  
#define NUMT 4  

#endif

#if defined USE_OPENSSL && defined _LINUX64

/* we have this global to let the callback get easy access to it */   
static pthread_mutex_t *lockarray;  

#include <openssl/crypto.h>  
static void lock_callback(int mode, int type, const char *file, int line)  
{
    (void)file;  
    (void)line;  
    if (mode & CRYPTO_LOCK) {  
        pthread_mutex_lock(&(lockarray[type]));  
    }  
    else {  
        pthread_mutex_unlock(&(lockarray[type]));  
    }  
}  

static unsigned long thread_id(void)  
{
    unsigned long ret;
    ret=(unsigned long)pthread_self();  
    return(ret);  
}  

static void init_locks(void)  
{
    int i;
    lockarray=(pthread_mutex_t *)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));  
    for (i=0; i<CRYPTO_num_locks(); i++) {  
        pthread_mutex_init(&(lockarray[i]),NULL);  
    }  

    CRYPTO_set_id_callback((unsigned long (*)())thread_id);  
    CRYPTO_set_locking_callback(lock_callback);  
}  

static void kill_locks(void)  
{
    int i;
    CRYPTO_set_locking_callback(NULL);  
    for (i=0; i<CRYPTO_num_locks(); i++)
        pthread_mutex_destroy(&(lockarray[i]));
    OPENSSL_free(lockarray);  
} 

#endif  

bool test_bk()
{
    SpiderMgr* pSpiderMgr = SpiderMgr::getSinglePtr();
    Assert(pSpiderMgr);
    bool ret = pSpiderMgr->getBKToken();
    if(!ret)
    {
        LogSystem::getSinglePtr()->saveLog(LOG_FILE_ERROR,"get token error.");
        return false;
    }

    //connect db
    string ip = CVariableSystem::getSinglePtr()->getStringValue("bk_ip");
    int port = CVariableSystem::getSinglePtr()->getIntValue("bk_port");
    string user = CVariableSystem::getSinglePtr()->getStringValue("bk_user");
    string pwd = CVariableSystem::getSinglePtr()->getStringValue("bk_pwd");
    string db = CVariableSystem::getSinglePtr()->getStringValue("bk_dbname");
    string encoding = CVariableSystem::getSinglePtr()->getStringValue("bk_encoding");
    ret = DBInterface::getSinglePtr()->init(const_cast<char*>(ip.c_str()), port, 
        const_cast<char*>(user.c_str()), const_cast<char*>(pwd.c_str()),
        const_cast<char*>(db.c_str()), const_cast<char*>(encoding.c_str()));
    if (!ret)
    {
        LogSystem::getSinglePtr()->saveLog(LOG_FILE_ERROR,"init DBinterface fail.");
        return false;
    }

    //load person where bkId < 0
    if(!DBInterface::getSinglePtr()->loadPersonData(LOAD_PERSON_BK_SQL))
    {
        LogSystem::getSinglePtr()->saveLog(LOG_FILE_ERROR,"get persons from db error.");
        return false;
    }
    std::vector<stPersonDetail>* persons = DBInterface::getSinglePtr()->getPersonData();
    if(!persons || persons->size()<0)
    {
        return false;
    }
    //for test
    stPersonDetail test;
    test._id = 6543;
    //test._name_chs = "郑爽";
    str_util::gbkToUtf8("郑爽", test._name_chs);
    persons->clear();
    persons->push_back(test);
    for (int i=0; i<SPIDER_THREAD_NUM; ++i)
    {
        Spider* pSpider = pSpiderMgr->getSpider(i);
        if(pSpider) 
        {
            if(!pSpider->init())
                return false;
            pSpider->setPersonData(persons);
        }
    }
    pSpiderMgr->startBK();
    pSpiderMgr->waitBK();
    LogSystem::getSinglePtr()->saveLog(LOG_FILE_ERROR,"qsize=%d", 
        pSpiderMgr->getSinglePtr()->getPersonQSize());

    return true;
}

bool test_tupu()
{
    SpiderMgr* pSpiderMgr = SpiderMgr::getSinglePtr();
    Assert(pSpiderMgr);
    //load person where bkId > 0
    if(!DBInterface::getSinglePtr()->loadPersonData(LOAD_PERSON_TP_SQL))
    {
        LogSystem::getSinglePtr()->saveLog(LOG_FILE_ERROR,"get persons from db error.");
        return false;
    }
    std::vector<stPersonDetail>* persons = DBInterface::getSinglePtr()->getPersonData();
    if(!persons || persons->size()<0)
    {
        return false;
    }
    /*stPersonDetail test;
    test._id = 157290;
    test._name_chs = "刘在石";
    test._itemId = 867443;
    test._subLemmaId = 867443;
    test._newSubLemmaId = 2848601;
    persons->clear();
    persons->push_back(test);*/
    for (int i=0; i<SPIDER_THREAD_NUM; ++i)
    {
        Spider* pSpider = pSpiderMgr->getSpider(i);
        if(pSpider) 
        {
            if(!pSpider->init())
                return false;
            pSpider->setPersonData(persons);
        }
    }
    pSpiderMgr->startTP();
    pSpiderMgr->waitTP();
    LogSystem::getSinglePtr()->saveLog(LOG_FILE_ERROR,"qsize=%d", 
        pSpiderMgr->getSinglePtr()->getPersonQSize());

    return true;
}

bool getImage(TiXmlNode* node, string& img)
{
    TiXmlNode* n = getSuccessor(node, "img");
    if (n)
    {
        TiXmlElement* e = n->ToElement();
        if (e)
        {
            img = e->Attribute("src");
        }
    }
    return true;
}

bool getPerson(TiXmlNode* node, int& rank, string& personName, int& searchCnt)
{
    bool rankFlag = false;
    bool personFlag = false;
    bool searchFlag = false;
    for (TiXmlNode* child = node->FirstChild(); child; child = child->NextSibling())
    {
        if (!child)
            continue;
        TiXmlElement* e = child->ToElement();
        if (!e) return false;
        TiXmlAttribute* a = e->FirstAttribute();
        if (!a) return false;
        string name = a->Name();
        string value = a->Value();
        if (name=="class" && value=="first")
        {
            TiXmlNode* cn = e->FirstChild();
            if (cn && cn->FirstChild())
            {
                TiXmlText* ct = cn->FirstChild()->ToText();
                if (ct)
                {
                    rank = str_util::strToInt(ct->Value());
                    rankFlag = true;
                }
            }
        }
        else if (name=="class" && value=="keyword")
        {
            //e->Print(stdout, 1);
            TiXmlNode* cn = e->FirstChild();
            if (cn && cn->FirstChild())
            {
                TiXmlText* ct = cn->FirstChild()->ToText();
                if (ct)
                {
                    //str_util::Utf8ToGbk(ct->Value(), personName);
		    personName = ct->Value();
                    personFlag = true;
                }
            }
        }
        else if (name=="class" && value=="last")
        {
            //e->Print(stdout, 1);
            TiXmlNode* cn = e->FirstChild();
            if (cn && cn->FirstChild())
            {
                TiXmlText* ct = cn->FirstChild()->ToText();
                if (ct)
                {
                    searchCnt = str_util::strToInt(ct->Value());
                    searchFlag = true;
                }
            }
        }
    }

    return (rankFlag && personFlag && searchFlag);
}

bool test_rank()
{
    DBInterface::getSinglePtr()->disconnect();

    string ip = CVariableSystem::getSinglePtr()->getStringValue("rank_ip");
    int port = CVariableSystem::getSinglePtr()->getIntValue("rank_port");
    string user = CVariableSystem::getSinglePtr()->getStringValue("rank_user");
    string pwd = CVariableSystem::getSinglePtr()->getStringValue("rank_pwd");
    string db = CVariableSystem::getSinglePtr()->getStringValue("rank_dbname");
    string encoding = CVariableSystem::getSinglePtr()->getStringValue("rank_encoding");
    string rankfile = CVariableSystem::getSinglePtr()->getStringValue("rank_file");

    bool ret = DBInterface::getSinglePtr()->init(const_cast<char*>(ip.c_str()), port, 
        const_cast<char*>(user.c_str()), const_cast<char*>(pwd.c_str()),
        const_cast<char*>(db.c_str()), const_cast<char*>(encoding.c_str()));
    Assert(ret);

    //read rank file
    TiXmlDocument doc(rankfile.c_str());
    bool loadOkay = doc.LoadFile();
    TiXmlNode* node = doc.FirstChild( "table" );
    int suc = 0;
    vector<Person> vec;
    if (node)
    {
        TiXmlNode* child = 0;
        for (child = node->FirstChild(); child; child = child->NextSibling())
        {
            //get child
            if (!child)
                continue;
            //if child is person, then extract person info
            int rank = 0;
            string persoName;
            int searchCnt = 0;
            string image;
            if(!getPerson(child, rank, persoName, searchCnt))
                continue;
            suc ++;
            if (suc <= 1)
            {
                getImage(child->NextSibling(), image);
            }
            //save person
            Person p;
            p.name = persoName;
            p.rank = rank;
            p.searchCnt = searchCnt;
            p.image = image;
            vec.push_back(p);
            //
            if (suc >= 10)
            {
                break;
            }
        }
        bool ret = DBInterface::getSinglePtr()->saveRankData(vec);
        return 0;
    }
    return -1;
}

bool test_spider()
{
#if defined USE_OPENSSL && defined _LINUX64
    //init locks
    init_locks();
#endif

    //initialize
    SpiderMgr* pSpiderMgr = SpiderMgr::getSinglePtr();
    Assert(pSpiderMgr);
    bool ret = pSpiderMgr->init(5978657, //app_id
        "RiYiObM8SlO93BzwszBaWhOt", //api_key
        "FYgEHzIEljDaOLlPVNYQT6DZqD1fZEb2"); //secret_key
    if(!ret)
    {
        LogSystem::getSinglePtr()->saveLog(LOG_FILE_ERROR,"SpiderMgr init error.");
        return false;
    }
    
    //get bk 
    test_bk();

    //get tupu
    //test_tupu();

    //get rank
    //test_rank();

    pSpiderMgr->release();
    delete pSpiderMgr;

#if defined USE_OPENSSL && defined _LINUX64
    //kill locks
    kill_locks();
#endif

    return true;
}
