#ifndef __QSS_COMMON_DICTIONARY_H__
#define __QSS_COMMON_DICTIONARY_H__

#include <string>
#include <vector>

namespace serializer {
    class Object;
}

namespace cloud_search {

class DictionaryBase {
public:
    virtual ~DictionaryBase() {};
    virtual size_t Count() = 0;
    virtual const std::string& Name() = 0;
    virtual bool Find(const char* key) = 0;
    virtual bool Find(const char* key, serializer::Object*& value) = 0;
};

}

#endif
