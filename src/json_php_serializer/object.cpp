#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "format.h"
#include "object.h"
#include "exception.h"

using namespace std;

namespace serializer {

Object::Object()
    : type_(Type::Null)
    , index_(0)
{
}

Object::Object(bool b)
    : type_(Type::Bool)
    , index_(0)
    , value_b_(b)
{
}

Object::Object(int32_t num)
    : type_(Type::Int)
    , index_(0)
    , value_i_(num)
{
}

Object::Object(int64_t num)
    : type_(Type::Int)
    , index_(0)
    , value_i_(num)
{
}

Object::Object(size_t num)
    : type_(Type::Int)
    , index_(0)
    , value_i_(num)
{
}

Object::Object(double num)
    : type_(Type::Double)
    , index_(0)
    , value_d_(num)
{
}

Object::Object(const char* str)
    : type_(Type::String)
    , index_(0)
    , value_s_(str)
{
}

Object::Object(const std::string& str)
    : type_(Type::String)
    , index_(0)
    , value_s_(str)
{
}

Object::Object(const Object& obj)
    : type_(Type::Null)
    , index_(0)
{
    *this = obj;
}

void Object::destroy()
{
    type_ = Type::Null;
    index_ = 0;
    value_b_ = false;
    value_i_ = 0;
    value_d_ = 0;
    value_s_ = "";
    value_array_.clear();
}

Object& Object::operator=(bool val)
{
    destroy();
    type_ = Type::Bool;
    value_b_ = val;
    return *this;
}

Object& Object::operator=(int32_t val)
{
    destroy();
    type_ = Type::Int;
    value_i_ = val;
    return *this;
}

Object& Object::operator=(int64_t val)
{
    destroy();
    type_ = Type::Int;
    value_i_ = val;
    return *this;
}

Object& Object::operator=(double val)
{
    destroy();
    type_ = Type::Double;
    value_d_ = val;
    return *this;
}

Object& Object::operator=(const char* val)
{
    destroy();
    type_ = Type::String;
    value_s_ = val;
    return *this;
}

Object& Object::operator=(const std::string& val)
{
    destroy();
    type_ = Type::String;
    value_s_ = val;
    return *this;
}

Object& Object::operator=(const Object& o)
{
    destroy();
    type_ = o.type_;

    switch (o.GetType()) {
        case Type::Bool:
            return *this = o.GetBool();
        case Type::Int:
            return *this = o.GetInt();
        case Type::Double: 
            return *this = o.GetDouble();
        case Type::String:
            return *this = o.c_str();
        case Type::Vector:
        case Type::Map:
            {
                index_ = o.index_;
                value_array_ = o.value_array_;
            }
            return *this;
        default:
            return *this;
    }
    return *this;
}

Object& Object::operator[](const Object& index)
{
    if (!IsArray()) {
        throw exception("TypeError: expected object type vector/map");
    }

    if (IsVector() && !index.IsInt()) {
        throw exception("TypeError: unexpected vector index");
    }

    if (IsMap() && !index.IsInt() && !index.IsString()) {
        throw exception("TypeError: unexpected map::key_type");
    }

    ObjectArray::iterator iter = value_array_.begin();
    for (; iter != value_array_.end(); ++(++iter)) {
        if (*iter == index) {
            return *(++iter);
        }
    }

    value_array_.push_back(index);
    value_array_.push_back(Object());
    return value_array_.back();
}

const Object& Object::operator[](const Object& index) const
{
    return at(index);
}

Object Object::none()
{
    return Object();
}

Object Object::map()
{
    Object map;
    map.type_ = Type::Map;
    return map;
}

Object Object::vector()
{
    Object vector;
    vector.type_ = Type::Vector;
    return vector;
}

Object::~Object()
{
    destroy();
}

bool Object::GetBool() const
{
    return value_b_;
}

int64_t Object::GetInt() const
{
    return value_i_;
}

double Object::GetDouble() const
{
    return value_d_;
}

const string& Object::GetString() const
{
    return value_s_;
}

string& Object::GetString()
{
    return value_s_;
}

const char* Object::data() const
{
    if (IsString()) {
        return value_s_.data();
    }
    return NULL;
}

const char* Object::c_str() const
{
    if (IsString()) {
        return value_s_.c_str();
    }
    return NULL;
}

bool Object::empty() const
{
    if (IsArray()) {
        return value_array_.empty();
    } else if (IsString()) {
        return value_s_.empty();
    }
    return false;
}

size_t Object::size() const
{
    if (IsArray()) {
        return value_array_.size() / 2;
    } else if (IsString()) {
        return value_s_.size();
    }
    return 0;
}

size_t Object::count() const
{
    if (IsArray()) {
        return (value_array_.size() / 2);
    } 
    return 0;
}

size_t Object::length() const
{
    return size();
}

/*
 * Type::Vector
 * 可以向vector中追加数据
 * */
void Object::push_back(const Object& obj)
{
    if (!IsVector()) {
        throw exception("TypeError: push_back() expected object type vector");
    }

    value_array_.push_back(index_++);
    value_array_.push_back(obj);
}

void Object::insert(const Object& key, const Object& val)
{
    if (!IsArray()) {
        throw exception("TypeError: insert() expected object type map");
    }
    if (!key.IsInt() && !key.IsString()) {
        throw exception("TypeError: insert() expected map key int/string");
    }

    if (IsMap()) {
        ObjectArray::iterator iter = value_array_.begin();
        for (; iter != value_array_.end(); ++(++iter)) {
            if (*iter == key) {
                return;
            }
        }
    }

    value_array_.push_back(key);
    value_array_.push_back(val);
}

const Object& Object::at(const Object& index) const
{
    if (!IsArray()) {
        throw exception("at() TypeError: expected object type vector/map");
    }

    if (IsVector() && !index.IsInt()) {
        throw exception("at() TypeError: unexpected vector index");
    }

    if (IsVector()) {
        return value_array_[index.GetInt() * 2 + 1];
    } else {
        ObjectArray::const_iterator iter = value_array_.begin();
        for (; iter != value_array_.end(); ++(++iter)) {
            if (*iter == index) {
                return *(++iter);
            }
        }
    }

    throw exception("at() Not Found");
}

Object& Object::at(const Object& index)
{
    if (!IsArray()) {
        throw exception("at() TypeError: expected object type vector/map");
    }

    if (IsVector() && !index.IsInt()) {
        throw exception("at() TypeError: unexpected vector index");
    }

    ObjectArray::iterator iter = value_array_.begin();
    for (; iter != value_array_.end(); ++(++iter)) {
        if (*iter == index) {
            return *(++iter);
        }
    }

    throw exception("at() Not Found");
}

Object& Object::back()
{
    return value_array_.back();
}

bool Object::operator==(const Object& obj) const
{
    if (this->GetType() != obj.GetType()) {
        return false;
    }

    switch (this->GetType()) {
        case Type::Bool:
            return *this == obj.GetBool();
        case Type::Int:
            return *this == obj.GetInt();
        case Type::Double: 
            return *this == obj.GetDouble();
        case Type::String:
            return *this == obj.GetString();
        case Type::Vector:
        case Type::Map:
            return value_array_ == obj.value_array_;
        default:
            return false;
    }
    return false;
}

bool Object::operator==(bool b) const
{
    if (!IsBool()) {
        return false;
    }
    return GetBool() == b;
}

bool Object::operator==(int32_t n) const
{
    if (!IsInt()) {
        return false;
    }
    return GetInt() == n;
}

bool Object::operator==(int64_t n) const
{
    if (!IsInt()) {
        return false;
    }
    return GetInt() == n;
}

bool Object::operator==(double n) const
{
    if (!IsDouble()) {
        return false;
    }
    double x = GetDouble() - n;
    if (x >= -0.00001 && x <= 0.00001) {
        return true;
    }
    return false;
}

bool Object::operator==(const char* str) const
{
    if (!IsString()) {
        return false;
    }
    const string& s = GetString();
    return (strcmp(s.c_str(), str) == 0);
}

bool Object::operator==(const string& str) const
{
    if (!IsString()) {
        return false;
    }
    const string& s = GetString();
    if (s.size() == str.size() && s == str) {
        return true;
    }
    return false;
}

bool Object::operator!=(const Object& obj) const
{
    return !(*this == obj);
}

bool Object::operator<(const Object& obj) const
{
    if (GetType() != obj.GetType()) {
        return GetType() < obj.GetType();
    }
    switch (this->GetType()) {
        case Type::Bool:
            return  value_b_ < obj.value_b_;
        case Type::Int:
            return value_i_ < obj.value_i_;
        case Type::Double: 
            return value_d_ < obj.value_d_;
        case Type::String:
            return value_s_ < obj.value_s_;
        case Type::Vector:
        case Type::Map:
        default:
            return false;
    }

    return false;
}

Object::iterator& Object::iterator::operator++() 
{
    if (++iter_ == end_) {
        return *this;
    } else {
        ++iter_;
    }
    return *this;
}

Object::iterator Object::iterator::operator++(int) 
{
    iterator tmp = *this;
    ++(++iter_);
    return tmp;
}

Object::iterator& Object::iterator::operator--() 
{
    if (--iter_ == begin_) {
        return *this;
    } else {
        --iter_;
    }
    return *this;
}

Object::iterator Object::iterator::operator--(int) 
{
    iterator tmp = *this;
    --(--iter_);
    return tmp;
}

bool Object::iterator::operator==(const Object::iterator& rhs) 
{
    return iter_ == rhs.iter_;
}

bool Object::iterator::operator!=(const iterator& rhs) 
{ 
    return !(*this == rhs); 
}

Object::iterator Object::begin()
{
    switch (GetType()) {
        case Type::Null:
            throw exception("invalid .begin() operation on none");
        case Type::Int:
            throw exception("invalid .begin() operation on int");
        case Type::Double:
            throw exception("invalid .begin() operation on double");
        case Type::String:
            throw exception("invalid .begin() operation on string");
        case Type::Vector:
        case Type::Map:  
        case Type::Array:
            {
                ObjectArray::iterator iter = value_array_.begin();
                ObjectArray::iterator end = value_array_.end();
                return iterator(iter, iter, end);
            }
        default:
            throw exception("unhandled .begin() operation");
    }
}

Object::const_iterator Object::begin() const
{
    iterator result = const_cast<Object*>(this)->begin();
    return static_cast<Object::const_iterator&>(result);
}

Object::iterator Object::end()
{
    switch (GetType()) {
        case Type::Null:
            throw exception("invalid .end() operation on none");
        case Type::Int:
            throw exception("invalid .end() operation on int");
        case Type::Double:
            throw exception("invalid .end() operation on double");
        case Type::String:
            throw exception("invalid .end() operation on string");
        case Type::Vector:
        case Type::Map:  
        case Type::Array:  
            {
                ObjectArray::iterator iter = value_array_.end();
                ObjectArray::iterator begin = value_array_.begin();
                return iterator(iter, begin, iter);
            }
        default:
            throw exception("unhandled .end() operation");
    }
}

Object::const_iterator Object::end() const
{
    iterator result = const_cast<Object*>(this)->end(); 
    return static_cast<Object::const_iterator&>(result);
}

const Object& Object::iterator::key() const
{
    return *iter_;
}

Object& Object::iterator::value()
{
    return *(iter_ + 1);
}

const Object& Object::const_iterator::key() const
{
    return *iter_;
}

const Object& Object::const_iterator::value() const
{
    return *(iter_ + 1);
}

Object::iterator Object::find(const Object& key)
{
    if (IsArray()) {
        ObjectArray::iterator begin = value_array_.begin();
        ObjectArray::iterator end = value_array_.end();
        ObjectArray::iterator iter = value_array_.begin();
        for (; iter != value_array_.end(); ++(++iter)) {
            if (*iter == key) {
                return iterator(iter, begin, end);
            }
        }
    }
    return end();
}

Object::const_iterator Object::find(const Object& key) const
{
    iterator result = const_cast<Object*>(this)->find(key); 
    return static_cast<Object::const_iterator&>(result);
}

bool Object::find(const Object& key, Object& value)
{    
    return find(key, value, none());
}

bool Object::find(const Object& key, Object& value, const Object& default_value)
{    
    ObjectArray::iterator iter = value_array_.begin();
    for (; iter != value_array_.end(); ++(++iter)) {
        if (*iter == key) {
            value = *(iter + 1);
            return true;
        }
    }

    value = default_value;
    return false;
}

size_t Object::erase(const Object& key)
{
    if (!IsArray()) {
        throw exception("erase() TypeError: expected object type vector/map");
    }

    Object::iterator pos = find(key);
    if (pos == end()) {
        return 0;
    }

    erase(pos);

    return 1;
}

Object::iterator Object::erase(iterator pos)
{
    if (!IsArray()) {
        throw exception("erase() TypeError: expected object type vector/map");
    }

    ObjectArray::iterator iter = pos.iter();
    iter = value_array_.erase(iter);
    iter = value_array_.erase(iter);

    if (IsVector()) {
        ObjectArray::iterator tmp = value_array_.begin();
        int64_t index = 0;
        for (; tmp != value_array_.end(); ++(++tmp), ++index) {
            *tmp = index;
        }
        index_ = index;
    }

    return iterator(iter, value_array_.begin(), value_array_.end());
}

Object::iterator Object::erase(Object::iterator first, Object::iterator last)
{
    if (!IsArray()) {
        throw exception("erase() TypeError: expected object type vector/map");
    }

    ObjectArray::iterator iter;
    for (iter = first.iter(); iter != last.iter(); ++iter) {
        iter = value_array_.erase(iter);
    }

    if (IsVector()) {
        ObjectArray::iterator tmp = value_array_.begin();
        int64_t index = 0;
        for (; tmp != value_array_.end(); ++(++tmp), ++index) {
            *tmp = index;
        }
        index_ = index;
    }

    return iterator(iter, value_array_.begin(), value_array_.end());
}

}
