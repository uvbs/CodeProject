////////////////////////////////////////////////////////////////////////////////////
// 
// @file: ssample.h
// @author: by Mr.Chen
// @date: 2015/4/16	14:27
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __SSAMPLE_H__
#define  __SSAMPLE_H__

#include <malloc.h>
#include <iostream>
using std::cout;
using std::endl;

/************************************************************************/
/*  可变参数的函数设计                                   */
/************************************************************************/
void vParamFunc(char* dst, int dstLen, const char* format, ...);
void  sample_test1();

/************************************************************************/
/*   重载new与delete , new[]与delete[]                                                              */
/************************************************************************/
struct Base 
{ 
    Base()
    { 
        //throw int(3); 
    }
    virtual ~Base() 
    {
        cout <<" ~Base ..."<<endl;
    }
    // new 与 delete
    void* operator new( size_t nSize, const char*,int) 
    { 
        //void* p = malloc( nSize ); 
        void* p = ::operator new( nSize );
        return p; 
    }
    void operator delete( void* p,const char*,int) 
    { 
        free(p);
    }
    void operator delete( void *p) 
    { 
    	::operator delete(p);
    	//free(p);
    }
    // new[] 与 delete[]
    void* operator new[]( size_t nSize, const char*,int) 
    {
        void* p = ::operator new[]( nSize );
        return p; 
    }
    void operator delete[]( void* p,const char*,int) 
    {
        free(p); 
    }
    void operator delete[]( void *p) 
    { 
        free(p);
    }
}; 

struct Derive : public Base 
{ 
	Derive() 
	{ 
	} 
	void* operator new( size_t nSize, const char*,int) 
	{
		// class Son 
		//void* p = malloc( nSize ); 
		void* p = ::operator new( nSize );
		return p; 
	} 
	void operator delete( void *p) 
	{ 
		// class Son 
		free(p); 
	}
	void operator delete( void* p,const char*,int) 
	{ 
		// class Son 
		free( p ); 
	} 
}; 

void sample_test2();

/************************************************************************/
/*  面试题目                                   */
/************************************************************************/
// 交换排序
int		SwapSort();
// 求数组元素大小序号
void	Order();
void	size();
// 取得long型数字中二进制位“1”的个数
int		GetBitsCount(long a);
int		GetBitsCount2(long a);
int		GetBitsCount3(long a);

// 判断CPU类型
bool	IsBigEndian();
int		TestBigEndian();
// 网络字节序
#define sw16(x) \
    ((short)( \
    (((short)(x) & (short)0x00ffU) << 8) | \
    (((short)(x) & (short)0xff00U) >> 8) ))

#define sw32(x) \
    ((long)( \
    (((long)(x) & (long)0x000000ff) << 24) | \
    (((long)(x) & (long)0x0000ff00) << 8) | \
    (((long)(x) & (long)0x00ff0000) >> 8) | \
    (((long)(x) & (long)0xff000000) >> 24) ))

#define htons(x) sw16(x)
#define htonl(x) sw32(x)
//#define htons(x) (x)
//#define htonl(x) (x)

void test_threadStack();

#endif
