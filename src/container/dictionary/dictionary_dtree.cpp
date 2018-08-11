#include <stdio.h>
#include "dictionary_dtree.h"
#include <algorithm>
#include <iostream>
#include "../../filereader/file_utils.h"

using namespace std;
using namespace serializer;

namespace cloud_search {
    
DictionaryDTree::DictionaryDTree(const std::string& name) : name_(name), tree(NULL)
{
}

DictionaryDTree::~DictionaryDTree()
{
    dict_.clear();

    if(NULL != tree)
        delete tree;
}

bool DictionaryDTree::Load(const char* file)
{
    tree = new DTreeClass<array_index_t>;
    if (NULL == tree)
        return false;

    if (check_file(file) < 0)
        return false;

    FILE *f = fopen(file, "rb");
    if (f == NULL)
    {
        return false;
    }

    char *save = NULL;
    array_index_t index = {0, 0};

    char buf[1024] = {0}, tmp[1024] = {0};
    char *line = buf;
    int i = 0, pos = 0;

    while (fgets(line, sizeof(buf), f))
    {
        if (line[0] == '#')
            continue;

        int len = strlen(line);
        while (isspace(line[len-1]))
            line[--len] = 0;

        if (len <= 0)
            continue;

        char sz_save[1024] = {'\t'};
        strcpy(sz_save,line);

        char* word = strtok(sz_save, "\t"); 

        char *ptr = strchr(line, '\t');
        if(NULL == ptr || *(ptr + 1) == '\0')
            continue;

        dict_.push_back(pair<string, string>(string(line, ptr - line), string(ptr + 1)));

        if (save && strcmp(word, save))
        {
            index.pos = pos;
            index.num = i - pos;
            if (i - pos >= 0xffff)
                index.num = 0xffff;

            if (tree->insert(save, strlen(save), index) < 0)
            {
                fclose(f);
                return false;
            }
            pos = i;
        }

        if (!save || strcmp(word, save))
        {
            save = strtok(line, "\t");
            line = (line == buf) ? tmp : buf;
        }

        i++;
    }

    if (save)
    {
        index.pos = pos;
        index.num = i - pos;
        if (i - pos >= 0xffff)
            index.num = 0xffff;

        if (tree->insert(save, strlen(save), index) < 0)
        {
            fclose(f);
            return false;
        }
    }

    fclose(f);

    if(0 == dict_.size())
        return false;
    else
        return true;
}


bool DictionaryDTree::Find(const char* key)
{
    if(0 == dict_.size())
        return false;

    int len     = strlen(key);
    char *ptr   = (char *)key;
    array_index_t item = {0, 0};
    int match_len = tree->match(ptr, len, item);

    if(match_len == len)
        return (item.pos >= 0 && item.num >= 1) ? true : false; 
    else 
        return false;
}

bool DictionaryDTree::Find(const char *key, serializer::Object*& value)
{
    const int max_num = 1024;
    array_index_t datas[max_num];
    int data_lens[max_num];

    char *ptr = (char *)key;
    int len = strlen(key);
    int matched_num = tree->match_all(ptr, len, (void**)&datas, data_lens, max_num);
   
    if (matched_num <= 0)
        return false;

    *value = Object::vector();
    for(int idx = 0; idx < matched_num; idx++)
    {
        Object item = Object::map();
        
        array_index_t* hit = (array_index_t*)&datas[idx];    
        if (hit->num <= 0)
            continue;

        item["key"] = dict_[hit->pos].first;
        item["len"] = data_lens[idx];

        Object val_vec = Object::vector();
        int hit_end = hit->pos + hit->num;

        for (int i = hit->pos; i < hit_end; i++)
        {
            val_vec.push_back(Object(dict_[i].second));
        }

        item["value"] = val_vec;
        value->push_back(item);
    }

    return true;
}

size_t DictionaryDTree::Count()
{
    return dict_.size();    
}

const string& DictionaryDTree::Name()
{
    return name_;
}

} 
