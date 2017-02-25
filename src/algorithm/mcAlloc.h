#ifndef __MAPTRIXC_ALLOCATE__
#define __MAPTRIXC_ALLOCATE__
// MaptrixC Template Library


template <class _T>
class mcAllocT
{
public:
	mcAllocT(int nGrow = 1024);
	~mcAllocT();

	void *Alloc(int&nSize, int nMin);
	void *Alloc(int nSize);
	void FreeAll();

protected:
	_T *GetNext();

protected:
	int m_nGrow;
	_T *m_pRoot;
	_T *m_pNext;
};

typedef mcAllocT<class mcVarBox> mcAlloc;

/////////////////////////////////////////////////////////////////////////////
//
// mcVarBox
//
/////////////////////////////////////////////////////////////////////////////

class mcVarBox
{
protected:
	typedef mcVarBox* LPBOX;

public:
	void MakeIt(int nFree)
	{
		m_pNext = NULL;
		m_nFree = nFree;
	}

	void *Alloc(int &nSize, int nMin)
	{
		if (m_nFree < nSize)
		{
			if (m_nFree >= nMin)
				nSize = m_nFree / nMin * nMin;
			else
			{
				m_nFree = 0;
				return NULL;
			}
		}
		return Data() + (m_nFree -= nSize);
	}

	void *Alloc(int nSize)
	{
		if (m_nFree < nSize)
		{
			m_nFree = 0;
			return NULL;
		}
		return Data() + (m_nFree -= nSize);
	}

	void FreeAll(int nFree)
	{
		m_nFree = nFree;
	}

protected:
	char *Data() { return (char *)(this + 1); }

public:
	LPBOX m_pNext;

protected:
	int   m_nFree;
};

/////////////////////////////////////////////////////////////////////////////
//
// mcVarBox class
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// mcAllocT class
//
/////////////////////////////////////////////////////////////////////////////

template <class _T>
mcAllocT<_T>::mcAllocT(int nGrow)
{
	m_nGrow = nGrow;

	m_pRoot = m_pNext = (_T *)new char[sizeof(_T) + nGrow];
	if (m_pRoot != NULL)
		m_pRoot->MakeIt(nGrow);
}

template <class _T>
mcAllocT<_T>::~mcAllocT()
{
	_T *pData, *pNext;
	pData = m_pRoot;
	while(pData)
	{
		pNext = pData->m_pNext;
		delete[] (char *)pData;
		pData = pNext;
	}
}

template <class _T>
void *mcAllocT<_T>::Alloc(int &nSize, int nMin)
{
	void *pData = m_pNext->Alloc(nSize, nMin);
	if (pData == NULL)
	{
		_T *pNext = GetNext();
		if (pNext != NULL)
			pData = pNext->Alloc(nSize, nMin);
	}
	return pData;
}

template <class _T>
void *mcAllocT<_T>::Alloc(int nSize)
{
	void *pData = m_pNext->Alloc(nSize);
	if (pData == NULL)
	{
		_T *pNext = GetNext();
		if (pNext != NULL)
			pData = pNext->Alloc(nSize);
	}
	return pData;
}

template <class _T>
void mcAllocT<_T>::FreeAll()
{
	_T *pNext;
	pNext = m_pNext = m_pRoot;
	while(pNext)
	{
		pNext->FreeAll(m_nGrow);
		pNext = pNext->m_pNext;
	}
}

template <class _T>
_T *mcAllocT<_T>::GetNext()
{
	_T *pNext = m_pNext->m_pNext;
	if (pNext == NULL)
	{
		pNext = (_T *)new char[sizeof(_T) + m_nGrow];
		if (pNext == NULL)
			return NULL;
		pNext->MakeIt(m_nGrow);
		m_pNext->m_pNext = pNext;
	}

	m_pNext = pNext;
	return pNext;
}

/////////////////////////////////////////////////////////////////////////////
//
// mcAllocT class end
//
/////////////////////////////////////////////////////////////////////////////

#endif
