#ifndef __QSS_COMMON_DICTIONARY_UNORDERED_MAP_H__
#define __QSS_COMMON_DICTIONARY_UNORDERED_MAP_H__

#include <cstring>
#include <string>
#include <vector>
#include <boost/unordered_map.hpp>

#include "dictionary.h"

namespace cloud_search {

class DictionaryUnorderedMap : public DictionaryBase {
public:
    DictionaryUnorderedMap(const std::string& name);
    ~DictionaryUnorderedMap();

    size_t Count();
    const std::string& Name();
    bool Find(const char* key);
    bool Find(const char* key, serializer::Object*& value);
    void Add(const char* key, serializer::Object* value);

private:
    std::string name_;
    boost::unordered_map<std::string, serializer::Object*> dict_;
};

}

#endif
