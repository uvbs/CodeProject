#ifndef __SORTING_ARRAY_H__
#define __SORTING_ARRAY_H__

#include <pluma/pluma.h>

class SortingArray {
public:
    virtual ~SortingArray() {printf("~SortingArray %p\n", this);}
    virtual void Sort(int a[], int n) = 0;
    virtual std::string Name() = 0;
};

PLUMA_PROVIDER_HEADER(SortingArray);

#endif
