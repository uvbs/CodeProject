#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "../format.h"
#include "serialize.h"

using namespace std;

namespace serializer {

/*
 * PHPSerializer
 * */
void PHPSerializer::SerializeNull(const Object& obj, string& out)
{
    out.append("N;");
}

void PHPSerializer::SerializeBool(const Object& obj, string& out)
{
    out.append("b:");
    out.append(FormatInt(obj.GetBool()).c_str());
    out.append(";");
}

void PHPSerializer::SerializeInt(const Object& obj, string& out)
{
    out.append("i:");
    out.append(FormatInt(obj.GetInt()).c_str());
    out.append(";");
}

void PHPSerializer::SerializeDouble(const Object& obj, string& out)
{
    char buf[96];
    sprintf(buf, "d:%f;", obj.GetDouble());
    out.append(buf);
}

void PHPSerializer::SerializeString(const Object& obj, string& out)
{
    out.append("s:");
    out.append(FormatInt(obj.size()).c_str());
    out.append(":\"");
    out.append(obj.c_str());
    out.append("\";");
}

void PHPSerializer::SerializeArray(const Object& obj, string& out)
{
    out.append("a:");
    out.append(FormatInt(obj.size()).c_str());
    out.append(":{");

    Object::const_iterator iter = obj.begin();
    for (; iter != obj.end(); ++iter) {
        const Object& key = iter.key();
        const Object& val = iter.value();

        if (key.IsInt()) {
            out.append("i:");
            out.append(FormatInt(key.GetInt()).c_str());
            out.append(";");
        } else {
            out.append("s:");
            out.append(FormatInt(key.size()).c_str());
            out.append(":\"");
            out.append(key.c_str());
            out.append("\";");
        } 

        SerializeObject(val, out);
    }

    out.append("}");
}

void PHPSerializer::SerializeObject(const Object& root, string& out)
{
    switch (root.GetType()) {
        case Object::Type::Null:
            SerializeNull(root, out);
            break;
        case Object::Type::Bool:
            SerializeBool(root, out);
            break;
        case Object::Type::Int:
            SerializeInt(root, out);
            break;
        case Object::Type::Double:
            SerializeDouble(root, out);
            break;
        case Object::Type::String:
            SerializeString(root, out);
            break;
        case Object::Type::Vector:
        case Object::Type::Map:
            SerializeArray(root, out);
            break;
        default:
            break;
    }
}

string& PHPSerializer::Serialize(const Object& root, string& out)
{
    out.clear();
    SerializeObject(root, out);
    return out;
}

/*
 * PHPUnserializer 
 * */

PHPUnserializer::PHPUnserializer()
    : begin_(NULL)
    , end_(NULL)
{
}

bool PHPUnserializer::Unserialize(const char* input, int len, Object& obj)
{
    if (input == NULL || len <= 0) {
        return false;
    }

    obj = Object::none();
    begin_ = input;
    end_ = input + len;

    return Unserialize(obj);
}

bool PHPUnserializer::Unserialize(Object& obj)
{
    char type = begin_[0];

    switch (type) {
        case 'N':
            return UnserializeNull(obj);
        case 'b':
            return UnserializeBool(obj);
        case 'i':
            return UnserializeInt(obj);
        case 'd':
            return UnserializeDouble(obj);
        case 's':
            return UnserializeString(obj);
        case 'a':
            return UnserializeArray(obj);
        default:
            break;
    }

    return false;
}

/*
 * Null 
 * Format: N; 
 * Example: N;
 * */
bool PHPUnserializer::UnserializeNull(Object& obj)
{
    if (++begin_ >= end_) {
        return false;
    }

    if (*begin_ != ';') {
        return false;
    }

    begin_++;

    obj = Object::none();

    return true;
}

/*
 * Boolean
 * Format: b:value; (does not store "true" or "false", does store '1' or '0')
 * Example: b:1;
 * */
bool PHPUnserializer::UnserializeBool(Object& obj)
{
    if (++begin_ >= end_) {
        return false;
    }

    if (*begin_ != ':') {
        return false;
    }

    if (++begin_ >= end_) {
        return false;
    }

    char* cursor = NULL;
    long int val = strtol(begin_, &cursor, 10);

    if (*cursor != ';') {
        return false;
    }

    begin_ = cursor + 1;

    obj = (val != 0);

    return true;
}

/*
 * Integer
 * Format: i:value;
 * Example: i:100;
 * */
bool PHPUnserializer::UnserializeInt(Object& obj)
{
    if (++begin_ >= end_) {
        return false;
    }

    if (*begin_ != ':') {
        return false;
    }

    if (++begin_ >= end_) {
        return false;
    }

    char* cursor = NULL;
    long int val = strtol(begin_, &cursor, 10);

    if (*cursor != ';') {
        return false;
    }

    begin_ = cursor + 1;

    obj = val;

    return true;
}

/*
 * Double 
 * Format: d:value;
 * Example: d:100.0;
 * */
bool PHPUnserializer::UnserializeDouble(Object& obj)
{
    if (++begin_ >= end_) {
        return false;
    }

    if (*begin_ != ':') {
        return false;
    }

    if (++begin_ >= end_) {
        return false;
    }

    char* cursor = NULL;
    double val = strtod(begin_, &cursor);

    if (*cursor != ';') {
        return false;
    }

    begin_ = cursor + 1;

    obj = val;

    return true;
}

/*
 * String
 * Format: s:size:value;
 * Example: s:10:"0123456789";
 * */
bool PHPUnserializer::UnserializeString(Object& obj)
{
    if (++begin_ >= end_) {
        return false;
    }

    if (*begin_ != ':') {
        return false;
    }

    if (++begin_ >= end_) {
        return false;
    }

    char* cursor = NULL;
    long size = strtol(begin_, &cursor, 10);

    if (*cursor != ':') {
        return false;
    }

    if (size < 0) {
        return false;
    }

    begin_ = cursor + 3 + size + 1;
    if (begin_ > end_) {
        return false;
    }

    if (cursor[1] != '"' || cursor[size + 2] != '"' || cursor[size + 3] != ';') {
        return false;
    }

    string str;
    str.resize(size);
    str.assign(cursor + 2, size);

    //TODO Object可以支持resize和assign方法初始化字符串
    obj = str;

    return true;;
}

/*
 * Array
 * Format: a:size:{key definition;value definition;(repeated per element)}
 * */
bool PHPUnserializer::UnserializeArray(Object& obj)
{
    if (++begin_ >= end_) {
        return false;
    }

    if (*begin_ != ':') {
        return false;
    }

    if (++begin_ >= end_) {
        return false;
    }

    char* cursor = NULL;
    long size = strtol(begin_, &cursor, 10);

    if (cursor[0] != ':' || cursor[1] != '{') {
        return false;
    }

    if (size < 0) {
        return false;
    }

    begin_ = cursor + 2;

    vector<Object*> kvs;
    for (long i = 0; i < size; ++i) {
        Object* key = new Object;
        Object* value = new Object;
        if (!Unserialize(*key)) {
            delete key;
            delete value;
            return false;
        }

        if (!key->IsInt() && !key->IsString()) {
            delete key;
            delete value;
            return false;
        }

        if (!Unserialize(*value)) {
            delete key;
            delete value;
            return false;
        }

        kvs.push_back(key);
        kvs.push_back(value);
    }

    bool is_vector = true;
    int64_t last_value = 0;
    vector<Object*>::iterator iter; 
    for (iter = kvs.begin(); iter != kvs.end(); ++(++iter)) {
        Object* key = *iter;

        if (!key->IsInt()) {
            is_vector = false;
            break;
        }

        if (key->IsInt() && key->GetInt() < 0) {
            is_vector = false;
            break;
        }

        if (iter == kvs.begin()) {
            if (key->GetInt() != 0) {
                is_vector = false;
                break;
            }
        } else if (key->GetInt() != (last_value + 1)) {
            is_vector = false;
            break;
        }

        last_value = key->GetInt();
    }

    if (is_vector) {
        obj = Object::vector();
    } else {
        obj = Object::map();
    }

    for (iter = kvs.begin(); iter != kvs.end(); ++(++iter)) {
        Object* key = *iter;
        Object* value = *(iter + 1);

        if (obj.IsVector()) {
            obj.push_back(*value);
        } else {
            obj.insert(*key, *value);
        }
        delete key;
        delete value;
    }

    if (*begin_ != '}') {
        return false;
    } 

    begin_ += 1;

    return true;
}

}
