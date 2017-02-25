/**********************************
** @file: uncopyable.h
** @brief:  
** @autor: by Mr.Chen
** @date: 2016/07/30 1:07
**********************************/
#ifndef _UNCOPYABLE_H
#define _UNCOPYABLE_H

namespace sys_util
{
    class Uncopyable
    {
    protected:
        Uncopyable() {}
        virtual ~Uncopyable() {}

    private:
        Uncopyable( const Uncopyable &rhs );
        Uncopyable &operator = ( const Uncopyable &rhs );
    };

} // namespace sys_util


#endif // _UNCOPYABLE_H
