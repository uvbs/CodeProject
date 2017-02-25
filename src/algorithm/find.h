////////////////////////////////////////////////////////////////////////////////////
// 
// @file: find.h
// @author: by mr.chen
// @date: 2014/5/14	14:21
// @brief: 查找
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef _FIND_H
#define _FIND_H

namespace algorithm
{
/**************************************************
	二分查找(从有序表中查找)
**************************************************/
    template <class T>
    int find_binary(T array[], int length, T key)
    {
        int low = 0, high = length - 1, mid = 0;
        while (low <= high)
        {
            mid = (low + high) / 2;
            if (array[mid] == key) { return mid; }
            else if (array[mid] < key) { low = mid + 1; }
            else { high = mid - 1; }
        }
        return -1;
    }

   
/**************************************************
	找次大元, 从e[0]...e[n-1]中，利用树结构
**************************************************/
	template <class T>
	T find_second_max(T e[], int n)
	{
		T* tmp = new T[2 * n];
		//load n elements into tmp[n]...tmp[2n-1]
		for (int i = 0; i < n; i++) { tmp[i + n] = e[i]; }
		for (int last = 2 * n - 2; last >= 2; last -= 2) 
		{
			tmp[last / 2] = Max(tmp[last], tmp[last + 1]);
		}
		//find the secondLargest
		T largest = tmp[1];
		T second_largest = -100000000;
		int i = 1;
		while (i < n)
		{
			if (tmp[2 * i] == largest) 
			{
				i = 2 * i;
				if (tmp[i + 1] > second_largest) { second_largest = tmp[i + 1]; }
			}
			else 
			{
				i = 2 * i + 1;
				if (tmp[i - 1] > second_largest) { second_largest = tmp[i - 1]; }
			}
		}
		delete []tmp;
		return second_largest;
	}


/**************************************************
	查找第k个小的元素
**************************************************/
	template <class T>
	T find_kth(T array[], int first, int last, int k)
	{
		T ans;
		if (first == last) { ans = array[first]; }
		else
		{
			int splitPoint = partition<T>(array, first, last);
			if (k < splitPoint) { ans = find_kth(array, first, splitPoint - 1, k); }
			else if (k > splitPoint) { ans = find_kth(array, splitPoint + 1, last, k); } //注意k是下标，因此不要写成k-split
			else { ans = array[splitPoint]; }
		}
		return ans;
	}

};	//namespace algorithm

using namespace algorithm;

#endif	// _FIND_H
