/***************************************************
** @file: mcStr.h
** @author: Mr.Chen
** @date: 2016/7/29
** @brief: ¶¯Ì¬×Ö·û´®
***************************************************/
#ifndef __MAPTRIXC_STR_LIB__
#define __MAPTRIXC_STR_LIB__

#include "type.h"
#include "mcMTL.h"
#include <string.h>

namespace MTL
{

template <class _T>
class mcStrT
{
public:
	mcStrT();
	mcStrT(const mcStrT& Src);
	mcStrT(const _T *Data);

	mcStrT& operator =(const mcStrT& Src);
	mcStrT& operator =(const _T *Data);

	mcStrT& operator +=(const mcStrT& Src);
	mcStrT& operator +=(const _T *Data);
	mcStrT& operator +=(_T ch);

	friend mcStrT operator + _NULL_ARGS_ (const mcStrT& s1, const mcStrT& s2);
	friend mcStrT operator + _NULL_ARGS_ (const mcStrT& s1, _T ch);
	friend mcStrT operator + _NULL_ARGS_ (_T ch, const mcStrT& s2);
	friend mcStrT operator + _NULL_ARGS_ (const mcStrT& s1, const _T *s2);
	friend mcStrT operator + _NULL_ARGS_ (const _T *s1, const mcStrT& s2);

	operator const _T*() const;

public:
	int  Compare(const _T *Data) const;
	int  Find(const _T ch, int nPos = 0) const;
	int  Find(const _T *substr, int nPos = 0) const;
	int  ReverseFind(const _T ch) const;
	int  GetLength() const;

	void MakeUpper();
	void MakeLower();
	void MakeReverse();

	void Append(const _T *Data, int nLen);

	mcStrT Mid(int nFirst, int nCount) const;
	mcStrT Mid(int nFirst) const;
	mcStrT Left(int nCount) const;
	mcStrT Right(int nCount) const;

	const _T *GetBuffer();

public:
	~mcStrT();

protected:
	static _T *GetEmptyStr();

	bool AllocBuffer(int nLens);
	bool AllocBeforeWrite(int nLens);
	void CopyBeforeWrite();
	void AllocCopy(mcStrT& dest, int nCopyLen, int nCopyIndex) const;

	void Concatenate(const _T *src1, int len1, const _T *src2, int len2);
	void Release();

	static void Release(void *pData);

protected:
	_T *m_pData;
};

template <class _T> bool operator ==(const mcStrT<_T>& s1, const mcStrT<_T>& s2);
template <class _T> bool operator ==(const mcStrT<_T>& s1, const _T* s2);
template <class _T> bool operator ==(const _T* s1, const mcStrT<_T>& s2);
template <class _T> bool operator !=(const mcStrT<_T>& s1, const mcStrT<_T>& s2);
template <class _T> bool operator !=(const mcStrT<_T>& s1, const _T* s2);
template <class _T> bool operator !=(const _T* s1, const mcStrT<_T>& s2);
template <class _T> bool operator <(const mcStrT<_T>& s1, const mcStrT<_T>& s2);
template <class _T> bool operator <(const mcStrT<_T>& s1, const _T* s2);
template <class _T> bool operator <(const _T* s1, const mcStrT<_T>& s2);
template <class _T> bool operator >(const mcStrT<_T>& s1, const mcStrT<_T>& s2);
template <class _T> bool operator >(const mcStrT<_T>& s1, const _T* s2);
template <class _T> bool operator >(const _T* s1, const mcStrT<_T>& s2);
template <class _T> bool operator <=(const mcStrT<_T>& s1, const mcStrT<_T>& s2);
template <class _T> bool operator <=(const mcStrT<_T>& s1, const _T* s2);
template <class _T> bool operator <=(const _T* s1, const mcStrT<_T>& s2);
template <class _T> bool operator >=(const mcStrT<_T>& s1, const mcStrT<_T>& s2);
template <class _T> bool operator >=(const mcStrT<_T>& s1, const _T* s2);
template <class _T> bool operator >=(const _T* s1, const mcStrT<_T>& s2);

typedef mcStrT<char> mcStrA;
typedef mcStrT<WORD> mcStrW;

#if defined UNICODE || defined _UNICODE
	typedef mcStrW mcStr;
#else
	typedef mcStrA mcStr;
#endif

/////////////////////////////////////////////////////////////////////////////
//
// mcStrT
//
/////////////////////////////////////////////////////////////////////////////
#include "mcStd.h"

#define GetRefs(Data) ((int *)Data)[-3]
#define GetLens(Data) ((int *)Data)[-2]
#define GetSize(Data) ((int *)Data)[-1]
#define GetData(Data) &GetRefs(Data)

template <class _T>
inline mcStrT<_T>::mcStrT()
{
	m_pData = GetEmptyStr();
}

template <class _T>
mcStrT<_T>::mcStrT(const mcStrT& Src)
{
	m_pData = Src.m_pData;
	if (GetRefs(m_pData) >= 0)
		GetRefs(m_pData)++;
}

template <class _T>
mcStrT<_T>::mcStrT(const _T *Data)
{
	if (Data != NULL && Data[0] != 0)
	{
		int n = MTL::strlen(Data);
		if (AllocBuffer(n))
		{
			memcpy(m_pData, Data, n * sizeof(_T));
			return;
		}
	}
	m_pData = GetEmptyStr();
}

template <class _T>
mcStrT<_T>& mcStrT<_T>::operator =(const mcStrT& Src)
{
	if (Src.m_pData != m_pData)
	{
		Release();

		m_pData = Src.m_pData;
		if (GetRefs(m_pData) >= 0)
			GetRefs(m_pData)++;
	}
	return *this;
}

template <class _T>
mcStrT<_T>& mcStrT<_T>::operator =(const _T *Data)
{
	if (Data != NULL && Data[0] != 0)
	{
		int n = MTL::strlen(Data);
		if (AllocBeforeWrite(n))
		{
			memcpy(m_pData, Data, n * sizeof(_T));
			return *this;
		}
	}
	Release();
	return *this;
}

template <class _T>
mcStrT<_T>& mcStrT<_T>::operator +=(const mcStrT& Src)
{
	Append(Src, GetLens(Src.m_pData));
	return *this;
}

template <class _T>
mcStrT<_T>& mcStrT<_T>::operator +=(const _T *Data)
{
	Append(Data, MTL::safestrlen(Data));
	return *this;
}

template <class _T>
mcStrT<_T>& mcStrT<_T>::operator +=(_T ch)
{
	Append(&ch, 1);
	return *this;
}

template <class _T>
mcStrT<_T> operator +(const mcStrT<_T>& s1, const mcStrT<_T>& s2)
{
	mcStrT<_T> s;
	s.Concatenate(s1, GetLens(s1.m_pData), s2, GetLens(s2.m_pData));
	return s;
}

template <class _T>
mcStrT<_T> operator +(const mcStrT<_T>& s1, _T ch)
{
	mcStrT<_T> s;
	s.Concatenate(s1, GetLens(s1.m_pData), &ch, 1);
	return s;
}

template <class _T>
mcStrT<_T> operator +(_T ch, const mcStrT<_T>& s2)
{
	mcStrT<_T> s;
	s.Concatenate(&ch, 1, s2, GetLens(s2.m_pData));
	return s;
}

template <class _T>
mcStrT<_T> operator +(const mcStrT<_T>& s1, const _T *s2)
{
	mcStrT<_T> s;
	s.Concatenate(s1, GetLens(s1.m_pData), s2, MTL::safestrlen(s2));
	return s;
}

template <class _T>
mcStrT<_T> operator +(const _T *s1, const mcStrT<_T>& s2)
{
	mcStrT<_T> s;
	s.Concatenate(s1, MTL::safestrlen(s1), s2, GetLens(s2.m_pData));
	return s;
}

template <class _T>
inline mcStrT<_T>::operator const _T*() const
{
	return m_pData;
}

template <class _T>
inline int mcStrT<_T>::Compare(const _T *Data) const
{
	return m_pData == Data ? 0 : MTL::strcmp(m_pData, Data);
}

template <class _T>
int mcStrT<_T>::Find(const _T ch, int nPos) const
{
	if (nPos < 0 || GetLens(m_pData) <= nPos)
		return -1;

	_T *Temp = MTL::strchr(m_pData + nPos, ch);
	return Temp == NULL ? -1 : Temp - m_pData;
}

template <class _T>
int mcStrT<_T>::Find(const _T *substr, int nPos) const
{
	if (nPos < 0 || GetLens(m_pData) <= nPos)
		return -1;

	_T *Temp = MTL::strstr(m_pData + nPos, substr);
	return Temp == NULL ? -1 : Temp - m_pData;
}

template <class _T>
int mcStrT<_T>::ReverseFind(const _T ch) const
{
	_T *Temp = MTL::strrchr(m_pData, ch);
	return Temp == NULL ? -1 : Temp - m_pData;
}

template <class _T>
inline int mcStrT<_T>::GetLength() const
{
	return GetLens(m_pData);
}

template <class _T>
void mcStrT<_T>::MakeUpper()
{
	if (GetRefs(m_pData) >= 0)
	{
		CopyBeforeWrite();
		MTL::upper(m_pData);
	}
}

template <class _T>
void mcStrT<_T>::MakeLower()
{
	if (GetRefs(m_pData) >= 0)
	{
		CopyBeforeWrite();
		MTL::lower(m_pData);
	}
}

template <class _T>
void mcStrT<_T>::MakeReverse()
{
	if (GetRefs(m_pData) >= 0)
	{
		CopyBeforeWrite();
		MTL::strrev(m_pData);
	}
}

template <class _T>
void mcStrT<_T>::Append(const _T *Data, int nLens)
{
	if (nLens == 0)
		return;

	int nOlds = GetLens(m_pData);
	int nNews = nLens + nOlds;

	if (GetRefs(m_pData) > 1 || GetSize(m_pData) <= nNews)
	{
		_T *pData = m_pData;
		Concatenate(m_pData, nOlds, Data, nLens);
		Release(pData);
	}
	else
	{
		memcpy(m_pData + nOlds, Data, nLens * sizeof(_T));
		GetLens(m_pData) = nNews;
		m_pData[nNews] = 0;
	}
}

template <class _T>
mcStrT<_T> mcStrT<_T>::Mid(int nFirst, int nCount) const
{
	if (nFirst < 0)
		nFirst = 0;
	if (nCount < 0)
		nCount = 0;

	if (nFirst + nCount > GetLens(m_pData))
		nCount = GetLens(m_pData) - nFirst;
	if (nFirst > GetLens(m_pData))
		nCount = 0;

	mcStrT dst;
	AllocCopy(dst, nCount, nFirst);
	return dst;
}

template <class _T>
inline mcStrT<_T> mcStrT<_T>::Mid(int nFirst) const
{
	return Mid(nFirst, GetLens(m_pData) - nFirst);
}

template <class _T>
mcStrT<_T> mcStrT<_T>::Left(int nCount) const
{
	mcStrT dst;

	if (0 < nCount && nCount <= GetLens(m_pData))
		AllocCopy(dst, nCount, 0);

	return dst;
}

template <class _T>
mcStrT<_T> mcStrT<_T>::Right(int nCount) const
{
	mcStrT dst;

	if (0 < nCount && nCount <= GetLens(m_pData))
		AllocCopy(dst, nCount, GetLens(m_pData) - nCount);
	return dst;
}

template <class _T>
inline const _T *mcStrT<_T>::GetBuffer()
{
	return m_pData;
}

template <class _T>
mcStrT<_T>::~mcStrT()
{
	if (GetRefs(m_pData) >= 0)
	{
		if (--GetRefs(m_pData) <= 0)
			delete (BYTE*)GetData(m_pData);
	}
}

template <class _T>
inline _T *mcStrT<_T>::GetEmptyStr()
{
	const static int InitData[] = { -1, 0, 0, 0 };
	return (_T *)&InitData[3];
}

template <class _T>
bool mcStrT<_T>::AllocBuffer(int nLens)
{
//	if (nLens == 0)
	if (0)
	{
		m_pData = GetEmptyStr();
	}
	else
	{
		int  nSize = (nLens + 0x40) & (~0x3F);	// 64×Ö½Ú¶ÔÆë

		int *pData = (int *)new BYTE[12 + nSize * sizeof(_T)];
		if (pData == NULL)
			return false;

		pData[0] = 1;
		pData[1] = nLens;
		pData[2] = nSize;

		m_pData = (_T *)&pData[3];
		m_pData[nLens] = 0;
	}
	return true;
}

template <class _T>
bool mcStrT<_T>::AllocBeforeWrite(int nLens)
{
	bool bRet = true;
	if (GetRefs(m_pData) > 1 || GetSize(m_pData) <= nLens)
	{
		Release();
		bRet = AllocBuffer(nLens);
	}
	else
	{
		GetLens(m_pData) = nLens;
		m_pData[nLens] = 0;
	}
	return bRet;
}

template <class _T>
void mcStrT<_T>::CopyBeforeWrite()
{
	if (GetRefs(m_pData) > 1)
	{
		GetRefs(m_pData)--;

		_T *pData = m_pData;
		if (AllocBuffer(GetLens(pData)))
			memcpy(m_pData, pData, GetLens(pData) * sizeof(_T));
		else
			m_pData = GetEmptyStr();
	}
}

template <class _T>
void mcStrT<_T>::AllocCopy(mcStrT& dst, int nCopyLen, int nCopyIndex) const
{
	if (nCopyLen > 0)
	{
		if (dst.AllocBuffer(nCopyLen))
			memcpy(dst.m_pData, m_pData + nCopyIndex, nCopyLen * sizeof(_T));
	}
}

template <class _T>
void mcStrT<_T>::Concatenate(const _T *src1, int len1, const _T *src2, int len2)
{
	int n = len1 + len2;
	if (n > 0)
	{
		if (AllocBuffer(n))
		{
			memcpy(m_pData, src1, len1 * sizeof(_T));
			memcpy(m_pData + len1, src2, len2 * sizeof(_T));
		}
	}
}

template <class _T>
void mcStrT<_T>::Release()
{
	if (GetRefs(m_pData) >= 0)
	{
		if (--GetRefs(m_pData) <= 0)
			delete (BYTE*)GetData(m_pData);
		m_pData = GetEmptyStr();
	}
}

template <class _T>
void mcStrT<_T>::Release(void *pData)
{
	if (GetRefs(pData) >= 0)
	{
		if (--GetRefs(pData) <= 0)
			delete (BYTE*)GetData(pData);
	}
}

#undef GetRefs
#undef GetLens
#undef GetSize
#undef GetData

/////////////////////////////////////////////////////////////////////////////
//
// mcStrT end
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
// mcStrT operator
//
/////////////////////////////////////////////////////////////////////////////

template <class _T>
inline bool operator ==(const mcStrT<_T>& s1, const mcStrT<_T>& s2)
{
	return s1.Compare(s2) == 0;
}

template <class _T>
inline bool operator ==(const mcStrT<_T>& s1, const _T* s2)
{
	return s1.Compare(s2) == 0;
}

template <class _T>
inline bool operator ==(const _T* s1, const mcStrT<_T>& s2)
{
	return s2.Compare(s1) == 0;
}

template <class _T>
inline bool operator !=(const mcStrT<_T>& s1, const mcStrT<_T>& s2)
{
	return s1.Compare(s2) != 0;
}

template <class _T>
inline bool operator !=(const mcStrT<_T>& s1, const _T* s2)
{
	return s1.Compare(s2) != 0;
}

template <class _T>
inline bool operator !=(const _T* s1, const mcStrT<_T>& s2)
{
	return s2.Compare(s1) != 0;
}

template <class _T>
inline bool operator <(const mcStrT<_T>& s1, const mcStrT<_T>& s2)
{
	return s1.Compare(s2) < 0;
}

template <class _T>
inline bool operator <(const mcStrT<_T>& s1, const _T* s2)
{
	return s1.Compare(s2) < 0;
}

template <class _T>
inline bool operator <(const _T* s1, const mcStrT<_T>& s2)
{
	return s2.Compare(s1) > 0;
}

template <class _T>
inline bool operator >(const mcStrT<_T>& s1, const mcStrT<_T>& s2)
{
	return s1.Compare(s2) > 0;
}

template <class _T>
inline bool operator >(const mcStrT<_T>& s1, const _T* s2)
{
	return s1.Compare(s2) > 0;
}

template <class _T>
inline bool operator >(const _T* s1, const mcStrT<_T>& s2)
{
	return s2.Compare(s1) < 0;
}

template <class _T>
inline bool operator <=(const mcStrT<_T>& s1, const mcStrT<_T>& s2)
{
	return s1.Compare(s2) <= 0;
}

template <class _T>
inline bool operator <=(const mcStrT<_T>& s1, const _T* s2)
{
	return s1.Compare(s2) <= 0;
}

template <class _T>
inline bool operator <=(const _T* s1, const mcStrT<_T>& s2)
{
	return s2.Compare(s1) >= 0;
}

template <class _T>
inline bool operator >=(const mcStrT<_T>& s1, const mcStrT<_T>& s2)
{
	return s1.Compare(s2) >= 0;
}

template <class _T>
inline bool operator >=(const mcStrT<_T>& s1, const _T* s2)
{
	return s1.Compare(s2) >= 0;
}

template <class _T>
inline bool operator >=(const _T* s1, const mcStrT<_T>& s2)
{
	return s2.Compare(s1) <= 0;
}

/////////////////////////////////////////////////////////////////////////////
//
// mcStrT operator end
//
/////////////////////////////////////////////////////////////////////////////

} // namespace MTL


#endif   // __MAPTRIXC_STR_LIB__
