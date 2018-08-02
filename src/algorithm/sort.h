////////////////////////////////////////////////////////////////////////////////////
// 
// @file: sort.h
// @author: by Mr.Chen
// @date: 2014/5/14	16:07
// @brief: ����
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef _SORT_H
#define _SORT_H

#include "maths.h"
#include "macrodef.h"
#include "myassert.h"
#include "partition.h"

namespace algorithm
{
/**************************************************
	��������
**************************************************/

	/// �汾1����a[0...n-1]����
	template<class T>
	void	qsort(T array[], int low, int high)
	{
		if (low < high)
		{
			int pivot_pos = partition(array, low, high);
			qsort(array, low, pivot_pos - 1);
			qsort(array, pivot_pos + 1, high);
		}
	}

	/// �汾2����a[0...n-1]����
	template<class T>
	void	qsort_v2(T array[], int first, int last)
	{
		//ֻ�Զ̵������н��еݹ飬���ٵݹ�ջ���
		//first2...last2��ʾ���Σ�first1...last1��ʾ�̶�
		int first1, last1, first2, last2;
		first2 = first;
		last2 = last;
		while (last2 > first2) //�����õ���
		{
			//ѡ��E[first2],E[(first2+last2)/2],E[last2]���м�ֵ��Ϊpivot
			int split_pos = partition(array, first2, last2);
			if (split_pos < (first2 + last2) / 2)
			{
				//���̶θ�ֵ
				first1 = first2;
				last1 = split_pos - 1;
				//�����θ�ֵ
				first2 = split_pos + 1;
				last2 = last2;
			}
			else
			{
				//���̶θ�ֵ
				first1 = split_pos + 1;
				last1 = last2;
				//�����θ�ֵ
				first2 = first2;
				last2 = split_pos - 1;
			}
			qsort_v2(array, first1, last1); //�̶��õݹ�
		}
	}


/**************************************************
	������
**************************************************/
	// �ѵ���
	// ��֪array[s...m]�еļ�¼����array[s]֮�������ѵĶ��壬
	// ����HeapAdjust()ʹarray[s...m]������һ���󶥶ѡ�
	template<class T>
	void	adjust_heap(T array[], int s, int m)
	{
		int rc = array[s];
		for (int j = 2 * s + 1; j <= m; j = 2 * j + 1)
		{
			if (j < m && array[j] < array[j + 1]) { j++;}
			if (rc >= array[j]) { break;}
			array[s] = array[j];
			s = j;
		}
		array[s] = rc;
	}

	// ��˳���array���ж�����
	template<class T>
	void	sort_heap(T array[], int length)
	{
		for (int i = length / 2; i >= 0; i--)
		{
			adjust_heap(array, i, length - 1);
		}
		for (int i = length - 1; i > 0; i--)
		{
			swap<T>(array[i], array[0]);
			adjust_heap(array, 0, i - 1);
		}
	}


/**************************************************
	�鲢����
**************************************************/
	// �鲢��������
	template<class T>
	void	merge(T array[], int first, int mid, int last, T temp[])
	{
		int i = first, j = mid + 1;
		int m = mid, n = last;
		int k = 0;
		while (i <= m && j <= n)  
		{
			if (array[i] < array[j]) { temp[k++] = array[i++];}
			else { temp[k++] = array[j++];}
		}
		while (i <= m)
		{
			temp[k++] = array[i++];
		}
		while (j <= n)
		{
			temp[k++] = array[j++];
		}
		for (i = 0; i < k; i++)
		{
			array[first + i] = temp[i];
		}
	}

	template<class T>
	void	sort_merge(T array[], int first, int last, T temp[])
	{
		if (first < last)
		{
			int mid = (first + last) / 2;
			sort_merge<T>(array, first, mid, temp); 
			sort_merge<T>(array, mid + 1, last, temp);
			merge<T>(array, first, mid, last, temp);
		}
	}

	// �鲢����
	template<class T>
	void	sort_merge(T array[], int length)
	{
		T* temp = new T[length];
		sort_merge<T>(array, 0, length - 1, temp);
		SAFE_DELETE_ARRAY(temp);
	}
 
/**************************************************
	ð������
**************************************************/
	///�汾1
    template<class T>
    void	sort_bubble(T array[], int length)
    {
        //��a[0...n-1]����
        bool change = true;
        for (int i = length - 1; i >= 1 && change; --i)
        {
            change = false;
            for (int j = 0; j < i; ++j)
            {
                if (array[j] > array[j + 1]) 
				{ 
					swap<T>(array[j], array[j + 1]); 
					change = true;
				}
            }
        }
    }

	///�汾2
    template<class T>
    void	sort_bubble_v2(T array[], int length) //��¼���Ľ���λ��
    {
        //��a[0...n-1]����
        int num_pairs = length - 1;
        int last_exchange_index = -1;
        while (num_pairs > 0)
        {
            last_exchange_index = -1;
            for (int i = 0; i < num_pairs; ++i)
            {
                if (array[i] > array[i + 1]) 
				{
                    swap<T>(array[i], array[i + 1]);
                    last_exchange_index = i;
                }
            }
            num_pairs = last_exchange_index;
        }
    }

	///�汾2
    template<class T>
    void	sort_bubble_v3(T array[], int length) //��¼��ʼ�����Ľ���λ��
    {
        //��a[0...n-1]����
        int num_pairs1 = 0;
        int num_pairs2 = length - 1;
        int first_exchange_index = -1;
        int last_exchange_index = -1;
        while (num_pairs1 < num_pairs2)
        {
            int num_of_exchange = 0;
            first_exchange_index = -1;
            last_exchange_index = -1;
            for (int i = num_pairs1; i < num_pairs2; ++i)
            {
                if (array[i] > array[i + 1]) 
				{
                    swap<T>(array[i], array[i + 1]);
                    num_of_exchange++;
                    if (1 == num_of_exchange) { first_exchange_index = i ? i - 1 : i; }
                    last_exchange_index = i;
                }
            }
            num_pairs1 = first_exchange_index;
            num_pairs2 = last_exchange_index;
        }
    }


/**************************************************
	��������
**************************************************/
	template<class T>
	void	sort_insert(T array[],int length)
	{
		for(int i = 1;i < length; i++)
		{
			T w = array[i];
			int j = 0;
			for(j = i - 1; j >= 0 && w < array[j]; j--) { array[j + 1]=array[j]; }
			array[j + 1]=w;
		}
	}


/**************************************************
	ѡ������
**************************************************/
	template<class T>
	void	sort_select(T array[], int length)
	{
		int i = 0, j = 0, k = 0;
		for (i = 0; i < length; i++)
		{
			j = i;
			for (k = i + 1; k < length; k++)
			{
				if (array[k] < array[j]) { j = k; }
			}
			if (j != i) { swap<T>(array[i], array[j]); }
		}
	}


/**************************************************
	��������
**************************************************/
	
	const int RADIX = 10; // 10����
	// �������s�ĵ�mλ
	// m==0 ��ʾ���λ����
	// m==MAX_DIGITS-1 ��ʾ��λ����
	template<int MAX_DIGITS>
	int getkey(int s, int m)
	{
		MyAssert(m >= 0 && m < MAX_DIGITS);
		int keys[RADIX] = {0};
		int i = MAX_DIGITS - 1;
		while (s)
		{
			keys[i] = s % RADIX;
			s = s / RADIX;
			--i;
		}
		return keys[m];
	}

	// һ�˻��������ռ�&�ַ���
	template<int MAX_DIGITS>
	void	radixPass(int array1[], int array2[], int count[], int length, int m)
	{
		// Collect
		for(int key = 0; key < RADIX; key++) { count[key] = 0; }
		for (int i = 0; i < length; i++)
		{
			int key = getkey<MAX_DIGITS>(array1[i], m);
			count[key]++;
		}
		for (int key = 1; key < RADIX; key++) { count[key] = count[key - 1] + count[key];}
		// Distribute
		for (int i = length - 1; i >= 0; i--)
		{
			int key = getkey<MAX_DIGITS>(array1[i], m);
			array2[count[key] - 1] = array1[i];
			count[key]--;
		}
	}

	// ��������
	template<int MAX_DIGITS>
	void	sort_radix(int array[], int length)
	{
		int *arraytemp = new int[length];
		int *count = new int[RADIX];
		int i = MAX_DIGITS-1;
		bool bReverse = false;
		while (i >= 0) 
		{
			if (!bReverse) { radixPass<MAX_DIGITS>(array, arraytemp, count, length, i);}
			else { radixPass<MAX_DIGITS>(arraytemp, array, count, length, i);}
			--i; 
			bReverse = !bReverse;
		}
		if (bReverse) { 
			for (int i = 0; i < length; i++) { array[i] = arraytemp[i];}
		}
	}

};	// namespace algorithm

using namespace algorithm;

#endif	// _SORT_H

