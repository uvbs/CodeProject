#ifndef __LIB_JSON_SERIALIZER_H__
#define __LIB_JSON_SERIALIZER_H__

#include <string>

#include "../iserialize.h"

namespace serializer { 

class JsonSerializer : public Serializer {
public:
    ~JsonSerializer() {}
    std::string& Serialize(const Object& root, std::string& output);

private:
    void SerializeObject(const Object& obj, std::string& output);
    void   SerializeNull(const Object& obj, std::string& output);
    void   SerializeBool(const Object& obj, std::string& output);
    void    SerializeInt(const Object& obj, std::string& output);
    void SerializeDouble(const Object& obj, std::string& output);
    void SerializeString(const Object& obj, std::string& output);
    void SerializeVector(const Object& obj, std::string& output);
    void    SerializeMap(const Object& obj, std::string& output);
};

class JsonUnserializer : public Unserializer {
public:
    JsonUnserializer();
    ~JsonUnserializer() {}
    bool Unserialize(const char* input, int len, Object& obj);
    bool Unserialize(const std::string& input, Object& obj);
    std::string ErrorMessage() { return error_message_; };

private:
    void ErrorMessage(const std::string& message);
    bool Unserialize(Object& obj);
    bool UnserializeNull(Object& obj);
    bool UnserializeBool(Object& obj, bool value);
    bool UnserializeNumber(Object& obj);
    bool UnserializeString(Object& obj);
    bool UnserializeVector(Object& obj);
    bool UnserializeMap(Object& obj);

    const char* current_;
    const char* begin_;
    const char* end_;
    std::string error_message_;
};

} // namespace serializer

#endif
