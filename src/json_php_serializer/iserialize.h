#ifndef __LIB_SERIALIZER_INTERFACE_H__
#define __LIB_SERIALIZER_INTERFACE_H__

#include <string>

#include "object.h"

namespace serializer {

class Serializer {
public:
    virtual ~Serializer() {}
    virtual std::string& Serialize(const Object& root, std::string& output) = 0;
};

class Unserializer {
public:
    virtual ~Unserializer() {}
    virtual bool Unserialize(const char* input, int len, Object& obj) = 0;
    virtual std::string ErrorMessage() = 0;
};

}

#endif
