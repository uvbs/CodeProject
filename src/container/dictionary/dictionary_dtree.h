#ifndef __QSS_COMMON_DICTIONARY_DTREE_H__
#define __QSS_COMMON_DICTIONARY_DTREE_H__

#include <cstring>
#include <string>
#include <vector>
#include <map>

#include "../../json_php_serializer/object.h"
#include "dictionary.h"
#include "dtree.h"

namespace cloud_search {

typedef struct {
        unsigned int pos;
        unsigned short num;
} array_index_t;


class DictionaryDTree : public DictionaryBase {
public:
    DictionaryDTree(const std::string& name);
    ~DictionaryDTree();

    bool Load(const char* file);
    size_t Count();
    const std::string& Name();
    bool Find(const char* key);
    bool Find(const char* key, serializer::Object*& value);

private:
    std::string name_;
    std::vector<std::pair<std::string, std::string> > dict_;
    DTreeClass<array_index_t> *tree;

};

}

#endif
