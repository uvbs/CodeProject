#ifndef __MAPTRIXC_HASH_LIB__
#define __MAPTRIXC_HASH_LIB__
//	MaptrixC Template Library

//	下面的Hash对象适合如下情况
//	1.静态数量的数据
//	2.只是增加的数据
//	3.如果是反复执行增加删除操作
//	  使用#define _HASH_DYNAMIC_
//	4.如果需要遍历所有对象和VC接口一样
//	  使用#define _HASH_VC_ENUM_

#ifndef HASH_GROW_SIZE
#define HASH_GROW_SIZE 256
#endif

#ifndef HASH_INIT_SIZE
#define HASH_INIT_SIZE 256
#endif

#include "mcAlloc.h"
#include <string.h>
#include <stdlib.h>
#include "string_util.h"


template<class _KEY_, class _IDX_ = _KEY_>
class mcHashSet;
template<class _KEY_, class _VAL_>
class mcHashMap;
template<class _KEY_, class _VAL_>
class mcHashTab;

template <class _T>
class HashNum;
typedef HashNum<int>			HashInt;
typedef HashNum<int>			HashI32;
typedef HashNum<uint>		HashU32;
typedef HashNum<int64>		HashI64;
typedef HashNum<uint64>		HashU64;
typedef HashNum<void*>		HashPtr;
typedef HashNum<long>		HashL32;
class HashP64;
class HashBin;
class HashStr;

typedef mcHashMap<HashInt, void*>	mcHashInt;
typedef mcHashMap<HashI32, void*>	mcHashI32;
typedef mcHashMap<HashU32, void*>	mcHashU32;
typedef mcHashMap<HashI64, void*>	mcHashI64;
typedef mcHashMap<HashU64, void*>	mcHashU64;
typedef mcHashMap<HashPtr, void*>	mcHashPtr;
typedef mcHashMap<HashP64, void*>	mcHashP64;
typedef mcHashMap<HashBin, void*>	mcHashBin;
typedef mcHashMap<HashStr, void*>	mcHashStr;

#ifdef _HASH_VC_ENUM_
	#ifndef __AFX_H__
		#ifndef __POSITION_DEF__
		#define __POSITION_DEF__
		typedef void* POSITION;
		#endif
	#endif
#endif

/////////////////////////////////////////////////////////////////////////////
//
// mcHashSet
//
/////////////////////////////////////////////////////////////////////////////

template<class _KEY_, class _IDX_>
class mcHashSet : public mcAlloc
{
public:
	mcHashSet(int nGrow = HASH_GROW_SIZE, int nSize = HASH_INIT_SIZE);
	~mcHashSet();

	bool IsValid();
	bool InitHashSize(int nSize); // nSize必须是2 ^ n

	void Insert(_KEY_ Key);
	void Insert(_KEY_*Key);
	void Remove(_KEY_ Key);
	void RemoveAll();

	bool Lookup(_IDX_ Key, _KEY_ &Val);
	bool Lookup(_IDX_ Key);
	bool Lookup(_IDX_*Key, _KEY_ &Val);
	bool Lookup(_IDX_*Key);

	int  GetCount();

#ifdef _HASH_VC_ENUM_
	POSITION GetStartPosition() const;
	void GetNextAssoc(POSITION &NextPosition, _KEY_ &Key) const;
#endif

protected:
	class CNode
	{
	public:
		CNode *Nxt;
		_KEY_  Key;
//#ifdef _HASH_VC_ENUM_
//		uint   Pos;
//#endif
	};

protected:
	int    m_nHashSize;
	int    m_nHashMask;
	CNode**m_pHashBuff;

#ifdef _HASH_DYNAMIC_
	CNode *m_pHashFree;
#endif
};

template<class _KEY_, class _IDX_>
mcHashSet<_KEY_, _IDX_>::mcHashSet(int nGrow, int nSize) : mcAlloc(nGrow * sizeof(CNode))
{
	if (nSize & (nSize - 1))
		nSize = 256;

	m_nHashSize = 0;
	m_nHashMask = nSize - 1;
	m_pHashBuff = (CNode **)malloc(sizeof(CNode *) * nSize);
#ifdef _HASH_DYNAMIC_
	m_pHashFree = NULL;
#endif

	if (m_pHashBuff != NULL)
		memset(m_pHashBuff, 0, sizeof(CNode *) * nSize);
}

template<class _KEY_, class _IDX_>
mcHashSet<_KEY_, _IDX_>::~mcHashSet()
{
	if (m_pHashBuff)
		free(m_pHashBuff);
}

template<class _KEY_, class _IDX_>
bool mcHashSet<_KEY_, _IDX_>::IsValid()
{
	return m_pHashBuff != NULL;
}

template<class _KEY_, class _IDX_>
bool mcHashSet<_KEY_, _IDX_>::InitHashSize(int nSize)
{
	if (nSize & (nSize - 1))
		return false;

	m_nHashMask = nSize - 1;
	m_pHashBuff = (CNode **)realloc(m_pHashBuff, sizeof(CNode *) * nSize);
	RemoveAll();
	return true;
}

template<class _KEY_, class _IDX_>
void mcHashSet<_KEY_, _IDX_>::Insert(_KEY_ Key)
{
	uint nHash = Key.GetHashVal() & m_nHashMask;

	CNode *pItem;
#ifdef _HASH_DYNAMIC_
	if ((pItem = m_pHashFree) == NULL)
	{
		if ((pItem = (CNode *)Alloc(sizeof(CNode))) == NULL)
			return;
	}
	else
	{
		m_pHashFree = pItem->Nxt;
	}
#else
	{
		if ((pItem = (CNode *)Alloc(sizeof(CNode))) == NULL)
			return;
	}
#endif

//#ifdef _HASH_VC_ENUM_
//	pItem->Pos = nHash;
//#endif
	pItem->Key = Key;
	pItem->Nxt = m_pHashBuff[nHash];
	m_pHashBuff[nHash] = pItem;

	m_nHashSize++;
}

template<class _KEY_, class _IDX_>
void mcHashSet<_KEY_, _IDX_>::Insert(_KEY_ *Key)
{
	uint nHash = Key->GetHashVal() & m_nHashMask;

	CNode *pItem;
#ifdef _HASH_DYNAMIC_
	if ((pItem = m_pHashFree) == NULL)
	{
		if ((pItem = (CNode *)Alloc(sizeof(CNode))) == NULL)
			return;
	}
	else
	{
		m_pHashFree = pItem->Nxt;
	}
#else
	{
		if ((pItem = (CNode *)Alloc(sizeof(CNode))) == NULL)
			return;
	}
#endif

//#ifdef _HASH_VC_ENUM_
//	pItem->Pos = nHash;
//#endif
	pItem->Key = *Key;
	pItem->Nxt = m_pHashBuff[nHash];
	m_pHashBuff[nHash] = pItem;

	m_nHashSize++;
}

template<class _KEY_, class _IDX_>
void mcHashSet<_KEY_, _IDX_>::Remove(_KEY_ Key)
{
	if (m_pHashBuff == NULL)
		return;

	uint   nHash = Key.GetHashVal() & m_nHashMask;
	CNode**pPrev = &m_pHashBuff[nHash];
	CNode *pTemp;
	while((pTemp = *pPrev) != NULL)
	{
		if (Key == pTemp->Key)
		{
			*pPrev = pTemp->Nxt;

#ifdef _HASH_DYNAMIC_
			pTemp->Nxt  = m_pHashFree;
			m_pHashFree = pTemp;
#endif

			m_nHashSize--;
			return;
		}

		pPrev = &pTemp->Nxt;
	}
}

template<class _KEY_, class _IDX_>
void mcHashSet<_KEY_, _IDX_>::RemoveAll()
{
	FreeAll();

	m_nHashSize = 0;
#ifdef _HASH_DYNAMIC_
	m_pHashFree = NULL;
#endif
	if (m_pHashBuff != NULL)
		memset(m_pHashBuff, 0, sizeof(CNode *) * (m_nHashMask + 1));
}

template<class _KEY_, class _IDX_>
bool mcHashSet<_KEY_, _IDX_>::Lookup(_IDX_ Key, _KEY_ &Val)
{
#if defined _DEBUG || defined DEBUG	
	if (m_pHashBuff == NULL)
		return false;
#endif

	CNode *pItem = m_pHashBuff[Key.GetHashVal() & m_nHashMask];
	for(; pItem != NULL; pItem = pItem->Nxt)
	{
		if (pItem->Key == Key)
		{
			Val = pItem->Key;
			return true;
		}
	}

	return false;
}

template<class _KEY_, class _IDX_>
bool mcHashSet<_KEY_, _IDX_>::Lookup(_IDX_ Key)
{
#if defined _DEBUG || defined DEBUG	
	if (m_pHashBuff == NULL)
		return false;
#endif

	CNode *pItem = m_pHashBuff[Key.GetHashVal() & m_nHashMask];
	for(; pItem != NULL; pItem = pItem->Nxt)
	{
		if (pItem->Key == Key)
		{
			return true;
		}
	}

	return false;
}

template<class _KEY_, class _IDX_>
bool mcHashSet<_KEY_, _IDX_>::Lookup(_IDX_ *Key, _KEY_ &Val)
{
#if defined _DEBUG || defined DEBUG	
	if (m_pHashBuff == NULL)
		return false;
#endif

	CNode *pItem = m_pHashBuff[Key.GetHashVal() & m_nHashMask];
	for(; pItem != NULL; pItem = pItem->Nxt)
	{
		if (pItem->Key == *Key)
		{
			Val = pItem->Key;
			return true;
		}
	}

	return false;
}

template<class _KEY_, class _IDX_>
bool mcHashSet<_KEY_, _IDX_>::Lookup(_IDX_ *Key)
{
	if (m_pHashBuff == NULL)
		return false;

	CNode *pItem = m_pHashBuff[Key->GetHashVal() & m_nHashMask];
	for(; pItem != NULL; pItem = pItem->Nxt)
	{
		if (pItem->Key == *Key)
		{
			return true;
		}
	}

	return false;
}

template<class _KEY_, class _IDX_>
inline int mcHashSet<_KEY_, _IDX_>::GetCount()
{
	return m_nHashSize;
}

#ifdef _HASH_VC_ENUM_
template<class _KEY_, class _IDX_>
POSITION mcHashSet<_KEY_, _IDX_>::GetStartPosition() const
{
	int i;
	for(i = 0; i <= m_nHashMask; i++)
	{
		CNode *pItem;
		if ((pItem = m_pHashBuff[i]) != NULL)
		{
			return (POSITION)pItem;
		}
	}
	return NULL;
}

template<class _KEY_, class _IDX_>
void mcHashSet<_KEY_, _IDX_>::GetNextAssoc(POSITION &NextPosition, _KEY_ &Key) const
{
	CNode *pNext;
	CNode *pItem;

	if ((pItem = (CNode *)NextPosition) != NULL)
	{
		Key = pItem->Key;

		if ((pNext = pItem->Nxt) == NULL)
		{
			int i = (pItem->Key.GetHashVal() & m_nHashMask) + 1;
			for(; i <= m_nHashMask; i++)
			{
				if ((pNext = m_pHashBuff[i]) != NULL)
					break;
			}
		}
		NextPosition = (POSITION)pNext;
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////
//
// mcHashSet end
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
// mcHashMap
//
/////////////////////////////////////////////////////////////////////////////

template<class _KEY_, class _VAL_>
class mcHashMap : public mcAlloc
{
public:
	mcHashMap(int nGrow = HASH_GROW_SIZE, int nSize = HASH_INIT_SIZE);
	~mcHashMap();

	bool IsValid();
	bool InitHashSize(int nSize); // nSize必须是2 ^ n

	void Insert(_KEY_ Key, _VAL_ Val);
	void Update(_KEY_ Key, _VAL_ Val);
	void Remove(_KEY_ Key);
	void RemoveAll();

	bool Lookup(_KEY_ Key, _VAL_ &Val);
	bool Lookup(_KEY_ Key);
	bool Lookup(_KEY_*Key, _VAL_ &Val);
	bool Lookup(_KEY_*Key);

	int  GetCount();
	void ResetAll(_VAL_ Val);

#ifdef _HASH_VC_ENUM_
	POSITION GetStartPosition() const;
	void GetNextAssoc(POSITION &NextPosition, _KEY_ &Key, _VAL_ &Val) const;
	void GetNextAssoc(POSITION &NextPosition, _VAL_ &Val) const;
#endif

protected:
	class CNode
	{
	public:
		CNode *Nxt;
//#ifdef _HASH_VC_ENUM_
//		uint   Pos;
//#endif
		_KEY_  Key;
		_VAL_  Val;
	};

protected:
	int    m_nHashSize;
	int    m_nHashMask;
	CNode**m_pHashBuff;

#ifdef _HASH_DYNAMIC_
	CNode *m_pHashFree;
#endif
};

template<class _KEY_, class _VAL_>
mcHashMap<_KEY_, _VAL_>::mcHashMap(int nGrow, int nSize) : mcAlloc(nGrow * sizeof(CNode))
{
	int nMask = nSize - 1;
	if (nSize & nMask)
		nSize = 256, nMask = 255;

	m_nHashSize = 0;
	m_nHashMask = nMask;
	m_pHashBuff = (CNode **)malloc(sizeof(CNode *) * nSize);
#ifdef _HASH_DYNAMIC_
	m_pHashFree = NULL;
#endif

	if (m_pHashBuff != NULL)
		memset(m_pHashBuff, 0, sizeof(CNode *) * nSize);
}

template<class _KEY_, class _VAL_>
mcHashMap<_KEY_, _VAL_>::~mcHashMap()
{
	if (m_pHashBuff)
		free(m_pHashBuff);
}

template<class _KEY_, class _VAL_>
bool mcHashMap<_KEY_, _VAL_>::IsValid()
{
	return m_pHashBuff != NULL;
}

template<class _KEY_, class _VAL_>
bool mcHashMap<_KEY_, _VAL_>::InitHashSize(int nSize)
{
	if (nSize & (nSize - 1))
		return false;

	m_nHashMask = nSize - 1;
	m_pHashBuff = (CNode **)realloc(m_pHashBuff, sizeof(CNode *) * nSize);
	RemoveAll();
	return true;
}

template<class _KEY_, class _VAL_>
void mcHashMap<_KEY_, _VAL_>::Insert(_KEY_ Key, _VAL_ Val)
{
	uint nHash = Key.GetHashVal() & m_nHashMask;

	CNode *pItem;
#ifdef _HASH_DYNAMIC_
	if ((pItem = m_pHashFree) == NULL)
	{
		if ((pItem = (CNode *)Alloc(sizeof(CNode))) == NULL)
			return;
	}
	else
	{
		m_pHashFree = pItem->Nxt;
	}
#else
	{
		if ((pItem = (CNode *)Alloc(sizeof(CNode))) == NULL)
			return;
	}
#endif

//#ifdef _HASH_VC_ENUM_
//	pItem->Pos = nHash;
//#endif
	pItem->Val = Val;
	pItem->Key = Key;
	pItem->Nxt = m_pHashBuff[nHash];
	m_pHashBuff[nHash] = pItem;

	m_nHashSize++;
}

template<class _KEY_, class _VAL_>
void mcHashMap<_KEY_, _VAL_>::Update(_KEY_ Key, _VAL_ Val)
{
	CNode *pItem = m_pHashBuff[Key.GetHashVal() & m_nHashMask];
	for(; pItem != NULL; pItem = pItem->Nxt)
	{
		if (pItem->Key == Key)
		{
			pItem->Val = Val;
			return;
		}
	}

	Insert(Key, Val);
}

template<class _KEY_, class _VAL_>
void mcHashMap<_KEY_, _VAL_>::Remove(_KEY_ Key)
{
	if (m_pHashBuff == NULL)
		return;

	uint   nHash = Key.GetHashVal() & m_nHashMask;
	CNode**pPrev = &m_pHashBuff[nHash];
	CNode *pTemp;
	while((pTemp = *pPrev) != NULL)
	{
		if (Key == pTemp->Key)
		{
			*pPrev = pTemp->Nxt;

#ifdef _HASH_DYNAMIC_
			pTemp->Nxt  = m_pHashFree;
			m_pHashFree = pTemp;
#endif

			m_nHashSize--;
			return;
		}

		pPrev = &pTemp->Nxt;
	}
}

template<class _KEY_, class _VAL_>
void mcHashMap<_KEY_, _VAL_>::RemoveAll()
{
	FreeAll();

	m_nHashSize = 0;
#ifdef _HASH_DYNAMIC_
	m_pHashFree = NULL;
#endif
	if (m_pHashBuff != NULL)
		memset(m_pHashBuff, 0, sizeof(CNode *) * (m_nHashMask + 1));
}

template<class _KEY_, class _VAL_>
bool mcHashMap<_KEY_, _VAL_>::Lookup(_KEY_ Key, _VAL_ &Val)
{
	if (m_pHashBuff == NULL)
		return false;

	CNode *pItem = m_pHashBuff[Key.GetHashVal() & m_nHashMask];
	for (; pItem != NULL; pItem = pItem->Nxt)
	{
		if (Key == pItem->Key)
		{
			Val = pItem->Val;
			return true;
		}
	}

	return false;
}

template<class _KEY_, class _VAL_>
bool mcHashMap<_KEY_, _VAL_>::Lookup(_KEY_ Key)
{
	if (m_pHashBuff == NULL)
		return false;

	CNode *pItem = m_pHashBuff[Key.GetHashVal() & m_nHashMask];
	for(; pItem != NULL; pItem = pItem->Nxt)
	{
		if (Key == pItem->Key)
		{
			return true;
		}
	}

	return false;
}

template<class _KEY_, class _VAL_>
bool mcHashMap<_KEY_, _VAL_>::Lookup(_KEY_ *Key, _VAL_ &Val)
{
	if (m_pHashBuff == NULL)
		return false;

	CNode *pItem = m_pHashBuff[Key.GetHashVal() & m_nHashMask];
	for(; pItem != NULL; pItem = pItem->Nxt)
	{
		if (*Key == pItem->Key)
		{
			Val = pItem->Val;
			return true;
		}
	}

	return false;
}

template<class _KEY_, class _VAL_>
bool mcHashMap<_KEY_, _VAL_>::Lookup(_KEY_ *Key)
{
	if (m_pHashBuff == NULL)
		return false;

	CNode *pItem = m_pHashBuff[Key.GetHashVal() & m_nHashMask];
	for(; pItem != NULL; pItem = pItem->Nxt)
	{
		if (*Key == pItem->Key)
		{
			return true;
		}
	}

	return false;
}

template<class _KEY_, class _VAL_>
inline int mcHashMap<_KEY_, _VAL_>::GetCount()
{
	return m_nHashSize;
}

template<class _KEY_, class _VAL_>
void mcHashMap<_KEY_, _VAL_>::ResetAll(_VAL_ Val)
{
	int i;
	for(i = 0; i <= m_nHashMask; i++)
	{
		CNode *pItem;
		if ((pItem = m_pHashBuff[i]) != NULL)
		{
			do
			{
				pItem->Val = Val;
			}
			while((pItem = pItem->Nxt) != NULL);
		}
	}
}

#ifdef _HASH_VC_ENUM_
template<class _KEY_, class _VAL_>
POSITION mcHashMap<_KEY_, _VAL_>::GetStartPosition() const
{
	int i;
	for(i = 0; i <= m_nHashMask; i++)
	{
		CNode *pItem;
		if ((pItem = m_pHashBuff[i]) != NULL)
		{
			return (POSITION)pItem;
		}
	}
	return NULL;
}

template<class _KEY_, class _VAL_>
void mcHashMap<_KEY_, _VAL_>::GetNextAssoc(POSITION &NextPosition, _KEY_ &Key, _VAL_ &Val) const
{
	CNode *pNext;
	CNode *pItem;

	if ((pItem = (CNode *)NextPosition) != NULL)
	{
		Key = pItem->Key;
		Val = pItem->Val;

		if ((pNext = pItem->Nxt) == NULL)
		{
			int i = (pItem->Key.GetHashVal() & m_nHashMask) + 1;
			for(; i <= m_nHashMask; i++)
			{
				if ((pNext = m_pHashBuff[i]) != NULL)
					break;
			}
		}
		NextPosition = (POSITION)pNext;
	}
}

template<class _KEY_, class _VAL_>
void mcHashMap<_KEY_, _VAL_>::GetNextAssoc(POSITION &NextPosition, _VAL_ &Val) const
{
	CNode *pNext;
	CNode *pItem;

	if ((pItem = (CNode *)NextPosition) != NULL)
	{
		Val = pItem->Val;

		if ((pNext = pItem->Nxt) == NULL)
		{
			int i = (pItem->Key.GetHashVal() & m_nHashMask) + 1;
			for(; i <= m_nHashMask; i++)
			{
				if ((pNext = m_pHashBuff[i]) != NULL)
					break;
			}
		}
		NextPosition = (POSITION)pNext;
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////
//
// mcHashMap end
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
// mcHashTab
//
/////////////////////////////////////////////////////////////////////////////

template<class _KEY_, class _VAL_>
class mcHashTab : public mcAlloc
{
public:
	mcHashTab(int nGrow = HASH_GROW_SIZE, int nSize = HASH_INIT_SIZE);
	~mcHashTab();

	bool IsValid();
	bool InitHashSize(int nSize); // nSize必须是2 ^ n

	void Insert(_KEY_ Key, _VAL_ Val);
	void Update(_KEY_ Key, _VAL_ Val);
	void Remove(_KEY_ Key);
	void RemoveAll();

	bool Lookup(_KEY_ Key, _VAL_ &Val);
	bool Lookup(_KEY_ Key);
	bool Lookup(_KEY_*Key, _VAL_ &Val);
	bool Lookup(_KEY_*Key);

	int  GetCount();
	void ResetAll(_VAL_ Val);

#ifdef _HASH_VC_ENUM_
	POSITION GetStartPosition() const;
	void GetNextAssoc(POSITION &NextPosition, _KEY_ &Key, _VAL_ &Val) const;
	void GetNextAssoc(POSITION &NextPosition, _VAL_ &Val) const;
#endif

protected:
	class CNode
	{
	public:
		CNode *Nxt;
//#ifdef _HASH_VC_ENUM_
//		uint   Pos;
//#endif
		_KEY_  Key;
		_VAL_  Val;
	};

protected:
	int    m_nHashSize;
	int    m_nHashMask;
	CNode**m_pHashBuff;

#ifdef _HASH_DYNAMIC_
	CNode *m_pHashFree;
#endif
};

template<class _KEY_, class _VAL_>
mcHashTab<_KEY_, _VAL_>::mcHashTab(int nGrow, int nSize) : mcAlloc(nGrow * sizeof(CNode))
{
	if (nSize & (nSize - 1))
		nSize = 256;

	m_nHashSize = 0;
	m_nHashMask = nSize - 1;
	m_pHashBuff = (CNode **)malloc(sizeof(CNode *) * nSize);
#ifdef _HASH_DYNAMIC_
	m_pHashFree = NULL;
#endif

	if (m_pHashBuff != NULL)
		memset(m_pHashBuff, 0, sizeof(CNode *) * nSize);
}

template<class _KEY_, class _VAL_>
mcHashTab<_KEY_, _VAL_>::~mcHashTab()
{
	int i;
	for(i = 0; i <= m_nHashMask; i++)
	{
		CNode *pItem = m_pHashBuff[i];
		for(; pItem != NULL; pItem = pItem->Nxt)
		{
			// 析构_KEY_
			pItem->Key.Release();
		}
	}

	if (m_pHashBuff)
		free(m_pHashBuff);
}

template<class _KEY_, class _VAL_>
bool mcHashTab<_KEY_, _VAL_>::IsValid()
{
	return m_pHashBuff != NULL;
}

template<class _KEY_, class _VAL_>
bool mcHashTab<_KEY_, _VAL_>::InitHashSize(int nSize)
{
	if (nSize & (nSize - 1))
		return false;

	m_nHashMask = nSize - 1;
	m_pHashBuff = (CNode **)realloc(m_pHashBuff, sizeof(CNode *) * nSize);
	RemoveAll();
	return true;
}

template<class _KEY_, class _VAL_>
void mcHashTab<_KEY_, _VAL_>::Insert(_KEY_ Key, _VAL_ Val)
{
	uint nHash = Key.GetHashVal() & m_nHashMask;

	CNode *pItem;
#ifdef _HASH_DYNAMIC_
	if ((pItem = m_pHashFree) == NULL)
	{
		if ((pItem = (CNode *)Alloc(sizeof(CNode))) == NULL)
			return;
	}
	else
	{
		m_pHashFree = pItem->Nxt;
	}
#else
	{
		if ((pItem = (CNode *)Alloc(sizeof(CNode))) == NULL)
			return;
	}
#endif

	pItem->Key.Initial(Key);

//#ifdef _HASH_VC_ENUM_
//	pItem->Pos = nHash;
//#endif
	pItem->Val = Val;
//	pItem->Key = Key;
	pItem->Nxt = m_pHashBuff[nHash];
	m_pHashBuff[nHash] = pItem;

	m_nHashSize++;
}

template<class _KEY_, class _VAL_>
void mcHashTab<_KEY_, _VAL_>::Update(_KEY_ Key, _VAL_ Val)
{
	CNode *pItem = m_pHashBuff[Key.GetHashVal() & m_nHashMask];
	for(; pItem != NULL; pItem = pItem->Nxt)
	{
		if (pItem->Key == Key)
		{
			pItem->Val = Val;
			return;
		}
	}

	Insert(Key, Val);
}

template<class _KEY_, class _VAL_>
void mcHashTab<_KEY_, _VAL_>::Remove(_KEY_ Key)
{
	if (m_pHashBuff == NULL)
		return;

	uint   nHash = Key.GetHashVal() & m_nHashMask;
	CNode**pPrev = &m_pHashBuff[nHash];
	CNode *pTemp;
	while((pTemp = *pPrev) != NULL)
	{
		if (Key == pTemp->Key)
		{
			// 析构_KEY_
			pTemp->Key.Release();

			*pPrev = pTemp->Nxt;

#ifdef _HASH_DYNAMIC_
			pTemp->Nxt  = m_pHashFree;
			m_pHashFree = pTemp;
#endif

			m_nHashSize--;
			return;
		}

		pPrev = &pTemp->Nxt;
	}
}

template<class _KEY_, class _VAL_>
void mcHashTab<_KEY_, _VAL_>::RemoveAll()
{
	int i;
	for(i = 0; i <= m_nHashMask; i++)
	{
		CNode *pItem = m_pHashBuff[i];
		for(; pItem != NULL; pItem = pItem->Nxt)
		{
			// 析构_KEY_
			pItem->Key.Release();
		}
	}
	FreeAll();

	m_nHashSize = 0;
#ifdef _HASH_DYNAMIC_
	m_pHashFree = NULL;
#endif
	if (m_pHashBuff != NULL)
		memset(m_pHashBuff, 0, sizeof(CNode *) * (m_nHashMask + 1));
}

template<class _KEY_, class _VAL_>
bool mcHashTab<_KEY_, _VAL_>::Lookup(_KEY_ Key, _VAL_ &Val)
{
	if (m_pHashBuff == NULL)
		return false;

	CNode *pItem = m_pHashBuff[Key.GetHashVal() & m_nHashMask];
	for(; pItem != NULL; pItem = pItem->Nxt)
	{
		if (Key == pItem->Key)
		{
			Val = pItem->Val;
			return true;
		}
	}

	return false;
}

template<class _KEY_, class _VAL_>
bool mcHashTab<_KEY_, _VAL_>::Lookup(_KEY_ Key)
{
	if (m_pHashBuff == NULL)
		return false;

	CNode *pItem = m_pHashBuff[Key.GetHashVal() & m_nHashMask];
	for(; pItem != NULL; pItem = pItem->Nxt)
	{
		if (Key == pItem->Key)
		{
			return true;
		}
	}

	return false;
}

template<class _KEY_, class _VAL_>
bool mcHashTab<_KEY_, _VAL_>::Lookup(_KEY_ *Key, _VAL_ &Val)
{
	if (m_pHashBuff == NULL)
		return false;

	CNode *pItem = m_pHashBuff[Key.GetHashVal() & m_nHashMask];
	for(; pItem != NULL; pItem = pItem->Nxt)
	{
		if (*Key == pItem->Key)
		{
			Val = pItem->Val;
			return true;
		}
	}

	return false;
}

template<class _KEY_, class _VAL_>
bool mcHashTab<_KEY_, _VAL_>::Lookup(_KEY_ *Key)
{
	if (m_pHashBuff == NULL)
		return false;

	CNode *pItem = m_pHashBuff[Key.GetHashVal() & m_nHashMask];
	for(; pItem != NULL; pItem = pItem->Nxt)
	{
		if (*Key == pItem->Key)
		{
			return true;
		}
	}

	return false;
}

template<class _KEY_, class _VAL_>
inline int mcHashTab<_KEY_, _VAL_>::GetCount()
{
	return m_nHashSize;
}

template<class _KEY_, class _VAL_>
void mcHashTab<_KEY_, _VAL_>::ResetAll(_VAL_ Val)
{
	int i;
	for(i = 0; i <= m_nHashMask; i++)
	{
		CNode *pItem;
		if ((pItem = m_pHashBuff[i]) != NULL)
		{
			do
			{
				pItem->Val = Val;
			}
			while((pItem = pItem->Nxt) != NULL);
		}
	}
}

#ifdef _HASH_VC_ENUM_
template<class _KEY_, class _VAL_>
POSITION mcHashTab<_KEY_, _VAL_>::GetStartPosition() const
{
	int i;
	for(i = 0; i <= m_nHashMask; i++)
	{
		CNode *pItem;
		if ((pItem = m_pHashBuff[i]) != NULL)
		{
			return (POSITION)pItem;
		}
	}
	return NULL;
}

template<class _KEY_, class _VAL_>
void mcHashTab<_KEY_, _VAL_>::GetNextAssoc(POSITION &NextPosition, _KEY_ &Key, _VAL_ &Val) const
{
	CNode *pNext;
	CNode *pItem;

	if ((pItem = (CNode *)NextPosition) != NULL)
	{
		Key = pItem->Key;
		Val = pItem->Val;

		if ((pNext = pItem->Nxt) == NULL)
		{
			int i = (pItem->Key.GetHashVal() & m_nHashMask) + 1;
			for(; i <= m_nHashMask; i++)
			{
				if ((pNext = m_pHashBuff[i]) != NULL)
					break;
			}
		}
		NextPosition = (POSITION)pNext;
	}
}

template<class _KEY_, class _VAL_>
void mcHashTab<_KEY_, _VAL_>::GetNextAssoc(POSITION &NextPosition, _VAL_ &Val) const
{
	CNode *pNext;
	CNode *pItem;

	if ((pItem = (CNode *)NextPosition) != NULL)
	{
		Val = pItem->Val;

		if ((pNext = pItem->Nxt) == NULL)
		{
			int i = (pItem->Key.GetHashVal() & m_nHashMask) + 1;
			for(; i <= m_nHashMask; i++)
			{
				if ((pNext = m_pHashBuff[i]) != NULL)
					break;
			}
		}
		NextPosition = (POSITION)pNext;
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////
//
// mcHashTab end
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
// mcHash
//
/////////////////////////////////////////////////////////////////////////////

template <class _T>
class HashNum
{
public:
	HashNum()
	{
	}
	HashNum(const _T n) : Key(n)
	{
	}
	bool operator == (const HashNum& A)
	{
		return Key == A.Key;
	}
	uint GetHashVal()
	{
		return (uint)(long)Key ^ ((uint)(long)Key << 5) ^ ((uint)(long)Key >> 5);
	}

public:
	_T Key;
};

class HashP64
{
public:
	HashP64()
	{
	}
	HashP64(const BYTE *pKey) : Key(pKey)
	{
	}
	bool operator == (const HashP64& A)
	{
		return *(uint64 *)Key == *(uint64 *)A.Key;
	}
	uint GetHashVal()
	{
		uint n = 0x811d9dc5;
		for(uint i = 0; i < 8; i++)
		{
			n = (n << 3) ^ n ^ Key[i];
		}

		return n;
	}

public:
	const BYTE *Key;
};

class HashBin
{
public:
	HashBin(const BYTE *pKey, const uint nLen)
	{
		Key = pKey;
		Len = nLen;
	}
	bool operator == (const HashBin& A)
	{
		return Len == A.Len && memcmp(Key, A.Key, Len) == 0;
	}
	uint GetHashVal()
	{
		uint n = 0x811d9dc5;
		for(uint i = 0; i < Len; i++)
		{
			n = (n << 3) ^ n ^ Key[i];
		}

		return n;
	}

public:
	const BYTE *Key;
	uint  Len;
};

class HashStr
{
public:
	HashStr()
	{
	}
	HashStr(const char *pKey) : Key(pKey)
	{
	}
	bool operator == (const HashStr& A)
	{
		return ::strcmp(Key, A.Key) == 0;
	}
	uint GetHashVal()
	{
		uint n = 0x811d9dc5;
		for(int i = 0; Key[i] != 0; i++)
		{
		//	n = (n << 3) + n + Key[i];
			n = (n << 3) ^ n ^ Key[i];
		//	n = (n * 0x01000193) ^ Key[i];
		//	n = (n * 0x63c63cd9) ^ Key[i];
		//	n = (n * 0x63c63cd9) + 0x9c39c33d + Key[i];
		}

		return n;
	}

	operator const char *() const { return Key; }

public:
	const char *Key;
};

#if _MSC_VER >= 1400
#pragma warning(disable:4996)
#endif

class HashObj
{
public:
	HashObj()
	{
		Key = NULL;
	}
	HashObj(const char *pKey)
	{
		Key = pKey;
	}
	void Initial(const HashObj& Src)
	{
		if (Src.Key != NULL)
		{
			Key = new char[::strlen(Src.Key) + 1];
			if (Key != NULL)
				::strcpy((char *)Key, Src.Key);
		}
		else
		{
			Key = NULL;
		}
	}
	void Release()
	{
		if (Key)
			delete[] (char *)Key;
	}
	bool operator == (const HashObj& A)
	{
		return ::strcmp(Key, A.Key) == 0;
	}
	uint GetHashVal()
	{
		uint n = 0x811d9dc5;
		for(int i = 0; Key[i] != 0; i++)
		{
			n = (n << 3) ^ n ^ Key[i];
		}

		return n;
	}

	operator const char *() const { return Key; }

public:
	const char *Key;
};

#if _MSC_VER >= 1400
#pragma warning(default:4996)
#endif

/////////////////////////////////////////////////////////////////////////////
//
// mcHash end
//
/////////////////////////////////////////////////////////////////////////////

#endif
