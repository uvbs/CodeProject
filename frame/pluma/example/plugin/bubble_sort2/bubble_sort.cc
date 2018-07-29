#include <string>
#include "bubble_sort.h"

using namespace std;

BubbleSort::~BubbleSort()
{
    printf("~BubbleSort %s %p\n", Name().c_str(), this);
}

string BubbleSort::Name()
{
    return "BubbleSortIverson2";
}

void BubbleSort::Sort(int a[], int n)
{
    int bound = n - 1;
    int i = 0, t = 0;

    while (true) {
        t = 0;
        while (i < bound) {
            if (a[i] > a[i + 1]) {
                int temp = a[i];
                a[i] = a[i + 1];
                a[i + 1] = temp;
                t = i;
            }
            i++;
        }

        if (t != 0) {
            bound = t;
            i = 0;
        } else {
            break;
        }
    }
}
