//////////////////////////////////////////////////////////////////////////
// Copyright 2016 AMAP. All Rights Reserved.
// Author: Mr.CHEN
//
// 安全内存管理类
// 类似于C++ stlxx的 safe_ptr(auto_ptr已废弃)
// 简化不必要的操作,不做范围检查,用于临时分配且需要回收的
// 不可以赋值，但可以初始化
//
//////////////////////////////////////////////////////////////////////////
#ifndef _SAFE_PTR_H
#define _SAFE_PTR_H
#include "uncopyable.h"

namespace sys_util
{

    template < class T >
    class SafePtr : private Uncopyable
    {
    public:
        typedef T &reference;
        typedef const T &const_reference;

        //分配且赋值
        //explicit  SafePtr(size_t size,const T& val){
        //    px = new T[size];
        //    ////std::fill_n(px,size,val);
        //    for(size_t i=0;i<size;i++){
        //      px[i]=val;
        //    }
        //}
        explicit SafePtr( size_t size )
        {
            px = new T[size];
        }

        explicit SafePtr()
        {
            px = new T[1];
        }

        ~SafePtr()
        {
            if (px)
            {
                delete[] px;
                px = NULL;
            }
        }

        T &operator *()  // never throws
        {
            return *px;
        }

        const T &operator *() const // never throws
        {
            return *px;
        }

        T* operator ->() const // never throws
        {
            return px;
        }

        T &operator [] ( int index )
        {
            return px[index];
        }

        const T &operator [] ( int index ) const
        {
            return px[index];
        }

        const T* get() const // never throws
        {
            return px;
        }

        T* get()  // never throws
        {
            return px;
        }

    private:
        //禁止赋值
        //SafePtr <T>& operator = ( const SafePtr<T> & rhs) ;
        //禁止拷贝
        //SafePtr ( const SafePtr<T> & rhs) ;
        T* px;

    }; // class SafePtr

    template < class T, class U >
    bool operator == ( const SafePtr < T > &a, const SafePtr < U > &b )
    {
        return a.get() == b.get();
    }

    template < class T, class U >
    bool operator != ( const SafePtr < T > &a, const SafePtr < U > &b )
    {
        return a.get() != b.get();
    }

    template < class T, class U >
    bool operator < ( const SafePtr < T > &a, const SafePtr < U > &b )
    {
        return a.get() < b.get();
    }

    template < class T, class U >
    bool operator > ( const SafePtr < T > &a, const SafePtr < U > &b )
    {
        return a.get() > b.get();
    }

    template < class T, class U >
    bool operator >= ( const SafePtr < T > &a, const SafePtr < U > &b )
    {
        return a.get() >= b.get();
    }

    template < class T, class U >
    bool operator <= ( const SafePtr < T > &a, const SafePtr < U > &b )
    {
        return a.get() <= b.get();
    }

} // namespace sys_util


#endif // _SAFE_PTR_H
