#ifndef __LIB_SERIALIZER_OBJECT_H__
#define __LIB_SERIALIZER_OBJECT_H__

#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace serializer {

struct ObjectMap;
class Object {
public:
    struct Type {
        enum ObjectType {
            Null = 0,
            Bool,
            Int,
            Double,
            String,
            Vector,
            Map,
            Array
        };
    };

    typedef std::vector<Object> ObjectArray;

public:
    static Object none();
    static Object map();
    static Object vector();

    Object();
    Object(bool b);
    Object(int32_t num);
    Object(int64_t num);
    Object(size_t num);
    Object(double num);
    Object(const char* str);
    Object(const std::string& str);
    Object(const Object& obj);

    ~Object();

    Object& operator=(bool val);
    Object& operator=(int32_t val);
    Object& operator=(int64_t val);
    Object& operator=(double val);
    Object& operator=(const char* val);
    Object& operator=(const std::string& val);
    Object& operator=(const Object& o);

    Type::ObjectType GetType() const { return type_; }
    bool IsNull() const { return type_ == Type::Null; } 
    bool IsBool() const { return type_ == Type::Bool; }
    bool IsInt() const { return type_ == Type::Int; }
    bool IsDouble() const { return type_ == Type::Double; }
    bool IsNumber() const { return IsInt() || IsDouble(); }
    bool IsString() const { return type_ == Type::String; }
    bool IsVector() const { return type_ == Type::Vector; }
    bool IsMap() const { return type_ == Type::Map; }
    bool IsArray() const { return IsVector() || IsMap(); }

    bool GetBool() const;
    int64_t GetInt() const;
    double GetDouble() const;
    std::string& GetString();
    const std::string& GetString() const;
    const char* data() const;
    const char* c_str() const;
    size_t length() const;
    size_t size() const;
    bool empty() const;

    /* vector */
    void push_back(const Object& obj);
    /* map */
    void insert(const Object& key, const Object& val);

    const Object& at(const Object& obj) const; 
    Object& at(const Object& obj); 
    Object& back();

    bool operator==(bool b) const;
    bool operator==(int32_t n) const;
    bool operator==(int64_t n) const;
    bool operator==(double n) const;
    bool operator==(const char* str) const;
    bool operator==(const std::string& str) const;
    bool operator==(const Object& obj) const;
    
    bool operator!=(const Object& obj) const;
    bool operator<(const Object& obj) const;

    size_t count() const;
    Object& operator[](const Object& index);
    const Object& operator[](const Object& index) const;

    class iterator {
        friend class Object;
    public:
        iterator& operator++();
        iterator  operator++(int);
        iterator& operator--();
        iterator  operator--(int);

        bool operator==(const iterator& rhs);
        bool operator!=(const iterator& rhs);

        const Object& key() const;
        Object& value();

    private :
        iterator(ObjectArray::iterator iter, ObjectArray::iterator begin, ObjectArray::iterator end)
            : iter_(iter), begin_(begin), end_(end) {}
        ObjectArray::iterator iter() { return iter_; }

        ObjectArray::iterator iter_;
        ObjectArray::iterator begin_;
        ObjectArray::iterator end_;
    };

    class const_iterator : public iterator {
        friend class Object;
    public:
        const Object& key() const;
        const Object& value() const;

    private:
        const_iterator(ObjectArray::iterator iter, ObjectArray::iterator begin, ObjectArray::iterator end) 
            : iterator(iter, begin, end) {}
    }; 

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    iterator find(const Object& key);
    const_iterator find(const Object& key) const;
    bool find(const Object& key, Object& value);
    bool find(const Object& key, Object& value, const Object& default_value);

    /* erase */
    size_t erase(const Object& key);
    iterator erase(iterator it);
    iterator erase(iterator first, iterator last);

private:
    void destroy();

private:
    Type::ObjectType type_;
    int64_t          index_;

    bool         value_b_;
    int64_t      value_i_;
    double       value_d_;
    std::string  value_s_;
    ObjectArray  value_array_;
};

}

#endif
