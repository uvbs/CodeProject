#ifndef __LIB_PHP_SERIALIZER_H__
#define __LIB_PHP_SERIALIZER_H__

#include <string>

#include "../iserialize.h"

namespace serializer {

class PHPSerializer : public Serializer {
public:
    ~PHPSerializer() {}
    std::string& Serialize(const Object& root, std::string& output);

private:
    void SerializeObject(const Object& obj, std::string& output);
    void   SerializeNull(const Object& obj, std::string& output);
    void   SerializeBool(const Object& obj, std::string& output);
    void    SerializeInt(const Object& obj, std::string& output);
    void SerializeDouble(const Object& obj, std::string& output);
    void SerializeString(const Object& obj, std::string& output);
    void  SerializeArray(const Object& obj, std::string& output);
};

class PHPUnserializer : public Unserializer {
public:
    PHPUnserializer();
    ~PHPUnserializer() {}
    bool Unserialize(const char* input, int len, Object& obj);
    std::string ErrorMessage() { return error_message_; }

private:
    bool Unserialize(Object& obj);
    bool UnserializeNull(Object& obj);
    bool UnserializeBool(Object& obj);
    bool UnserializeInt(Object& obj);
    bool UnserializeDouble(Object& obj);
    bool UnserializeString(Object& obj);
    bool UnserializeArray(Object& obj);

    const char* begin_;
    const char* end_;
    std::string error_message_;
};

}

#endif
