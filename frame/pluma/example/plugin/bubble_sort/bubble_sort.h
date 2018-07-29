#ifndef __PLUGIN_BUBBLE_SORT_H__
#define __PLUGIN_BUBBLE_SORT_H__

#include <string>
#include <pluma/pluma.h>
#include "interface/sorting_array.h"

class BubbleSort : public SortingArray {
public:
    ~BubbleSort();
    void Sort(int a[], int n);
    std::string Name();
};

PLUMA_INHERIT_PROVIDER(BubbleSort, SortingArray, 1);

#endif
