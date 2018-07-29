#ifndef __INSERTION_SORT_H__
#define __INSERTION_SORT_H__

#include <string>
#include "interface/sorting_array.h"

class InsertionSort : public SortingArray {
public:
    ~InsertionSort();
    void Sort(int a[], int n);
    std::string Name();
};

PLUMA_INHERIT_PROVIDER(InsertionSort, SortingArray, 1);

#endif
