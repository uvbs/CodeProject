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
/*  �ɱ�����ĺ������                                   */
/************************************************************************/
void vParamFunc(char* dst, int dstLen, const char* format, ...);
void  sample_test1();

/************************************************************************/
/*   ����new��delete , new[]��delete[]                                                              */
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
    // new �� delete
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
    // new[] �� delete[]
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
/*  ������Ŀ                                   */
/************************************************************************/
// ��������
int		SwapSort();
// ������Ԫ�ش�С���
void	Order();
void	size();
// ȡ��long�������ж�����λ��1���ĸ���
int		GetBitsCount(long a);
int		GetBitsCount2(long a);
int		GetBitsCount3(long a);

// �ж�CPU����
bool	IsBigEndian();
int		TestBigEndian();
// �����ֽ���
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
