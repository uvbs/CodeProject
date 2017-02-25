////////////////////////////////////////////////////////////////////////////////////
// 
// @file: bitflags.h
// @author: by Mr.Chen
// @date: 2015/4/28	12:56
// @brief: 位集合
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __BIT_FLAGS_H_
#define __BIT_FLAGS_H_

#include "type.h"

//用于原子类型
template <class T>
struct BitFlags
{
	T _flags;
	BitFlags()
	{
		cleanUp();
	}
    BitFlags(T val)
    {
        _flags = val;
    }
	bool isSetBit(int bit) const
	{
		if (_flags & (1 << bit))
		{
			return true;
		}
		return false;
	}
	void updateBits(int bit, bool bUpdate)
	{
		if (bUpdate)
		{
			_flags |= (1 << bit);
		}
		else
		{
			_flags &= (~(1 << bit));
		}
	}
	void cleanUp()
	{
		_flags = 0;
	}
};

//特化win-64位
template <>
struct BitFlags<uint64>
{
    uint32 _uLow ;
    uint32 _uHigh ;
    BitFlags()
    {
        cleanUp( ) ;
    }
    BitFlags(uint64 val)
    {
        _uLow = val & 0x0FFFFFFFF;
        _uHigh = val >> 32;
    }
    bool isSetBit( int bit ) const
    {
        if (bit < 32)
        {
            if ( _uLow & (1<<bit) )
			{
				return true;
			}
        }
        else
        {
            if (_uHigh & (1<<(bit-32)))
			{
				return true;
			}
        }
        return false;
    }
    void updateBits(int bit, bool bUpdate)
    {
        if (bit < 32)
        {
            if (bUpdate)
			{
				_uLow |= (1 << bit);
			}
            else
			{
				_uLow &= (~(1 << bit));
			}
        }
        else
        {
            if (bUpdate)
			{
				_uHigh |= (1 << (bit - 32));
			}
            else
			{
				_uHigh &= (~(1<<(bit - 32)));
			}
        }
    }
    void cleanUp( )
    {
        _uHigh = 0;
        _uLow	=0;
    }
};

//特化win-64位
template <>
struct BitFlags<int64>
{
    int32	_uLow ;
    int32	_uHigh ;
    BitFlags()
    {
        cleanUp();
    }
    BitFlags(int64 val)
    {
        _uLow = val & 0x0FFFFFFFF;
        _uHigh = val >> 32;
    }
    bool isSetBit(int bit) const
    {
        if (bit < 32)
        {
            if (_uLow & (1 << bit))
			{
				return true;
			}
        }
        else
        {
            if (_uHigh & (1 << (bit - 32)))
			{
				return true;
			}
        }
        return false;
    }
    void updateBits(int bit, bool bUpdate)
    {
        if (bit < 32)
        {
            if (bUpdate)
			{
				_uLow |= (1 << bit);
			}
            else
			{
				_uLow &= (~(1 << bit));
			}
        }
        else
        {
            if (bUpdate)
			{
				_uHigh |= (1 << (bit - 32));
			}
            else
			{
				_uHigh &= (~(1 << (bit - 32)));
			}
        }
    }
    void cleanUp( )
    {
        _uHigh = 0;
        _uLow =0;
    }
};

#endif  //__BIT_FLAGS_H_

