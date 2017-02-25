////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_spider.h
// @author: by Mr.Chen
// @date: 2015/5/29	14:16
// @brief: 抓取百科、图谱、图片等数据并入库
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef _TEST_SPIDER_H
#define _TEST_SPIDER_H

#include "type.h"
#include "test_mysql.h"
#include "http_client.h"
#include "thread.h"
#include <list>
#include <map>
using std::list;

#define PERSON_TABLE "person_detail"
//BK
#define	LOAD_PERSON_BK_SQL "select person_id,name_chs,name_eng,name_chs_other,name_eng_other,\
                                                    item_id,sublemma_id,new_sublemma_id \
                                                    from %s \
                                                    where item_id < 0 and state=1;"
#define UPDATE_PERSON_BK_SQL "update %s \
                                                        set item_id=%d, sublemma_id=%d, new_sublemma_id=%d \
                                                        where person_id=%d ;"
//TuPu
#define	LOAD_PERSON_TP_SQL "select person_id,name_chs,name_eng,name_chs_other,name_eng_other,\
                                                    item_id,sublemma_id,new_sublemma_id \
                                                    from %s \
                                                    where item_id > 0 ;"
#define UPDATE_PERSON_TP_SQL "update %s \
                                                        set tp_json=\'%s\' \
                                                        where person_id=%d ;"
//Rank
#define  RANK_TABLE "person_rank"
#define INSERT_PERSON_RANK_SQL "insert into %s values "
#define DEL_PERSON_RANK_SQL "delete from %s"

const int MAX_BUF_LEN = 512*1024; //512k
const int SPIDER_THREAD_NUM = 6; //6个抓取线程

enum QTYPE
{
    BK_Q = 0,
    TP_Q,
};

struct stPersonDetail
{
    int _id;
    string _name_chs;
    string _name_eng;
    string _name_chs_other;
    string _name_eng_other;
    int _itemId; 
    int _subLemmaId;
    int _newSubLemmaId;
    stPersonDetail() : _id(-1), _itemId(-1), _subLemmaId(-1), _newSubLemmaId(-1)   
    { }
};

struct stPersonResult
{
    int _id;
    int _bk_itemid;
    int _bk_subLemmaId;
    int _bk_new_subLemmaId;
    string _tp_json;
    stPersonResult() 
    {}
    stPersonResult(int id, int bk_itemid, int bk_subLemmaId, int bk_new_subLemmaId) :
        _id(id), 
        _bk_itemid(bk_itemid), 
        _bk_subLemmaId(bk_subLemmaId),
        _bk_new_subLemmaId(bk_new_subLemmaId)
    { }
};

struct Person
{
    string name;
    int rank;
    int searchCnt;
    string image;
};

class DBInterface : public IDBInterface, public Singleton<DBInterface>
{
public:
    //初始化
    bool init(char* ip, uint port, char* user, char* pwd, char* dbname, char* encoding);
    //人物库
    bool	loadPersonData(const char* sql);
    vector<stPersonDetail>* getPersonData() {return &_persons; }
    // 存储
    bool savePersonData(QTYPE qtype, std::list<stPersonResult>& _dataQ);
    bool saveRankData(vector<Person>& data);

private:
    vector<stPersonDetail> _persons;
};

//一只蜘蛛对应一个线程
class Spider
{
public:
    typedef void (Spider::*CbFunc)(int index);  //定义类成员函数指针的类型
    struct stThrParam
    {
        Spider* _spider;
        int _threadIndex;
        CbFunc _func;
        stThrParam(Spider* spider=NULL, int index=-1, CbFunc f=NULL) : _spider(spider), _threadIndex(index), _func(f)
        { }
    };

    struct stTPJsonItem
    {
        int toId;
        string toName;
        string relation;
    };

public:
    bool init();
    void setPersonData(std::vector<stPersonDetail>* data) {_persons=data;}
    bool getBKId(const char* bk_key, int& id, int& subLemmaId, int& newSubLemmaId);
    bool getBKIdMulti(const char* bk_key, vector<int>& list, vector<int>& newlist);
    bool getTP(int id, string& fromName, vector<stTPJsonItem>& relations);
    
protected:
    int _requestBKId(const char* card_url, const char* bk_key, int bk_len, char* resp_buf, int resp_buf_len);
    int _requestTP(const char* relation_url, int id, char* resp_buf, int resp_buf_len);

public:
    //BK
    void dwloadBK(int index);
    void dwloadBKMultiPerson(int index);

    //TuPu
    void dwloadTP(int index);

    //call back
    static void* run_dwload(void* param);
    void start(int index, CbFunc f);
    void wait();

    //get videoId according to bkId
    int getVideoId(int fromId);
    bool genJson(int fromId, string& fromName, vector<stTPJsonItem>& relations, string& json);

private:
    //下载线程
    stThrParam _param;
    Thread _dwload_thr;
    std::list<stPersonResult> _personQ;
    //http-client
    HttpClient _h;
    char _buffer[MAX_BUF_LEN];
    //person's data
    std::vector<stPersonDetail>* _persons;
};

//蜘蛛对应多线程
class SpiderMgr : public Singleton<SpiderMgr>
{
    friend class Spider;

public:
    typedef void (SpiderMgr::*CbFunc)();  //定义类成员函数指针的类型
    struct stThrParam
    {
        SpiderMgr* _spiderMgr;
        CbFunc _func;
        stThrParam(SpiderMgr* sp=NULL, CbFunc f=NULL) : _spiderMgr(sp), _func(f)
        { }
    };

public:
    bool init(int app_Id, const char* api_key, const char* secret_key);
    Spider* getSpider(int index);
    bool release();

    //BK
    bool getBKToken();
    void startBK();
    void waitBK();
    void updateBK();

    //TuPu
    void startTP();
    void waitTP();
    void updateTP();

    //Person Q
    void flushPersonQ(QTYPE qtype);
    bool addToPersonQ(std::list<stPersonResult>& result);
    int getPersonQSize() {return _dataQ.size();}

    //call back
    static void* run_update(void* param);

protected:
    int _requestToken(const char* token_url, char* resp_buf, int resp_buf_len);

private:
    //http-client instance
    HttpClient _h;
    //下载线程
    Spider _spider[SPIDER_THREAD_NUM];
    bool _running;
    //更新线程
    Thread _updata_thr;
    stThrParam _param;
    //结果
    std::list<stPersonResult> _dataQ;
    MLock _lockQ;
    //app info
    int _app_Id;
    string _api_key;
    string _secret_key;
    string _access_token;
    string _refresh_token;
};

bool test_spider();
bool test_bk();
bool test_tupu();
bool test_rank();

#endif  //_TEST_SPIDER_H
