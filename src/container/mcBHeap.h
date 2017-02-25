#ifndef __MAPTRIXC_BINARYHEAP_LIB__
#define __MAPTRIXC_BINARYHEAP_LIB__
// MaptrixC Template Library

const int MAX_BINHEAP_SIZE = 1024;

#include "mcFuncDef.h"

template <class _KEY_, class _CMP_ = _DEF_LT_<_KEY_> >
class BinHeap
{
public:
	BinHeap();

	void Insert(_KEY_ K);
	void Update(int x);

	void Remove(int n);
	void RemoveAll();

	bool Verify();
	bool Verify(int n);

public:
	DWORD _Size;
	_KEY_ _List[MAX_BINHEAP_SIZE];
};


/////////////////////////////////////////////////////////////////////////////
//
// BinHeap
//
/////////////////////////////////////////////////////////////////////////////

template<class _KEY_, class _CMP_>
inline BinHeap<_KEY_, _CMP_>::BinHeap()
{
	_Size = 0;
	memset(_List,0,sizeof(_KEY_)*MAX_BINHEAP_SIZE);
}

template<class _KEY_, class _CMP_>
void BinHeap<_KEY_, _CMP_>::Insert(_KEY_ K)
{
#ifdef HEAP_PERFORMANCE
	int n, m;
	for(m = ++_Size; m > 1 && _CMP_()(K, _List[n = m >> 1]); m = n)
	{
		_List[m] = _List[n];
		_List[m]->nPos = m;
	}
	_List[m] = K;
	_List[m]->nPos = m;
#else
	int n, m;
	for(m = ++_Size; m > 1 && _CMP_()(K, _List[n = m >> 1]); m = n)
	{
		_List[m] = _List[n];
	}
	_List[m] = K;
#endif
}

template<class _KEY_, class _CMP_>
void BinHeap<_KEY_, _CMP_>::Update(int x)
{
	_KEY_ K = _List[x];

	int n, m;
	for(m = x; m > 1 && _CMP_()(K, _List[n = m >> 1]); m = n)
	{
		_List[m] = _List[n];
	}
	_List[m] = K;
}

template<class _KEY_, class _CMP_>
inline void BinHeap<_KEY_, _CMP_>::Remove(int n)
{
#ifdef HEAP_PERFORMANCE
	_KEY_ K = _List[_Size--];
	int m;
	for(m = n; (n = m << 1) < (int)_Size; m = n)
	{
#if 1
		n += _CMP_()(_List[n + 1], _List[n]);
#else
		int i = n++;
		if (_CMP_()(_List[i], _List[n]))
			n = i;
#endif
		_KEY_ T = _List[n];
		if (_CMP_()(T, K))
		{
			_List[m] = T;
			T->nPos = m;
		}
		else
		{
			_List[m] = K;
			K->nPos = m;
			return;
		}
	}
	if (n == (int)_Size)
	{
		_KEY_ T = _List[n];
		if (_CMP_()(T, K))
		{
			_List[m] = T;
			T->nPos = m;
			m = n;
		}
	}
	_List[m] = K;
	K->nPos = m;
#else
	_KEY_ K = _List[_Size--];
	int m;
	for(m = n; (n = m << 1) < (int)_Size; m = n)
	{
#if 1
		n += _CMP_()(_List[n + 1], _List[n]);
#else
		int i = n++;
		if (_CMP_()(_List[i], _List[n]))
			n = i;
#endif

		_KEY_ T = _List[n];
		if (_CMP_()(T, K))
			_List[m] = T;
		else
		{
			_List[m] = K;
			return;
		}
	}
	if (n == (int)_Size)
	{
		_KEY_ T = _List[n];
		if (_CMP_()(T, K))
		{
			_List[m] = T; m = n;
		}
	}
	_List[m] = K;
#endif
}

template<class _KEY_, class _CMP_>
inline void BinHeap<_KEY_, _CMP_>::RemoveAll()
{
	_Size = 0;
}

template<class _KEY_, class _CMP_>
bool BinHeap<_KEY_, _CMP_>::Verify()
{
	return Verify(1);
}

template<class _KEY_, class _CMP_>
bool BinHeap<_KEY_, _CMP_>::Verify(int n)
{
	int m = n << 1;
	if (m <= (int)_Size)
	{
		if (_CMP_()(_List[m], _List[n]))
			return false;
		if (!Verify(m))
			return false;
	}
	m++;
	if (m <= (int)_Size)
	{
		if (_CMP_()(_List[m], _List[n]))
			return false;
		if (!Verify(m))
			return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//
// BinHeap end
//
/////////////////////////////////////////////////////////////////////////////

#endif
