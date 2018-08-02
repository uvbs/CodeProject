#include "test_serializer.h"

using namespace std;
using namespace serializer;

namespace serializer_php {

void test_object()
{
    Object val;
    assert(val.IsNull());

    val = Object::none();
    assert(val.IsNull());

    val = 100;
    assert(val.IsInt());
    assert(val.GetInt() == 100);
    assert(val == 100);

    val = 123456789123456789;
    assert(val.IsInt());
    assert(val.GetInt() == 123456789123456789);
    assert(val == 123456789123456789);

    val = 123.123;
    assert(val.IsDouble());
    assert(val.GetDouble() == 123.123);
    assert(val == 123.123);

    val = true;
    assert(val.IsBool());
    assert(val.GetBool() == true);
    assert(val == true);

    val = "hello kitty";
    assert(val.IsString());
    assert(val.GetString() == "hello kitty");
    assert(val == "hello kitty");
    assert(!val.empty());
    assert(val.size() == strlen("hello kitty"));

    Object i = 1;
    val = i;
    assert(val.IsInt());
    assert(val.GetInt() == 1);
    assert(val == i);

    Object b = Object::vector();
    assert(b.IsVector());

    val = Object::vector();
    assert(val.IsArray());
    val.push_back("kitty");
    val.push_back(true);
    val.push_back(false);
    val.push_back(100);
    val.push_back(123.123);
    val.push_back("world");
    assert(val[5] == "world");
    assert(val.count() == 6);
    assert(val.find(5) != val.end());
    assert(val.erase(5) == 1);
    assert(val.count() == 5);
    assert(val.find(5) == val.end());
    assert(val.IsArray());

    val = Object::vector();
    for (int i = 1; i < 10; ++i) {
        val.push_back(i);
    }
    assert(val.IsArray());
    Object::iterator iter = val.begin();
    for (int i = 0; iter != val.end(); ++iter, ++i) {
        assert(iter.key().GetInt() == i);
        assert(iter.value().GetInt() == i + 1);
    }

    Object array = Object::vector();
    array.push_back(1);
    array.push_back(2);
    array.push_back(true);
    array.push_back(false);
    array.push_back("hello");
    array.push_back("kitty");
    array.push_back(123.123);
    assert(array.IsArray());
    assert(!array.empty());
    assert(array.size() == 7);
    assert(array.at(0) == 1);
    array.push_back("world");
    array.push_back(val);
    assert(array.at(0) == 1);
    assert(array.at(1) == 2);
    assert(array.at(2) == true);
    {
    Object::iterator it = array.find(1);
    assert(it != array.end());
    assert(it.value() == 2);
    }

    val = Object::map();
    val.insert("hello", "kitty");
    val.insert("zhang", "hua");
    val.insert("30", "abc");
    val.insert("array", array);
    assert(val.IsMap());
    assert(val.at("hello") == "kitty");
    assert(val.at("30") == "abc");
    assert(val.at("array").at(0) == 1);
    {
        Object::iterator iter = val.find("hello");
        assert(iter != val.end());
        assert(iter.key() == "hello");
        assert(iter.value() == "kitty");

        Object v;
        bool ret = val.find("zhang", v);
        assert(ret);
        assert(v == "hua");

        ret = val.find("NotFound", v, "NotFound");
        assert(!ret);
        assert(v == "NotFound");
    }

    val = Object::map();
    val["1"] = "one";
    val["2"] = "two";
    val["3"] = "three";
    val["4"] = "four";
    val["5"] = "five";
    val["6"] = "six";
    val["7"] = "seven";
    val["8"] = "eight";
    val["9"] = "nine";
    val["10"] = "ten";
    assert(val.IsMap());
    assert(val.count() == 10);
    assert(val.size() == 10);
    assert(val["1"] == "one");
    assert(val["5"] == "five");
    assert(val["10"] == "ten");
    Object& str = val["5"];
    assert(str.IsString());
    assert(str == "five");
    assert(val.count() == 10);
    assert(val.erase("10") == 1);
    assert(val.count() == 9);
    assert(val.find("10") == val.end());
    assert(val.erase(val.begin(), val.end()) == val.end());
    assert(val.count() == 0);
 
    val = Object::vector();
    assert(val.IsVector());
    val[0] = "zero";
    val[1] = "one";
    val[2] = "two";
    val[3] = "three";
    assert(val.count() == 4);
    assert(val[0] == "zero");
    assert(val[1] == "one");
    assert(val[2] == "two");
    assert(val[3] == "three");
    assert(val.at(0) == "zero");
    assert(val.at(1) == "one");
    assert(val.at(2) == "two");
    assert(val.at(3) == "three");

    const Object& a1 = val;
    const Object& s = a1[1];
    assert(s.IsString());
    assert(s == "one");
}

void test_null()
{
    PHPSerializer serializer;
    PHPUnserializer unserializer;
    bool ret = false;
    string str1;
    string str2;

    Object obj;
    serializer.Serialize(obj, str1);
    cout << str1 << endl;

    ret = unserializer.Unserialize(str1.c_str(), str1.size(), obj);
    assert(ret);
    assert(obj.IsNull());
    assert(serializer.Serialize(obj, str2) == str1);
}

void test_bool()
{
    PHPSerializer serializer;
    PHPUnserializer unserializer;
    bool ret = false;
    string str1;
    string str2;

    Object obj(true);
    serializer.Serialize(obj, str1);
    cout << str1 << endl;

    ret = unserializer.Unserialize(str1.c_str(), str1.size(), obj);
    assert(ret);
    assert(obj.IsBool());
    assert(serializer.Serialize(obj, str2) == str1);

    obj = false;
    serializer.Serialize(obj, str1);
    cout << str1 << endl;

    ret = unserializer.Unserialize(str1.c_str(), str1.size(), obj);
    assert(ret);
    assert(obj.IsBool());
    assert(serializer.Serialize(obj, str2) == str1);
}

void test_number()
{
    PHPSerializer serializer;
    PHPUnserializer unserializer;
    string str1, str2;
    bool ret = false;

    Object obj(100);
    serializer.Serialize(obj, str1); 
    cout << str1 << endl;
    ret = unserializer.Unserialize(str1.c_str(), str1.size(), obj);
    assert(ret);
    assert(obj.IsInt());
    assert(serializer.Serialize(obj, str2) == str1);

    obj = 123456789012345;
    serializer.Serialize(obj, str1); 
    cout << str1 << endl;
    ret = unserializer.Unserialize(str1.c_str(), str1.size(), obj);
    assert(ret);
    assert(obj.IsInt());
    assert(serializer.Serialize(obj, str2) == str1);

    obj = 100.123456789; 
    serializer.Serialize(obj, str1); 
    cout << str1 << endl;
    ret = unserializer.Unserialize(str1.c_str(), str1.size(), obj);
    assert(ret);
    assert(obj.IsDouble());
    assert(serializer.Serialize(obj, str2) == str1);
}

void test_string()
{
    PHPSerializer serializer;
    PHPUnserializer unserializer;
    string str1, str2;
    bool ret = false;

    Object obj("abcde");
    serializer.Serialize(obj, str1); 
    cout << str1 << endl;
    ret = unserializer.Unserialize(str1.c_str(), str1.size(), obj);
    assert(ret);
    assert(obj.IsString());
    assert(serializer.Serialize(obj, str2) == str1);

    obj = "中文测试-张华-好搜"; 
    serializer.Serialize(obj, str1); 
    cout << str1 << endl;
    ret = unserializer.Unserialize(str1.c_str(), str1.size(), obj);
    assert(ret);
    assert(obj.IsString());
    assert(serializer.Serialize(obj, str2) == str1);

    obj = "特殊字符测试: \"d:;\";;;;;:::::::,,,,,,,,,,,"; 
    serializer.Serialize(obj, str1); 
    cout << str1 << endl;
    ret = unserializer.Unserialize(str1.c_str(), str1.size(), obj);
    assert(ret);
    assert(obj.IsString());
    assert(serializer.Serialize(obj, str2) == str1);
}

void test_array1()
{
    PHPSerializer serializer;
    PHPUnserializer unserializer;
    string str1, str2;
    bool ret = false;

    Object array(Object::vector());
    for (int i = 0; i < 10; ++i) {
        array.push_back(i);
    }
    array.erase(9);
    array.erase(8);
    array.erase(7);

    serializer.Serialize(array, str1);
    cout << str1 << endl;

    ret = unserializer.Unserialize(str1.c_str(), str1.size(), array);
    assert(ret);
    assert(array.IsArray());
    assert(serializer.Serialize(array, str2) == str1);
}

void test_array2()
{
    PHPSerializer serializer;
    PHPUnserializer unserializer;
    string str1, str2;
    bool ret = false;

    Object array(Object::vector());
    for (int i = 0; i < 10; ++i) {
        array.push_back("abcde");
    }
    array.erase(array.begin(), array.end());
    for (int i = 0; i < 5; ++i) {
        array.push_back("abcde");
    }

    serializer.Serialize(array, str1);
    cout << str1 << endl;

    ret = unserializer.Unserialize(str1.c_str(), str1.size(), array);
    assert(ret);
    assert(array.IsArray());
    assert(serializer.Serialize(array, str2) == str1);
}

void test_array3()
{
    PHPSerializer serializer;
    PHPUnserializer unserializer;
    string str1, str2;
    bool ret = false;

    Object array(Object::vector());
    Object array1(Object::vector());
    Object array2(Object::vector());

    array1.push_back(100);
    array1.push_back(100.1);
    array1.push_back(true);
    array1.push_back(false);
    array1.push_back("hello kitty");
    array1.push_back("张桐语");
    array1.push_back(Object::none());

    array2.push_back(100);
    array2.push_back(100.1);
    array2.push_back(true);
    array2.push_back(false);
    array2.push_back(Object::none());

    array.push_back(array1);
    array.push_back(array2);

    serializer.Serialize(array, str1);
    cout << str1 << endl;

    ret = unserializer.Unserialize(str1.c_str(), str1.size(), array);
    assert(ret);
    assert(array.IsArray());
    assert(serializer.Serialize(array, str2) == str1);
}

void test_array4()
{
    PHPSerializer serializer;
    PHPUnserializer unserializer;
    string str1, str2;
    bool ret = false;

    Object map = Object::map();
    for (int i = 10; i > 0; --i) {
        char key[100];
        sprintf(key, "key-%d", i);
        map.insert(key, i);
    }

    serializer.Serialize(map, str1);
    cout << str1 << endl;

    ret = unserializer.Unserialize(str1.c_str(), str1.size(), map);
    assert(ret);
    assert(map.IsArray());
    assert(serializer.Serialize(map, str2) == str1);
}

void test_array5()
{
    PHPSerializer serializer;
    PHPUnserializer unserializer;
    string str1, str2;
    bool ret = false;

    Object map = Object::map();
    for (int i = 0; i < 10; ++i) {
        char key[100];
        char value[100];
        sprintf(key, "key-%d", i);
        sprintf(value, "value-%d", i);
        map.insert(key, value);
    }

    serializer.Serialize(map, str1);
    cout << str1 << endl;

    ret = unserializer.Unserialize(str1.c_str(), str1.size(), map);
    assert(ret);
    assert(map.IsArray());
    assert(serializer.Serialize(map, str2) == str1);
}

void test_array6()
{
    PHPSerializer serializer;
    PHPUnserializer unserializer;
    string str1, str2;
    bool ret = false;

    Object map = Object::map();
    map.insert("from", "engine");
    map.insert("none", Object::none());

    Object array(Object::vector());
    for (int i = 0; i < 10; ++i) {
        array.push_back("abcd");
    }
    map.insert("results", array);

    Object params = Object::map();
    params.insert("key1", "value1");
    params.insert("key2", "value2");
    params.insert("key3", "value3");
    params.insert("key4", "value4");
    params.insert("int32", 10000);
    params.insert("int64", 111111111111111);
    params.insert("double", 12344.124);

    Object param_array(Object::vector());
    for (int i = 0; i < 10; ++i) {
        param_array.push_back("abcde");
    }
    params.insert("param_array", param_array);

    Object param_map = Object::map();
    param_map.insert("map-key-1", "map-value-1");
    param_map.insert("map-key-2", "map-value-2");
    param_map.insert("map-key-3", "map-value-3");
    param_map.insert("map-key-4", "map-value-4");
    param_map.insert("map-key-5", "map-value-5");
    param_map.insert("map-key-6", "map-value-6");
    params.insert("param_map", param_map);

    map.insert("params", params);

    serializer.Serialize(map, str1);
    cout << str1 << endl;

    ret = unserializer.Unserialize(str1.c_str(), str1.size(), map);
    assert(ret);
    assert(map.IsArray());
    assert(serializer.Serialize(map, str2) == str1);
}

void test_operator()
{
    Object obj1;
    Object obj2;

    obj1 = true;
    obj2 = true;
    assert(obj1 == obj2);
    assert(obj1 == true);
    assert(obj2 == true);
    assert(obj1 != false);
    assert(obj2 != false);

    obj1 = false;
    obj2 = false;
    assert(obj1 == obj2);
    assert(obj1 == false);
    assert(obj2 == false);
    assert(obj1 != true);
    assert(obj2 != true);

    obj1 = 12345678;
    obj2 = 12345678;
    assert(obj1 == obj2);
    assert(obj1 == 12345678);
    assert(obj2 == 12345678);
    assert(obj1 != 0);
    assert(obj2 != 0);
    
    obj1 = 12345678.4321;
    obj2 = 12345678.4321;
    assert(obj1 == obj2);
    assert(obj1 == 12345678.4321);
    assert(obj2 == 12345678.4321);
    assert(obj1 != 0);
    assert(obj2 != 0);

    obj1 = 12345678.87654321;
    obj2 = 12345678.87654;
    assert(obj1 == obj2);

    obj1 = "kitty";
    obj2 = "kitty";
    assert(obj1 == obj2);
    assert(obj1 == "kitty");
    assert(obj2 == "kitty");
    assert(obj1 != "");
    assert(obj2 != "");

    obj1 = "张桐语";
    obj2 = "张桐语";
    assert(obj1 == obj2);

    obj1 = Object::vector();
    obj1[0] = 1;
    obj1[1] = true;
    obj1[2] = false;
    obj1[3] = "hello";
    obj1[4] = 123.123;
    obj2 = Object::vector();
    obj2.push_back(1);
    obj2.push_back(true);
    obj2.push_back(false);
    obj2.push_back("hello");
    obj2.push_back(123.123);
    assert(obj1 == obj2);
}

void test_exception()
{
    string str;
    PHPUnserializer unserializer;
    Object obj;

    str = "s:300000000:\"124";
    assert(!unserializer.Unserialize(str.c_str(), str.size(), obj));

    str = "s:2:\"124\";";
    assert(!unserializer.Unserialize(str.c_str(), str.size(), obj));
}

void test_AddItem(Object& root)
{
    root = Object::map();

#if 1
    root.insert("errno", 0);
    root.insert("count", 20);
    root.insert("total", 321023456781);
    root.insert("consume", 120.6);
    root.insert("from", "engine");
    root.insert("type", "wenda");
    root.insert("is_engine", true);
#else
    root["errno"] = 0;
    root["count"] = 20;
    root["total"] = 321023456781;
    root["consume"] = 120.6;
    root["from"] = "engine";
    root["type"] = "wenda";
    root["is_engine"] = true;
#endif

#if 1
    root.insert("items", Object::vector());
    Object& items = root.at("items");
    for (int i = 0; i < 10; ++i) {
        items.push_back(Object::map());
        Object& back = items.back();
        back.insert("title", "中文标题测试");
        back.insert("summary", "中文摘要测试");
        back.insert("url", "http://www.haosou.com/");
    }
#else
    Object items = Object::vector();

    for (int i = 0; i < 10; ++i) {
        Object item = Object::map();
        item.insert("title", "中文标题测试");
        item.insert("summary", "中文摘要测试");
        item.insert("url", "http://www.haosou.com/");
        items.push_back(item);
    }
    root.insert("items", items);
#endif

#if 0
    boost::shared_ptr<Object> test(new Object);
    root->AddItem("test", test);
    test->AddItem(true);
    test->AddItem(false);
    test->AddItem(1);
    test->AddItem(0);
    test->AddItem(10000.123456789);
    test->AddItem("hello kitty");
    test->AddItem("hello kitty 张桐语");
#endif
}

void test_GetItem(Object& root)
{
    assert(root.IsArray());

    Object& obj_count = root.at("count");
    assert(obj_count.IsInt());
    assert(obj_count.GetInt() == 20);
    assert(obj_count == 20);
    obj_count = 30;
    assert(obj_count == 30);

    Object& obj_from = root.at("from");
    assert(obj_from.IsString());
    assert(obj_from.GetString() == "engine");
    assert(obj_from == "engine");

    Object& obj_is_engine = root.at("is_engine");
    assert(obj_is_engine.IsBool());
    assert(obj_is_engine.GetBool() == true);
    assert(obj_is_engine == true);

    Object& obj_items = root.at("items");
    assert(obj_items.IsArray());
    assert(obj_items.size() == 10);

    for (size_t i = 0; i < obj_items.size(); ++i) {
        Object& obj_item = obj_items.at((int64_t)i);
        assert(obj_item.IsArray());

        Object& obj_title = obj_item.at("title");
        assert(obj_title.IsString());
        assert(obj_title.GetString() == "中文标题测试");
        assert(obj_title == "中文标题测试");
    }
}

uint64_t get_monotonic_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);                                                                                                                                                                                                          
    return (tv.tv_sec * 1000000 + tv.tv_usec);
}

void test_stress(int loop_count)
{
    uint64_t start, end;
    uint64_t serialize_total = 0;
    uint64_t unserialize_total = 0;
    uint64_t getitem_total = 0;
    uint64_t additem_total = 0;

    //ProfilerStart("PHPSerializeProfile"); 

    int percent = 0;
    string progress;
    for (int i = 0; i <= loop_count; ++i) {
        string output;
        Object root;

        start = get_monotonic_time();
        test_AddItem(root);
        end = get_monotonic_time();
        additem_total += (end - start);
        assert(root.IsArray());

#if 1
        PHPSerializer serializer;
        start = get_monotonic_time();
        serializer.Serialize(root, output);
        end = get_monotonic_time();
        serialize_total += (end - start);

        PHPUnserializer unserializer;
        Object obj;
        start = get_monotonic_time();
        bool ret = unserializer.Unserialize(output.c_str(), output.size(), obj);
        end = get_monotonic_time();
        unserialize_total += (end - start);
        assert(ret);
        assert(obj.IsArray());

        start = get_monotonic_time();
        test_GetItem(root);
        end = get_monotonic_time();
        getitem_total += (end - start);
#endif

        if ((i % (loop_count / 100)) == 0) {
            //printf("progress: \033[41m\033[1m %d%% %s\r\033[0m", percent++, progress.c_str());
            printf("\033[?25l\033[31m\033[1m%s[%-100s] %d%%\033[?25h\033[0m\r", 
                    "性能测试 ", progress.c_str(), percent++);
            fflush(stdout);
            progress += "#";
        }
    }
    //ProfilerStop();

    double serialize_elapsed = serialize_total/1e6;
    double unserialize_elapsed = unserialize_total/1e6;
    double getitem_elapsed = getitem_total/1e6;
    double additem_elapsed = additem_total/1e6;

    printf("\n");
    printf("      Times: %d\n", loop_count);
    printf("    AddItem: total=%f per=%f qps=%f\n", 
            additem_total/1e6, additem_elapsed/loop_count, 1/(additem_elapsed/loop_count));
    printf("  Serialize: total=%f per=%f qps=%f\n", 
            serialize_total/1e6, serialize_elapsed/loop_count, 1/(serialize_elapsed/loop_count));
    printf("Unserialize: total=%f per=%f qps=%f\n", 
            unserialize_total/1e6, unserialize_elapsed/loop_count, 1/(unserialize_elapsed/loop_count));
    printf("    GetItem: total=%f per=%f qps=%f\n", 
            getitem_total/1e6, getitem_elapsed/loop_count, 1/(getitem_elapsed/loop_count));
}

void test_vector_merge()
{
    bool ret = false;

    Object array1(Object::vector());
    Object array2(Object::vector());
    for (int i = 0; i < 3; ++i) {
        array1.push_back(i);
        array2.push_back(i + 3);
    }

    ArrayMerge(array1, array2);

    for (int i = 0; i < 6; i++) {
        assert(array1[i] == i);
        cout << array1[i].GetInt() << endl;
    }

    PHPSerializer serializer;
    PHPUnserializer unserializer;
    std::string str1;
    std::string str2;
    serializer.Serialize(array1, str1);
    cout << str1 << endl;

    Object array;
    ret = unserializer.Unserialize(str1.c_str(), str1.size(), array);
    assert(ret);
    assert(array.IsArray());
    assert(serializer.Serialize(array, str2) == str1);
}

void test_map_merge()
{
    Object array1(Object::map());
    Object array2(Object::map());
    for (int i = 0; i < 3; ++i) {
        array1.insert(i, i);
        array2.insert(i, i + 3);
    }

    ArrayMerge(array1, array2);

    for (int i = 0; i < 3; i++) {
        //assert(array1[i].key() == i);
        assert(array1[i] == i + 3);
        cout << i << ":" << array1[i].GetInt() << endl;
    }

    Object array3(Object::map());
    array3.insert("a", "aaa");
    array3.insert("b", "bbb");

    ArrayMerge(array1, array3);

    Object::iterator it = array1.begin();
    for (; it != array1.end(); it++) {
        if (it.key().IsInt()) {
            cout << "key: " << it.key().GetInt() << ", value: " << it.value().GetInt() << endl;
        } else {
            cout << "key: " << it.key().GetString() << ", value: " << it.value().GetString() << endl;
        }
    }

    PHPSerializer serializer;
    PHPUnserializer unserializer;
    std::string str1;
    std::string str2;
    serializer.Serialize(array1, str1);
    cout << str1 << endl;

    Object array;
    bool ret = unserializer.Unserialize(str1.c_str(), str1.size(), array);
    assert(ret);
    assert(array.IsArray());
    assert(serializer.Serialize(array, str2) == str1);
    return;
}

void test_string_map_merge()
{
    Object array1(Object::map());
    Object array2(Object::map());

    array1.insert("a", "aaa");
    array1.insert("b", "aaa");
    array2.insert("a", "ccc");
    array2.insert("c", "ddd");

    ArrayMerge(array1, array2);

    Object::iterator it = array1.begin();
    for (; it != array1.end(); it++) {
        cout << "key: " << it.key().GetString() << ", value: " << it.value().GetString() << endl;
    }

    PHPSerializer serializer;
    PHPUnserializer unserializer;
    std::string str1;
    std::string str2;
    serializer.Serialize(array1, str1);
    cout << str1 << endl;

    Object array;
    bool ret = unserializer.Unserialize(str1.c_str(), str1.size(), array);
    assert(ret);
    assert(array.IsArray());
    assert(serializer.Serialize(array, str2) == str1);

    return;
}

void test_array_merge()
{
    test_vector_merge();
    test_map_merge();
    test_string_map_merge();
}

int test_serializer_php()
{
    test_object();
    test_null();
    test_bool();
    test_number();
    test_string();
    test_array1();
    test_array2();
    test_array3();
    test_array4();
    test_array5();
    test_array6();
    test_operator();
    test_exception();
    //test_stress(loop_count);
    test_array_merge();

    return 0;
}

} // namespace serializer_php

