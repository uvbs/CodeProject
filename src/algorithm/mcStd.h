/***************************************************
** @file: mcStd.h
** @author: Mr.Chen
** @date: 2016/7/29
** @brief: ×Ö·û´®±ê×¼º¯Êý¿â
***************************************************/
#ifndef __MAPTRIXC_STD_STRING_LIB__
#define __MAPTRIXC_STD_STRING_LIB__

// MaptrixC Template Library
namespace MTL
{

template <class _T> _T *strchr(const _T *str, _T ch);
template <class _T> _T *strrchr(const _T *str, _T ch);
template <class _T> _T *strcat(_T *dst, const _T *src);
template <class _T> _T *strcpy(_T *dst, const _T *src);
template <class _T> _T *strstr(const _T *dst, const _T *src);
template <class _T> int strcmp(const _T *dst, const _T *src);
template <class _T> int stricmp(const _T *dst, const _T *src);
template <class _T> int strlen(const _T *str);
template <class _T> int safestrlen(const _T *str);
template <class _T> void strset(_T *str, _T ch);
template <class _T> void strrpl(_T *str, _T cf, _T ct);
template <class _T> void strrev(_T *str);
template <class _T> void strdate(_T *dst);
template <class _T> void strtime(_T *dst);
template <class _T> void upper(_T *str);
template <class _T> void lower(_T *str);

template <class _T> bool isspace(_T n);

/////////////////////////////////////////////////////////////////////////////
//
// std string
//
/////////////////////////////////////////////////////////////////////////////

template <class _T> _T *strchr(const _T *str, _T ch)
{
	for(; *str != 0; str++)
	{
		if (*str == ch)
			return (_T *)str;
	}
	return NULL;
}

template <class _T> _T *strrchr(const _T *str, _T ch)
{
	_T *end = NULL;
	for(; *str != 0; str++)
	{
		if (*str == ch)
			end = (_T *)str;
	}
	return end;
}

template <class _T> _T *strcat(_T *dst, const _T *src)
{
	while((*dst) != 0) dst++;
	while((*dst++ = *src++) != 0);
	return --dst;
}

template <class _T> _T *strcpy(_T *dst, const _T *src)
{
	while((*dst++ = *src++) != 0);
	return --dst;
}

template <class _T> _T *strstr(const _T *dst, const _T *src)
{
	if (*src == 0)
		return (_T *)dst;

	for(; *dst != 0; dst++)
	{
		_T *d = (_T *)dst;
		_T *s = (_T *)src;

		while(*d == *s && *s != 0)
			d++, s++;

		if (*s == 0)
			return (_T *)dst;
	}
	return NULL;
}

template <class _T> int strcmp(const _T *dst, const _T *src)
{
	while(*dst == *src && *src != 0)
		src++, dst++;

	return (unsigned int)*dst - (unsigned int)*src;
}

template <class _T> int stricmp(const _T *dst, const _T *src)
{
	int d, s;
	do
	{
		if ('A' <= (d = *dst++) && (d) <= 'Z')
			d += 'a' - 'A';
		if ('A' <= (s = *src++) && (s) <= 'Z')
			s += 'a' - 'A';
	}
	while(d == s && s != 0);

	return (unsigned int)d - (unsigned int)s;
}

template <class _T> int strlen(const _T *str)
{
	const _T *tmp = str;
	while(*tmp) tmp++;
	return (int)(tmp - str);
}

template <class _T> inline int safestrlen(const _T *str)
{
	return str == NULL ? 0 : strlen(str);
}

template <class _T> void strset(_T *str, _T ch)
{
	while(*str != 0)
		*str++ = ch;
}

template <class _T> void strrpl(_T *str, _T cf, _T ct)
{
	for(; *str != 0; str++)
	{
		if (*str == cf)
			*str = ct;
	}
}

template <class _T> void strrev(_T *str)
{
	_T *tmp = str;
	while(*tmp++);
	tmp -= 2;

	while(str < tmp)
	{
		_T c = *str;
		*str++ = *tmp;
		*tmp-- = c;
	}
}

template <class _T> void strdate(_T *dst)
{
	long t;
	time(&t);

	tm *m = localtime(&t);

	dst[2] = dst[5] = '/';
	dst[8] = 0;

	int Y = m->tm_year % 100;
	int M = m->tm_mon + 1;
	int D = m->tm_mday;

	dst[0] = Y / 10 + '0';
	dst[1] = Y % 10 + '0';
	dst[3] = M / 10 + '0';
	dst[4] = M % 10 + '0';
	dst[6] = D / 10 + '0';
	dst[7] = D % 10 + '0';
}

template <class _T> void strtime(_T *dst)
{
	long t;
	time(&t);

	tm *m = localtime(&t);

	dst[2] = dst[5] = ':';
	dst[8] = 0;

	int H = m->tm_hour;
	int M = m->tm_min;
	int S = m->tm_sec;

	dst[0] = H / 10 + '0';
	dst[1] = H % 10 + '0';
	dst[3] = M / 10 + '0';
	dst[4] = M % 10 + '0';
	dst[6] = S / 10 + '0';
	dst[7] = S % 10 + '0';
}

template <class _T> void upper(_T *str)
{
	for(_T c; (c = *str) != 0; str++)
	{
		if ('a' <= c && c <= 'z')
		//	*str -= 'a' - 'A';
			*str += 'A' - 'a';
	}
}

template <class _T> void lower(_T *str)
{
	for(_T c; (c = *str) != 0; str++)
	{
		if ('A' <= c && c <= 'Z')
			*str += 'a' - 'A';
	}
}

template <class _T> inline bool isspace(_T n)
{
	return ((unsigned int)(n - 9) < 5U || n == ' '); // (9 <= n && n <= 13 || n == ' ');
}

/////////////////////////////////////////////////////////////////////////////
//
// std string end
//
/////////////////////////////////////////////////////////////////////////////

}  //namespace MTL

#endif   //__MAPTRIXC_STD_STRING_LIB__

