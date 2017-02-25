#ifndef __MAPTRIXC_DEFAULT_FUNCTION__
#define __MAPTRIXC_DEFAULT_FUNCTION__
// MaptrixC Template Library

template <class _T>
struct _DEF_LT_
{
	bool operator()(const _T& A, const _T& B) const { return A < B; }
};

template <class _T>
struct _DEF_GT_
{
	bool operator()(const _T& A, const _T& B) const { return A > B; }
};

template <class _T>
struct _DEF_LE_
{
	bool operator()(const _T& A, const _T& B) const { return A <= B; }
};

template <class _T>
struct _DEF_GE_
{
	bool operator()(const _T& A, const _T& B) const { return A >= B; }
};

template <class _T>
struct _DEF_EQ_
{
	bool operator()(const _T& A, const _T& B) const { return A == B; }
};

template <class _T>
struct _DEF_NE_
{
	bool operator()(const _T& A, const _T& B) const { return A != B; }
};

#endif
