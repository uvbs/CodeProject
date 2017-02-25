////////////////////////////////////////////////////////////////////////////////////
// 
// @file: partition.h
// @author: by Mr.Chen
// @date: 2014/5/14	16:10
// @brief: �з�
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __PARTIRION_H_
#define __PARTIRION_H_

namespace algorithm
{
//////////////////////////////////////////////////////////////////////////////////////
///  ����λ��
//////////////////////////////////////////////////////////////////////////////////////
	template <class T>
	int	 median(T array[], int low, int mid, int high)
	{
		if (array[low] <= array[high])
		{
			if (array[low] <= array[mid])
			{
				if (array[mid] <= array[high]) { return mid; }
				else { return high; }
			}
			else { return low;}
		}
		else
		{
			if (array[high] <= array[mid])
			{
				if (array[mid] <= array[high]) { return mid;}
				else { return high; }
			}
			else { return high; }
		}
	}


/////////////////////////////////////////////////////////////////////////////
// �ֻ��ֳ�2��
/////////////////////////////////////////////////////////////////////////////
	template <class T>
	int	 partition(T array[], int low, int high) 
	{
		//ѡ��E[low],E[(low+high)/2],E[high]���м�ֵ��Ϊpivot
		int pivot_pos = median(array, low, (low + high) / 2, high);
		swap<T>(array[pivot_pos], array[low]);
		T pivot = array[low];
		while (low < high)
		{
			while (low < high && array[high] >= pivot) { --high; }
			array[low] = array[high];
			while (low < high && array[low] <= pivot) { ++low; }
			array[high] = array[low];
		}
		array[low] = pivot;
		return low;
	}

};	// namespace algorithm
using namespace algorithm;

#endif	// __PARTIRION_H_
