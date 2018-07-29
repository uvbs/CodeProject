#include <pluma/pluma.h>
#include "quick_sort.h"

using namespace cloud_common::pluma;

extern "C" 
bool Register(Registrar& registrar)
{
    if (!registrar.Register<QuickSortProvider>(100)) {
        printf("Register QuickSortProvider failed\n");
    }

    return true;
}
