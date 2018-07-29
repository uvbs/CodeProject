#ifndef __PLUGIN_QUICK_SORT_H__
#define __PLUGIN_QUICK_SORT_H__

#include <string>
#include "interface/sorting_array.h"

class QuickSort: public SortingArray {
public:
    ~QuickSort();
    void Sort(int a[], int n);
    std::string Name();

private:
    void call(int a[], int left, int right);
};

PLUMA_INHERIT_PROVIDER(QuickSort, SortingArray, 1);

#endif
