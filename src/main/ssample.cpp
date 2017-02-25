////////////////////////////////////////////////////////////////////////////////////
// 
// @file: ssample.cpp
// @author: by Mr.Chen
// @date: 2015/4/16	14:27
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#include "ssample.h"
#include <stdarg.h>
#include <stdio.h>
#include "string_util.h"

/************************************************************************/
/*  �ɱ�����ĺ������                                   */
/************************************************************************/
void 
vParamFunc(char* dst, int dstLen, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vsnprintf(dst, dstLen, format, args);
    va_end(args);
}

void
sample_test1()
{
    char dest[100] = {0};
    vParamFunc(dest, 100, "%s-%d", "Hello World", 100);
}


/************************************************************************/
/*   ����new��delete , new[]��delete[]                                                              */
/************************************************************************/
void sample_test2()
{
    #define new new(__FILE__, __LINE__) 

	Base* p = NULL; 
	try 
	{ 
		//p = new Base; 
        //delete p;

		//p = new Derive;
		//delete p; 

        p = new Base[3];
        delete[] p;
	} 
	catch(...) 
	{
	} 
}

/************************************************************************/
/*  ������Ŀ                                   */
/************************************************************************/
//-------------------------------
//��1,2,....һֱ��n����������,�������㷨,����Ҫ��ʱ�临�Ӷ�ΪO(n),
//�ռ临�Ӷ�O(1),ʹ�ý���,����һ��ֻ�ܽ���������.����Ϊ��
int	SwapSort()
{
    int a[]  = {10,6,9,5,2,8,4,7,1,3};
    int len = sizeof(a) / sizeof(int);
    int temp;
    for(int i = 0; i < len; )
    {
        temp = a[a[i] - 1];
        a[a[i] - 1] = a[i];
        a[i] = temp;
        if ( a[i] == i + 1)
            i++;
    }
    for (int j = 0; j < len; j++)
        cout<<a[j]<<" ";
    cout<<endl;
    return 0;
}

//-------------------------------
//����һ������X[]����һ������Y[]���洢����X�ж�ӦԪ�ش�С��˳��ţ�����Y��ʼ��Ϊ1��
const int N=5;
void Order()
{
    int X[N] = {5,4,1,3,2};
    int Y[N] = {0};
    for(int i=0;i<N;i++) Y[i]=1;
    //for(i=0;i<N;i++)
    //{
    //	cout<<"X["<<i<<"]=";
    //	cin>>X[i];
    //}
    for(int i=0;i<N;i++)
    {
        for(int j=i+1;j<N;j++)
        {
            if(X[j]>X[i])
                Y[j]++;
            else if(X[j]<X[i])
                Y[i]++;
        }
    }
    for(int i=0;i<N;i++)
        cout<<Y[i]<<" ";
    cout<<endl;
}

//-----------------------------------
//sizeof����
void print(int a[])
{
    printf("%d\n",sizeof(a));
}

void size()
{
    int a[]={1,2,3,4};
    printf("%d\n",sizeof(a));
    print(a);
}

//-----------------------------------
//��long�����еĶ�����λ��1���ĸ���
int GetBitsCount(long a)
{
    int count=0,i=0;
    while(i<32)
    {
        if(a & 1<<i++) count++;
    }
    return count;
}

int GetBitsCount2(long a)
{
    int count=0;
    while(a)
    {
        if(a & 1) count++;
        a>>=1;
    }
    return count;
}

int GetBitsCount3(long a)
{
    int count=0;
    while(a)
    {
        a=a & (a-1);
        count++;
    }
    return count;
}

//--------------------------
//�ж�CPU�Ƿ�Ϊ���CPU
bool IsBigEndian()
{
    unsigned short test = 0x1122;
    if(*((unsigned char*)&test) == 0x11)
        return true;
    else
        return false;
}

int TestBigEndian()
{
    bool isBig = IsBigEndian();
    short test;

    FILE* fp;
    test = htons(0x3132); //(31ASIIC��ġ�1��,32ASIIC��ġ�2��)
    if ((fp = fopen ("c:\\test.txt", "wb")) == NULL)
    {
        return -1;
    }
    fwrite(&test, sizeof(short), 1, fp);
    fclose(fp);

    return 0;
}

void 
smaple_test3()
{
    SwapSort();
    Order();
    size();
    int count = GetBitsCount3(9);
    TestBigEndian();
}

char* test_func1()
{
    char s1[] = "hello hello!";
    return s1;
}

char* test_func2()
{
    char s2[] = "world world!";
    return s2;
}

void test_threadStack()
{
    char* s1 = test_func1();
    //printf("%s\n", s1);
    char* s2 = test_func2();
    //printf("%s\n", s1);
    printf("%s\n", s2);
}
