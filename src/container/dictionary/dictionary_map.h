#ifndef __QSS_COMMON_DICTIONARY_MAP_H__
#define __QSS_COMMON_DICTIONARY_MAP_H__

#include <cstring>
#include <string>
#include <vector>
#include <map>

#include "dictionary.h"

namespace cloud_search {

class DictionaryMap : public DictionaryBase {
public:
    DictionaryMap(const std::string& name);
    ~DictionaryMap();

    size_t Count();
    const std::string& Name();
    bool Find(const char* key);
    bool Find(const char* key, serializer::Object*& value);
    void Add(const char* key, serializer::Object* value);

private:
    std::string name_;
    std::map<std::string, serializer::Object*> dict_;
};

}

#endif
