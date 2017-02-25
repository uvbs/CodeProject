////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_math.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 1:01
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "macrodef.h"
#include "test_maths.h"
#include <string.h>
#include <stdio.h>
#include <set>
using std::set;
using std::cout;
using std::endl;

void test_maths()
{
    const int PREFIX_NUM = 3;				//首字母个数
    const int BIT_NUM = 18;				//随机字符串的位数
    const int MAX_RANDSTRING_NUM = 50000;	//最大随机串个数
    const char* split_char = "\t";
    const char* line_char = "\n";
    //前缀字符
    char prefix[PREFIX_NUM+1] = "mr-";
    //随机串位数
    int bit_num = 18;
    if (bit_num < BIT_NUM) { bit_num = BIT_NUM; }
    //随机串个数
    int rand_str_num = 1000;
    if (rand_str_num < 0 || rand_str_num > MAX_RANDSTRING_NUM) {
        rand_str_num = MAX_RANDSTRING_NUM;
    }
    std::string *a = new std::string[rand_str_num];
    std::set<std::string> rand_str_set;
    int same = 0;
    FILE* fp = fopen("data.txt", "w");
    for (int i = 0; i < rand_str_num; i++)
    {
        a[i] = get_randstr(bit_num - PREFIX_NUM, prefix, true, true, false);
        if (rand_str_set.find(a[i]) != rand_str_set.end()) { same++; }
        else {
            rand_str_set.insert(a[i]);
            //cout << a[i] << endl;
            //fwrite(a[i].c_str(), a[i].length(), 1, fp);
            //fwrite(splitChar, strlen(splitChar), 1, fp);
            //fwrite("0", strlen("0"), 1, fp);
            //fwrite(lineChar, strlen(lineChar), 1, fp);
        }
    }
    cout << "same=" << same << endl;
    cout << "randomStringNum=" << rand_str_set.size() << endl;

    SAFE_DELETE_ARRAY(a);
    fclose(fp);
    fp = NULL;

    //带权随机选择
    int occurrence[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int weight[2][10] = { 
        {100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000},
        {100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000}
    };

    for (int i = 0; i < 2; ++i)
    {
        memset(occurrence, 0, sizeof(occurrence));
        for (int j = 0; j < 1000000; ++j)
        {
            int index = select_rand_weight(weight[i], 10);
            occurrence[index]++;
        }
        for (int j = 0; j < 10; ++j)
        {
            std::cout << occurrence[j] << " ";
        }
        std::cout << endl;
    }
}
