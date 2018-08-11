#include <string>
#include <vector>
#include <map>

#include "../../json_php_serializer/object.h"
#include "dictionary_map.h"

using namespace std;
using namespace serializer;

namespace cloud_search {

DictionaryMap::DictionaryMap(const string& name)
    : name_(name)
{
}

DictionaryMap::~DictionaryMap()
{
    map<string, Object*>::iterator iter = dict_.begin();
    for (; iter != dict_.end(); ++iter) {
        delete iter->second;
    }

    dict_.clear();
}

bool DictionaryMap::Find(const char* key)
{
    if (dict_.find(key) == dict_.end()) {
        return false;
    }

    return true;
}

bool DictionaryMap::Find(const char* key, Object*& value)
{
    map<string, Object*>::iterator iter = dict_.find(key);
    if (iter == dict_.end()) {
        return false;
    }

    value = iter->second;

    return true;
}

void DictionaryMap::Add(const char* key, Object* value)
{
    if (Find(key)) {
        delete value;
        return;
    }

    dict_.insert(make_pair(key, value));
}

size_t DictionaryMap::Count()
{
    return dict_.size();
}

const string& DictionaryMap::Name()
{
    return name_;
}

}
