////////////////////////////////////////////////////////////////////////////////////
// 
// @file: partition.h
// @author: by Mr.Chen
// @date: 2014/5/14	16:10
// @brief: 切分
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __PARTIRION_H_
#define __PARTIRION_H_

namespace algorithm
{
//////////////////////////////////////////////////////////////////////////////////////
///  找中位数
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
// 分划分成2段
/////////////////////////////////////////////////////////////////////////////
	template <class T>
	int	 partition(T array[], int low, int high) 
	{
		//选择E[low],E[(low+high)/2],E[high]的中间值作为pivot
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
