#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../format.h"
#include "serialize.h"

using namespace std;

namespace serializer {

/*
 * JsonSerializer
 * */
void JsonSerializer::SerializeNull(const Object& obj, string& out)
{
    out.append("null");
}

void JsonSerializer::SerializeBool(const Object& obj, string& out)
{
    if (obj == true) {
        out.append("true");
    } else {
        out.append("false");
    }
}

void JsonSerializer::SerializeInt(const Object& obj, string& out)
{
    out.append(FormatInt(obj.GetInt()).c_str());
}

void JsonSerializer::SerializeDouble(const Object& obj, string& out)
{
    char buf[96];
    sprintf(buf, "%f", obj.GetDouble());
    out.append(buf);
}

static char HexDigit(int c)
{
    return c < 10 ? c + '0' : c - 10 + 'a';
}

void JsonSerializer::SerializeString(const Object& obj, string& out)
{
    const string& input = obj.GetString();

    /* 性能原因注释 */
    //out.reserve(out.size() + input.size() + 2);
    out.push_back('\"');

    const unsigned char* p = (const unsigned char*)input.c_str();
    const unsigned char* e = (const unsigned char*)input.c_str() + input.size();

    while (p < e) {
        if (*p == '\\' || *p == '\"') {
            out.push_back('\\');
            out.push_back(*p++);
        } else if (*p <= 0x1f) {
            switch (*p) {
                case '\b': out.append("\\b"); p++; break;
                case '\f': out.append("\\f"); p++; break;
                case '\n': out.append("\\n"); p++; break;
                case '\r': out.append("\\r"); p++; break;
                case '\t': out.append("\\t"); p++; break;
                default:
                    // note that this if condition captures non readable chars
                    // with value < 32, so size = 1 byte (e.g control chars).
                    out.append("\\u00");
                    out.push_back(HexDigit((*p & 0xf0) >> 4));
                    out.push_back(HexDigit(*p & 0xf));
                    p++;
            }                                                                                                                    
        } else {
            out.push_back(*p++);
        } 
    }   

    out.push_back('\"');
}

void JsonSerializer::SerializeVector(const Object& obj, string& out)
{
    out.append("[");

    Object::const_iterator iter = obj.begin();
    for (; iter != obj.end(); ++iter) {
        const Object& val = iter.value();

        if (iter != obj.begin()) {
            out.append(",");
        }

        SerializeObject(val, out);
    }

    out.append("]");
}

void JsonSerializer::SerializeMap(const Object& obj, string& out)
{
    out.append("{");

    Object::const_iterator iter = obj.begin();
    for (; iter != obj.end(); ++iter) {
        const Object& key = iter.key();
        const Object& val = iter.value();

        if (!key.IsString()) {
            return;
        }

        if (iter != obj.begin()) {
            out.append(",");
        }

        SerializeString(key, out);
        out.append(":");
        SerializeObject(val, out);
    }

    out.append("}");
}

void JsonSerializer::SerializeObject(const Object& root, string& out)
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
            SerializeVector(root, out);
            break;
        case Object::Type::Map:
            SerializeMap(root, out);
            break;
        default:
            break;
    }
}

string& JsonSerializer::Serialize(const Object& root, string& out)
{
    out.clear();
    SerializeObject(root, out);
    return out;
}

/*
 * JsonUnserializer 
 * */

/* Utility to jump whitespace and cr/lf */
static const char* skip(const char *in) 
{
    while (in && *in && (unsigned char)*in <= 32) {
        in++; 
    }
    return in;
}

JsonUnserializer::JsonUnserializer()
    : current_(NULL)
    , begin_(NULL)
    , end_(NULL)
{
}

bool JsonUnserializer::Unserialize(const string& input, Object& obj)
{
    return Unserialize(input.c_str(), input.size(), obj);
}

bool JsonUnserializer::Unserialize(const char* input, int len, Object& obj)
{
    if (input == NULL || len <= 0) {
        return false;
    }

    begin_ = skip(input);
    end_ = input + len;
    current_ = begin_;

    obj = Object::none();
    return Unserialize(obj);
}

void JsonUnserializer::ErrorMessage(const string& message)
{
    int pos = current_ - begin_;
    int backward = end_ - current_;
    if (backward > 50) {
        backward = 50;
    }
    int forward = current_ - begin_;
    if (forward > 20) {
        forward = 20;
    }
    error_message_ = message + " pos=" 
        + FormatInt(pos).str() + " '" 
        + string(current_ - forward, backward + forward) + "'";
}

bool JsonUnserializer::Unserialize(Object& obj)
{
    if (strncmp(current_, "null", 4) == 0) {
        return UnserializeNull(obj);
    } 

    if (strncmp(current_, "false", 5) == 0) {
        return UnserializeBool(obj, false);
    }

    if (strncmp(current_, "true", 4) == 0) {
        return UnserializeBool(obj, true);
    }

    if (*current_ == '\"') {
        return UnserializeString(obj);
    }

    if (*current_ == '-' || (*current_ >= '0' && *current_ <= '9')) {
        return UnserializeNumber(obj);
    }

    if (*current_ == '[') {
        return UnserializeVector(obj);
    }

    if (*current_ == '{') {
        return UnserializeMap(obj);
    }

    ErrorMessage("expected json value.");

    return false;
}

/*
 * Null
 * Format: null
 * */
bool JsonUnserializer::UnserializeNull(Object& obj)
{
    current_ += 4;
    return true;
}

/*
 * Boolean
 * Format: false|true
 * Example: false 
 * */
bool JsonUnserializer::UnserializeBool(Object& obj, bool value)
{
    obj = value;
    if (value) {
        current_ += 4;
    } else {
        current_ += 5;
    }
    return true;
}

/*
 * Number 
 * Format: n
 * Example: 100
 * */
bool JsonUnserializer::UnserializeNumber(Object& obj)
{
    double n = 0; 
    double sign = 1; 
    double scale = 0;
    int subscale = 0; 
    int signsubscale = 1;
    bool is_double = false;

    if (*current_ == '-') {
        /* Has sign? */
        sign = -1;
        current_++;   
    }

    if (*current_ == '0') {
        /* is zero */
        current_++;
    }

    if (*current_ >= '1' && *current_ <= '9') {
        /* Number? */
        do { 
            n = (n * 10.0) + (*current_++ - '0');   
        } while (*current_ >= '0' && *current_ <= '9'); 
    }

    if (*current_ == '.' && current_[1] >= '0' && current_[1] <= '9') {
        /* Fractional part? */ 
        is_double = true;
        current_++;      
        do {
            n = (n * 10.0) + (*current_++ - '0');
            scale--; 
        } while (*current_ >= '0' && *current_ <= '9');
    }   

    if (*current_ == 'e' || *current_ == 'E') {
        /* Exponent? */
        is_double = true;
        current_++;
        if (*current_ == '+') {
            current_++; 
        } else if (*current_ == '-') {
            /* With sign? */
            signsubscale = -1;
            current_++;      
        }
        while (*current_ >= '0' && *current_ <= '9') {
            /* Number? */
            subscale = (subscale * 10) + (*current_++ - '0');   
        }
    }

    /* number = +/- number.fraction * 10^+/- exponent */
    n = sign * n * pow(10.0,(scale + subscale * signsubscale));   

    if (is_double) {
        obj = n;
    } else {
        obj = (int64_t)n;
    }

    return true;
}

static unsigned parse_hex4(const char *str)
{
    unsigned h = 0;
    if (*str >= '0' && *str <= '9')
        h += (*str) - '0';
    else if (*str >= 'A' && *str <= 'F')                                                                                       
        h += 10 + (*str) - 'A'; 
    else if (*str >= 'a' && *str <= 'f') 
        h += 10 + (*str) - 'a'; 
    else 
        return 0;

    h = h << 4;
    str++;
    if (*str >= '0' && *str <= '9') 
        h += (*str) - '0'; 
    else if (*str >= 'A' && *str <= 'F') 
        h += 10 + (*str) - 'A'; 
    else if (*str >= 'a' && *str <= 'f') 
        h += 10 + (*str) - 'a'; 
    else 
        return 0;

    h = h << 4;
    str++;
    if (*str >= '0' && *str <= '9') 
        h += (*str) - '0'; 
    else if (*str >= 'A' && *str <= 'F') 
        h += 10 + (*str) - 'A'; 
    else if (*str >= 'a' && *str <= 'f') 
        h += 10 + (*str) - 'a'; 
    else 
        return 0;

    h = h << 4;
    str++;
    if (*str >= '0' && *str <= '9') 
        h += (*str) - '0'; 
    else if (*str >= 'A' && *str <= 'F') 
        h += 10 + (*str) - 'A'; 
    else if (*str >= 'a' && *str <= 'f') 
        h += 10 + (*str) - 'a'; 
    else                                                                                                                   
        return 0;

    return h;
}

/* Parse the input text into an unescaped cstring, and populate item. */
static const unsigned char firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

/*
 * String
 * Format: "" 
 * Example: "string";
 * */
bool JsonUnserializer::UnserializeString(Object& obj)
{
    if (*current_ != '\"') {
        return false;
    }

    unsigned uc, uc2;
    int len = 0;
    const char* ptr = current_ + 1;

    while (*ptr != '\"' && ptr <= end_ && ++len) {
        if (*ptr++ == '\\') {
            /* Skip escaped quotes. */
            ptr++;  
        }
    }

    char* value = (char*)malloc(len + 1);
    if (value == NULL) {
        return false;
    }

    current_ += 1;
    char* pvalue = value;
    while (current_ != end_ && *current_ != '\"') {
        if (*current_ != '\\') {
            *pvalue++ = *current_++;
        } else {
            current_++;
            switch (*current_) {
                case 'b': 
                    *pvalue++ = '\b'; 
                    break;
                case 'f': 
                    *pvalue++ = '\f'; 
                    break;
                case 'n': 
                    *pvalue++ = '\n'; 
                    break;
                case 'r': 
                    *pvalue++ = '\r'; 
                    break;
                case 't': 
                    *pvalue++ = '\t'; 
                    break;
                case 'u': 
                    /* transcode utf16 to utf8. */
                    uc = parse_hex4(current_ + 1);
                    /* get the unicode char. */
                    current_ += 4; 

                    if ((uc >= 0xDC00 && uc <= 0xDFFF) || uc == 0) {
                        /* check for invalid. */
                        break;  
                    }

                    if (uc >= 0xD800 && uc <= 0xDBFF) {   
                        /* UTF16 surrogate pairs. */
                        if (current_[1] != '\\' || current_[2] != 'u') {
                            /* missing second-half of surrogate. */
                            break;  
                        }
                        uc2 = parse_hex4(current_ + 3);
                        current_ += 6;
                        if (uc2 < 0xDC00 || uc2 > 0xDFFF) {
                            /* invalid second-half of surrogate. */
                            break;  
                        }
                        uc = 0x10000 + (((uc & 0x3FF) << 10) | (uc2 & 0x3FF));
                    }

                    len=4;
                    if (uc < 0x80) {
                        len=1;
                    } else if (uc < 0x800) {
                        len=2;
                    } else if (uc < 0x10000) {
                        len=3; 
                    }
                    pvalue += len;

                    switch (len) {
                        case 4: *--pvalue =((uc | 0x80) & 0xBF); uc >>= 6;
                        case 3: *--pvalue =((uc | 0x80) & 0xBF); uc >>= 6;
                        case 2: *--pvalue =((uc | 0x80) & 0xBF); uc >>= 6;
                        case 1: *--pvalue =(uc | firstByteMark[len]);
                    }
                    pvalue += len;
                    break;
                default:  
                    *pvalue++ = *current_; 
                    break;
            }
            current_++;
        }
    }

    *pvalue = '\0';

    if (*current_ == '\"') {
        current_++;
    }

    obj = value;
    free(value);

    return true;;
}

/*
 * Array
 * Format: [value,value(repeated per element)]
 * */
bool JsonUnserializer::UnserializeVector(Object& obj)
{
    obj = Object::vector();

    do {
        current_ = skip(current_ + 1);
    
        /* 空数组的情况 */
        if (obj.size() == 0 && *current_ == ']') {
            break;
        }

        Object value;
        if (!Unserialize(value)) {
            return false;
        }
        obj.push_back(value);

        current_ = skip(current_);
    } while (*current_ == ',');

    if (*current_ == ']') {
        current_++;
        return true;
    }

    ErrorMessage("expected ']'.");

    return false;
}

/*
 * Object
 * Format: {string:value,string:value} value
 * */
bool JsonUnserializer::UnserializeMap(Object& obj)
{
    obj = Object::map();

    do {
        current_ = skip(current_ + 1);

        /* 空map的情况 */
        if (obj.size() == 0 && *current_ == '}') {
            break;
        }

        Object key;
        if (!Unserialize(key)) {
            return false;
        }
        if (!key.IsString()) {
            ErrorMessage("expected string for object key name.");
            return false;
        }

        current_ = skip(current_);

        if (*current_ != ':') {
            ErrorMessage("expected ':'.");
            return false;
        }

        current_ = skip(current_ + 1);

        Object value;
        if (!Unserialize(value)) {
            return false;
        }
        obj.insert(key, value);

        current_ = skip(current_);

    } while (*current_ == ',');

    if (*current_ == '}') {
        current_++;
        return true;
    }

    ErrorMessage("expected '}'.");

    return false;
}

} // namespace serializer
