#include <string>
#include <vector>

#include "../../json_php_serializer/object.h"
#include "dictionary_unorderedmap.h"

using namespace std;
using namespace boost;
using namespace serializer;

namespace cloud_search {

DictionaryUnorderedMap::DictionaryUnorderedMap(const string& name)
    : name_(name)
{
}

DictionaryUnorderedMap::~DictionaryUnorderedMap()
{
    boost::unordered_map<string, Object*>::iterator iter = dict_.begin();
    for (; iter != dict_.end(); ++iter) {
        delete iter->second;
    }

    dict_.clear();
}

bool DictionaryUnorderedMap::Find(const char* key)
{
    if (dict_.find(key) == dict_.end()) {
        return false;
    }

    return true;
}

bool DictionaryUnorderedMap::Find(const char* key, Object*& value)
{
    boost::unordered_map<string, Object*>::iterator iter = dict_.find(key);
    if (iter == dict_.end()) {
        return false;
    }

    value = iter->second;

    return true;
}

void DictionaryUnorderedMap::Add(const char* key, Object* value)
{
    if (Find(key)) {
        delete value;
        return;
    }

    dict_.insert(make_pair(key, value));
}

size_t DictionaryUnorderedMap::Count()
{
    return dict_.size();
}

const string& DictionaryUnorderedMap::Name()
{
    return name_;
}

}
