////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_algorithm.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 0:17
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "type.h"
#include "socketdef.h"
#include "test_algorithm.h"
#include "helper.h"
#include <string.h>
#include <iostream>
#include "sort.h"
#include "find.h"
#include "md5.h"
#include "mcSort.h"
#include "mcCRC.h"
#include "mcStr.h"
#include "mcMath.h"
#include "mcGBCode.h"
#include "mcHilbert.h"
#include "compress.h"

using std::cout;
using std::cout;
using std::endl;

void test_algorithm()
{
	//test find and sort.
    int a[] = {1, 2, 5, 7, 90, 40, 2000, 0, 100};
    int length = sizeof(a) / sizeof(int);
    output<int>(a, length);
    int k = find_kth<int>(a, 0, length - 1, 3);
    cout << k << endl;
    int sm = find_second_max<int>(a, length);
    cout << sm << endl;

	//test sort
    //sort_bubble_v3(a, length);
    //sort_insert(a, length);
    //sort_select(a, length);
    //sort_merge(a, length);
    //sort_heap<int>(a, length);
    //qsort_v2(a, 0, length-1);
    //sort_radix<4>(a, length);
	FastSort<int>(&a[0], &a[length-1]);
    output<int>(a, length);

	//test find
    int index = find_binary<int>(a, length, 40);
    cout << "find index= " << index << endl;
    index = find_binary<int>(a, length, 120);
    cout << "find index= " << index << endl;

	//test md5
    char str[] = "1328913394018403026f75eb3d0dd9a4c0fc80daa04ed99e39";
    MD5 encrypted_string(str, strlen(str));
    string m1 = encrypted_string.toString();
	cout << "md5 is " << m1 << endl;

    //MTL::mcMD5 md5;
    //md5.Starts();
    //md5.Update(reinterpret_cast<BYTE*>(str), ::strlen(str));
    //md5.Finish();
    //uint32 temp[4] = {0};
    //temp[0] = md5.m_nData[0];
    //temp[1] = md5.m_nData[1];
    //temp[2] = md5.m_nData[2];
    //temp[3] = md5.m_nData[3];
    //string m2 = str_util::bytesToHexString((BYTE*)&md5.m_nData[0], 16);
    //cout << m2 << endl;

    //test CRC
    char crc_str[] = "hello word, welcome to beijing!";
    if (uint32 ret = MTL::mcCRC::CRC32(reinterpret_cast<BYTE*>(crc_str), ::strlen(crc_str)))
    {
        cout << "CRC is: " << ret <<endl;
    }

    //test mcSTR
    MTL::mcStrT<char> mc_str = "hello world!";
    int pos = mc_str.Find("o");
    pos = mc_str.ReverseFind('o');
    pos = mc_str.Find("or");
    pos = mc_str.Find("dd");
    int len = mc_str.GetLength();
    mc_str += "welcome to beijing!";
    len = mc_str.GetLength();
    const char* buffer = mc_str.GetBuffer();
    cout << buffer << endl;
    mc_str.MakeUpper();
    buffer = mc_str.GetBuffer();
    cout << buffer << endl;


    //test mcMath
    int r = MTL::FastSqrt<int>(25);
    r = MTL::SimpSqrt(36);
    cout << r <<endl;
    r = MTL::SimpCos<int>(PI_HALF);

    //test mcGBCode
    //GBK to UTF-16
    char* src = "高德地图MTL";
    int src_len = ::strlen(src);
    BYTE* gbk_src = reinterpret_cast<BYTE*>(src);
    int dest_len = src_len * 2;
    WORD* utf16_dest = new WORD[dest_len + 1];
    MTL::mcGBCode::GB18030ToUnicode(utf16_dest, dest_len, gbk_src);
    utf16_dest[dest_len] = 0;
    //UTF-16 to UTF-8
    dest_len = src_len * 3;
    BYTE* utf8_dest = new BYTE[dest_len + 1];
    MTL::mcGBCode::UnicodeToUTF8((BYTE*)utf8_dest, dest_len, utf16_dest); //vc中用s8看utf8编码字符
    utf8_dest[dest_len] = 0;
    //release resources
    SAFE_DELETE_ARRAY(utf16_dest);
    SAFE_DELETE_ARRAY(utf8_dest);

	//test mcHilert
	int x1 = 1000, y1 = 20000;
	int x2 = 1000, y2 = 20001;
	uint64 hilbert_code1 = MTL::mcHilbert<uint32, uint64>::Encode2(x1, y1, 28);
	uint64 hilbert_code2 = MTL::mcHilbert<uint32, uint64>::Encode2(x2, y2, 28);

    //test compress
    char orginal_data[] = "0000000000000000000000000000000000000000000";
    uLong data_len = strlen(orginal_data);
    // compress
    char* zip_data = new char[data_len];
    memset(zip_data, 0, data_len);
    uLong zip_len = data_len;
    gzcompress((Bytef*)orginal_data, data_len, (Bytef*)zip_data, &zip_len);

    // decompress()
    uLong unzip_len = zip_len * 4;
    char* unzip_data = new char[unzip_len];
    memset(unzip_data, 0, unzip_len);
    gzdecompress((byte*)zip_data, zip_len, (byte*)unzip_data, &unzip_len);


}
