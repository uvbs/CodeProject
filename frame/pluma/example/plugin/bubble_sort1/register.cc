#include <pluma/pluma.h>
#include "bubble_sort.h"

using namespace cloud_common::pluma;

extern "C" 
bool Register(Registrar& registrar)
{
    /*
     * 这里注册的是*Provider
     * BubbleSortProvider类是通过宏PLUMA_INHERIT_PROVIDER自动创建的
     * */
    if (!registrar.Register<BubbleSortProvider>()) {
        printf("Register BubbleSortProvider failed\n");
    }

    return true;
}
