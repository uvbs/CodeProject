#include <iostream>
#include <string>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include "test_dictionary.h"
#include "../container/dictionary/dictionary_map.h"
#include "../container/dictionary/dictionary_dtree.h"


using namespace std;
using namespace cloud_search;
using namespace serializer;

void test1()
{
    DictionaryMap* dict = new DictionaryMap("number");

    assert(dict->Name() == "number");

    for (int i = 0; i < 100; i++) {
        char key[11] = {0};
        sprintf(key, "%d", i);
        Object* obj = new Object(i);
        dict->Add(key, obj);
    }

    assert(dict->Count() == 100);

    for (int i = 0; i < 100; i++) {
        char key[11] = {0};
        sprintf(key, "%d", i);
        assert(dict->Find(key) == true);
    }
    assert(dict->Find("100") == false);

    for (int i = 0; i < 100; i++) {
        char key[11] = {0};
        sprintf(key, "%d", i);
        Object* value = NULL;
        assert(dict->Find(key, value) == true);
        assert(*value == i);
    }
    delete dict;
}

void test2()
{
    DictionaryDTree* dict = new DictionaryDTree("number");
    printf("begin load\n");
    bool ret_load = dict->Load("./data/error.txt");
    printf("end load:%d\n",ret_load); 
    Object *value = new Object;
    string key = "360-周鸿祎-胡锦涛-大肉棍-操-64-六四-天安门-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    char* p = (char*)key.c_str();
    char* end = (char*)key.c_str() + key.size();
    struct timeval tv;
    gettimeofday(&tv, NULL);                                                                                                                                                                                                          
    uint64_t begin_time = tv.tv_sec * 1000000 + tv.tv_usec;
    int i = 0;
    while(i < 30) {
        while (p < end){
            dict->Find(p,value);
            p++;
        }
        p = (char*)key.c_str();
        i++;
    }
    gettimeofday(&tv, NULL); 
    uint64_t end_time = tv.tv_sec * 1000000 + tv.tv_usec;;
    printf("consume_time:%d\n",(uint32_t)((end_time - begin_time)/1000));
    delete dict;
}


void test_dictionary()
{
    test2();
}

