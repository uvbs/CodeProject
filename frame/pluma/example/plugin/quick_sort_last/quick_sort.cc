#include <string>
#include "quick_sort.h"

using namespace std;

QuickSort::~QuickSort()
{
    printf("~QuickSort %s %p\n", Name().c_str(), this);
}

string QuickSort::Name()
{
    return "QuickSortLast";
}

void QuickSort::Sort(int a[], int n)
{
    call(a, 0, n - 1);
}

void QuickSort::call(int a[], int left, int right)
{
    int i = left;
    int j = right;
    int pivot = a[j];
    int temp;

    while (i <= j)
    {
        while (a[i] < pivot)
        {
            ++i;
        }

        while (a[j] > pivot)
        {
            --j;
        }

        if (i <= j)
        {
            temp = a[i];
            a[i] = a[j];
            a[j] = temp;
            ++i;
            --j;
        }
    }

    if (left < j)
    {
        QuickSort::call(a, left, j);
    }

    if (i < right)
    {
        QuickSort::call(a, i, right);
    }
}
