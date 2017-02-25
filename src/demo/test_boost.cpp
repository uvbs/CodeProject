#include "socketdef.h"
#include "test_boost.h"
#include <iostream>

#if defined(_LINUX64) && (__cplusplus == 201103L)
#include <unordered_map>
#include "boost/regex.h"
#include "boost/shared_ptr.hpp"
#elif defined(_WIN32)
#include <unordered_map>
#include "boost/regex.h"
#include "boost/shared_ptr.hpp"
#endif

using std::cout;

#ifdef _LINUX64
#if( __cplusplus == 201103L )
#include <unordered_map>
#include <unordered_set>
using namespace std;
#endif
#else
using std::tr1::unordered_map;
#endif

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
    typedef unordered_map<int, int>::_Pairib PAIR;
    int sz0 = m.size();
    PAIR ret = m.insert(unordered_map<int, int>::value_type(1, 2));
    int sz1 = m.size();
    ret = m.insert(unordered_map<int, int>::value_type(1, 3));
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
    boost::shared_ptr<MyStruct> o(obj);
#endif
}

void test_boost()
{
    test_regex();
    test_smartptr();
}
