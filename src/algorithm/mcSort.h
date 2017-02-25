#ifndef __MAPTRIXC_SORT_LIB__
#define __MAPTRIXC_SORT_LIB__
// MaptrixC Template Library

// void InsertSort(_T *Min, _T *Max);
// void QuickSort(_T *Min, _T *Max);
// void FastSort(_T *Min, _T *Max);
// void InsertSortRef(_T **Min, _T **Max);
// void QuickSortRef(_T **Min, _T **Max);
// void FastSortRef(_T **Min, _T **Max);
// void InsertSort(_T *Sort, _D *Data, int nMin, int nMax);
// void QuickSort(_T *Sort, _D *Data, int nMin, int nMax);
// void FastSort(_T *Sort, _D *Data, int nMin, int nMax);

// �������Ķ�����Ŀ < 20 ������ʹ�ò�������
template <class _T>
void InsertSort(_T *Min, _T *Max)
{
	_T *m, *n, i;
	for(m = Min; ++m <= Max;)
	{
		i = m[0];
		for (n = m; Min < n && i < n[-1]; n--) { n[0] = n[-1]; }
		n[0] = i;
	}
}

// �������Ķ�����Ŀ > 20 ������ʹ�ÿ�������
template <class _T>
void QuickSort(_T *Min, _T *Max)
{
	if (Max > Min)
	{
		_T i = Min[0];
		_T*m = Min;
		_T*n = Max;
		while(m < n)
		{
			while(i < *n && m < n)
				n--;
			if (m < n)
			{
				*m = *n;
				while(*++m < i && m < n) ;
				if (m < n) { *n-- = *m; }
			}
		}
		*m = i;
		QuickSort(Min, m - 1);
		QuickSort(m + 1, Max);
	}
}

// �������򷨡��������򷨵��ۺ�
template <class _T>
void FastSort(_T *Min, _T *Max)
{
	if (Max - Min > 24)
	{
		_T i = Min[0];
		_T*m = Min;
		_T*n = Max;
		while(m < n)
		{
			while(i < *n && m < n) { n--; }
			if (m < n)
			{
				*m = *n;
				while(*++m < i && m < n);
				if (m < n) { *n-- = *m; }
			}
		}

		*m = i;
		if (m > Min) { FastSort(Min, m - 1); }
		if (m < Max) { FastSort(m + 1, Max); }
	}
	else if (Max - Min > 0)
	{
		InsertSort(Min, Max);
	}
}

// ָ�����������
template <class _T>
void InsertSortRef(_T **Min, _T **Max)
{
	_T **m, **n, *i;
	for(m = Min; ++m <= Max;)
	{
		i = m[0];
		for (n = m; Min < n && *i < *n[-1]; n--) { n[0] = n[-1]; }
		n[0] = i;
	}
}

// ָ�����������
template <class _T>
void QuickSortRef(_T **Min, _T **Max)
{
	if (Max > Min)
	{
		_T *i = Min[0];
		_T**m = Min;
		_T**n = Max;

		while(m < n)
		{
			while(*i < **n && m < n) { n--; }
			if (m < n)
			{
				*m = *n;
				while(**++m < *i && m < n) ;
				if (m < n) { *n-- = *m; }
			}
		}
		*m = i;
		QuickSortRef(Min, m - 1);
		QuickSortRef(m + 1, Max);
	}
}

// ָ�����������
template <class _T>
void FastSortRef(_T **Min, _T **Max)
{
	if (Max - Min > 24)
	{
		_T *i = Min[0];
		_T**m = Min;
		_T**n = Max;

		while(m < n)
		{
			while(*i < **n && m < n) { n--; }
			if (m < n)
			{
				*m = *n;
				while(**++m < *i && m < n);
				if (m < n) { *n-- = *m; }
			}
		}
		*m = i;
		FastSortRef(Min, m - 1);
		FastSortRef(m + 1, Max);
	}
	else
	{
		InsertSortRef(Min, Max);
	}
}

// ���ո�������ֵ�����ݽ�������
template <class _T, class _D>
void InsertSort(_T *Sort, _D *Data, int nMin, int nMax)
{
	int m, n;
	_T  s;
	_D  d;
	for(m = nMin; ++m <= nMax;)
	{
		s = Sort[m];
		d = Data[m];
		for(n = m; nMin < n && s < Sort[n - 1]; n--)
		{
			Sort[n] = Sort[n - 1];
			Data[n] = Data[n - 1];
		}
		Sort[n] = s;
		Data[n] = d;
	}
}

// ���ո�������ֵ�����ݽ�������
template <class _T, class _D>
void QuickSort(_T *Sort, _D *Data, int nMin, int nMax)
{
	if (nMax > nMin)
	{
		int m = nMin;
		int n = nMax;
		_T  s = Sort[nMin];
		_D  d = Data[nMin];

		while(m < n)
		{
			while(s < Sort[n] && m < n) { n--; }
			if (m < n)
			{
				Sort[m] = Sort[n];
				Data[m] = Data[n];

				while(Sort[++m] < s && m < n);
				if (m < n)
				{
					Sort[n] = Sort[m];
					Data[n] = Data[m];
					n--;
				}
			}
		}

		Sort[m] = s;
		Data[m] = d;
		QuickSort(Sort, Data, nMin, m - 1);
		QuickSort(Sort, Data, m + 1, nMax);
	}
}

// ���ո�������ֵ�����ݽ�������
template <class _T, class _D>
void FastSort(_T *Sort, _D *Data, int nMin, int nMax)
{
	if (nMax - nMin > 24)
	{
		int m = nMin;
		int n = nMax;
		_T  s = Sort[nMin];
		_D  d = Data[nMin];

		while(m < n)
		{
			while(s < Sort[n] && m < n) { n--; }
			if (m < n)
			{
				Sort[m] = Sort[n];
				Data[m] = Data[n];
				while(Sort[++m] < s && m < n) ;
				if (m < n)
				{
					Sort[n] = Sort[m];
					Data[n] = Data[m];
					n--;
				}
			}
		}

		Sort[m] = s;
		Data[m] = d;
		FastSort(Sort, Data, nMin, m - 1);
		FastSort(Sort, Data, m + 1, nMax);
	}
	else
	{
		InsertSort(Sort, Data, nMin, nMax);
	}
}

#endif // __MAPTRIXC_SORT_LIB__

