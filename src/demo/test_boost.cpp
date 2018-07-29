#include "socketdef.h"
#include "test_boost.h"
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "boost/regex.h"

using namespace std;

void test_regex()
{
#if defined(_LINUX64) && (__cplusplus == 201103L) || defined(_WIN32)
    const char* rule = "huC[0-9A-Z]{5}";
    boost::RegEx* regex = new boost::RegEx(rule);
    std::string plate1 = "huC0000 ";
    if (regex->Match(plate1))
    {
        cout << "matched...\n";
    }
    else
    {
        cout << "not matched...\n";
    }
    
    std::string plate2 = "»¦C000AA";
    if (regex->Match(plate2))
    {
        cout << "matched...\n";
    }
    else
    {
        cout << "not matched...\n";
    }
    
    unordered_map<int, int> m;
    int sz0 = m.size();
    m.insert(unordered_map<int, int>::value_type(1, 2));
    int sz1 = m.size();
    m.insert(unordered_map<int, int>::value_type(1, 3));
    int sz2 = m.size();
#endif
}

void test_smartptr()
{
#if defined(_LINUX64) && (__cplusplus == 201103L) || defined(_WIN32)
    struct MyStruct
    {
        char* _name;
        MyStruct()
        {
            _name = new char[32];
        }
        ~MyStruct()
        {
            if (_name)
            {
                delete[] _name;
            }
        }
    };
    MyStruct* obj = new MyStruct;
    shared_ptr<MyStruct> o(obj);
#endif
}

void test_boost()
{
    test_regex();
    test_smartptr();
}
