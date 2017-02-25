/**********************************
** @file: mcHilbert.h
** @brief:  每天需要处理一批数据，可对待处理的数据进行排序，让地理位置相近的点处在队列的相邻位置。
	地理位置相邻是一个二维坐标，而处理队列的编号是一维。
	Hilbert Curve 就是一个很好的二维映射一维的曲线，维持了数据的局部性特征。
** @autor: by Mr.Chen
** @date: 2016/07/30 1:07
**********************************/
#ifndef __MAPTRIXC_HILBERT_LIB__
#define __MAPTRIXC_HILBERT_LIB__

#include "type.h"
#include "mcMTL.h"

namespace MTL
{

template <class TYPE, class HLBT = DWORD>
class mcHilbert
{
public:
	static HLBT Encode(TYPE *Pt, int n); 			// n = (x,y) bits length
	static HLBT Encode2(TYPE x, TYPE y, int n);
	static void Decode(TYPE *Pt, int n, HLBT s);
	static void Decode2(TYPE*x, TYPE*y, int n, HLBT s);
};

/////////////////////////////////////////////////////////////////////////////
//
// mcHilbert
//
/////////////////////////////////////////////////////////////////////////////
template <class TYPE, class HLBT>
HLBT mcHilbert<TYPE, HLBT>::Encode(TYPE *Pt, int n)
{
#if defined _DEBUG || defined DEBUG
	if (n > sizeof(HLBT) * 4)
	{
		// error.
	}
#endif

	HLBT s;
	TYPE t;

	TYPE X = Pt[0];
	TYPE Y = Pt[1];
	for(s = 0; n-- > 0;)
	{
		int x = (X >> n) & 1;
		int y = (Y >> n) & 1;

		if (y == 0)
		{
			t = X;
			X = Y ^ (-x);
			Y = t ^ (-x);
		}
		s = (s << 2) + (x << 1) + (x ^ y);
	} 
	return s;
} 

template <class TYPE, class HLBT>
void mcHilbert<TYPE, HLBT>::Decode(TYPE *Pt, int n, HLBT s)
{
#if defined _DEBUG || defined DEBUG
	if (n > sizeof(HLBT) * 4)
	{
		// error.
	}
#endif

	TYPE t, x, y, r;

	t = x = y = 0;
	for(n = (n << 1); (n -= 2) >= 0;)
	{
		r = (t << 2) | (TYPE)((s >> n) & 3);
		x = (x << 1) | (TYPE)((0x936C >> r) & 1);
		y = (y << 1) | (TYPE)((0x39C6 >> r) & 1);
		t = (0x3E6B94C1 >> (r << 1)) & 3;
	} 
	Pt[0] = x;
	Pt[1] = y;
}

/////////////////////////////////////////////////////////////////////////////
//
// BuildHilbert
//
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
template <class TYPE, class HLBT>
HLBT *BuildHilbert(TYPE **Objs, int nSize, int nCoor, int nBits)
{
	TYPE MinX, MinY, MaxX, MaxY;
	MinX = MinY = (TYPE)1E20;
	MaxX = MaxY = (TYPE)-1E20;

	int i;
	for(i = 0; i < nSize; i++)
	{
		TYPE *Coor = Objs[i];

		TYPE X, Y;
		if (nCoor == 2)
		{
			X = Coor[0];
			Y = Coor[1];
		}
		else
		{
			X = (Coor[0] + Coor[2]) / 2;
			Y = (Coor[1] + Coor[3]) / 2;
		}

		if (MinX > X)
			MinX = X;
		if (MinY > Y)
			MinY = Y;
		if (MaxX < X)
			MaxX = X;
		if (MaxY < Y)
			MaxY = Y;
	}

	TYPE Scale = (TYPE)cos((MinY + MaxY) * 0.00872664626);
	TYPE RectW = (MaxX - MinX) * Scale;
	TYPE RectH = (MaxY - MinY);
	TYPE Ratio;
	if (RectW > RectH)
		Ratio = (TYPE)((1 << nBits) - 1) / RectW;
	else
		Ratio = (TYPE)((1 << nBits) - 1) / RectH;

	HLBT *Hlbts = new HLBT[nSize];
	for(i = 0; i < nSize; i++)
	{
		TYPE *Coor = Objs[i];

		TYPE X, Y;
		if (nCoor == 2)
		{
			X = Coor[0];
			Y = Coor[1];
		}
		else
		{
			X = (Coor[0] + Coor[2]) / 2;
			Y = (Coor[1] + Coor[3]) / 2;
		}

		UINT Pt[2];
		Pt[0] = (UINT)((X - MinX) * Ratio * Scale);
		Pt[1] = (UINT)((Y - MinY) * Ratio);

		Hlbts[i] = mcHilbert<UINT, HLBT>::Encode(Pt, nBits);
	}
	return Hlbts;
}

template <class TYPE, class HLBT>
HLBT mcHilbert<TYPE, HLBT>::Encode2(TYPE x, TYPE y, int n)
{
#if defined _DEBUG || defined DEBUG
	if (n > sizeof(HLBT) * 4)
	{
		// error.
	}
#endif

	HLBT s = 0;
	TYPE t = 0;
	TYPE r;

	while(--n >= 0)
	{
		r = (t << 2) | 2 * ((x >> n) & 1) | (y >> n) & 1;
		s = (s << 2) | (0x361E9CB4 >> 2 * r) & 3;
		t = (0x8FE65831 >> 2 * r) & 3;
	}

	return s;
}

template <class TYPE, class HLBT>
void mcHilbert<TYPE, HLBT>::Decode2(TYPE*x, TYPE*y, int n, HLBT s)
{
#if defined _DEBUG || defined DEBUG
	if (n > sizeof(HLBT) * 4)
	{
		// error.
	}
#endif

	TYPE X = 0;
	TYPE Y = 0;
	TYPE t = 0;
	TYPE r;

	n = n * 2;
	while((n -= 2) >= 0)
	{
		r = (t << 2) | (s      >> n) & 3;
		X = (X << 1) | (0x936C >> r) & 1;
		Y = (Y << 1) | (0x39C6 >> r) & 1;
		t = (0x3E6B94C1 >> 2 * r) & 3;
	}
	*x = X;
	*y = Y;
}

/////////////////////////////////////////////////////////////////////////////
//
// mcHilbert end
//
/////////////////////////////////////////////////////////////////////////////


} // namespace MTL

#endif	// __MAPTRIXC_HILBERT_LIB__

