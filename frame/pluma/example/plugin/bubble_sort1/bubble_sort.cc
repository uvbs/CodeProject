#include <string>
#include "bubble_sort.h"

using namespace std;

BubbleSort::~BubbleSort()
{
    printf("~BubbleSort %s %p\n", Name().c_str(), this);
}

string BubbleSort::Name()
{
    return "BubbleSortIverson1";
}

void BubbleSort::Sort(int a[], int n)
{
    bool t = false;
    for (int i = 0; i < n - 1 && !t; i++) {
        t = true;
        for (int j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
                t = false;
            }
        }
    }
}
