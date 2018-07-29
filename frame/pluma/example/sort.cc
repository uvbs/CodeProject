#include <stdlib.h>
#include <assert.h>
#include <pluma/pluma.h>
#include "interface/sorting_array.h"
#include "insertion_sort.h"

using namespace std;
using namespace cloud_common::pluma;

void PlumaTest(const string& tag)
{
    /* 
     * 初始化Pluma对象
     * */
    Pluma pluma;

    /*
     * AcceptProviderType函数框架可以用来注册可以支持哪些Provider插件类型
     * 插件中注册Provider时pluma会判断是否是注册过的类型
     * 如果Provider的类型没有注册过,则注册失败
     * 演示例子只有一个SortingArrayProvider类型,实际业务中可以有多个类型
     * */
    pluma.AcceptProviderType<SortingArrayProvider>();

    /*
     * 不通过动态库加载的方式注册Provider
     * */
    pluma.RegisterProvider<InsertionSortProvider>();

    /*
     * 加载插件的动态库
     * 实际业务中可以通过配置文件动态加载
     * */
    if (!pluma.Load("./plugin/bubble_sort1/libbubble_sort.so.1.1", "BubbleSort/Iverson1", "bubble_sort.conf.1.1")) {
        printf("pluma load 'libbubble_sort.so.1.1' failed!\n");
        return;
    }

#if 0
    if (!pluma.Load("./plugin/bubble_sort1/libbubble_sort.so.1.1", "BubbleSort/Iverson1", "bubble_sort.conf.1.1")) {
        printf("pluma load 'libbubble_sort.so.1.1' failed!\n");
        return;
    }

    if (!pluma.Load("./plugin/bubble_sort1/libbubble_sort.so.1.1", "", "bubble_sort.conf.1.1")) {
        printf("pluma load 'libbubble_sort.so.1.1' failed!\n");
        return;
    }
#endif

    if (!pluma.Load("./plugin/bubble_sort2/libbubble_sort.so.1.2", "BubbleSort/Iverson2", "bubble_sort.conf.1.2")) {
        printf("pluma load 'libbubble_sort.so.1.2' failed!\n");
        return;
    }

    if (!pluma.Load("./plugin/bubble_sort/libbubble_sort.so.1.0", "default", "bubble_sort.conf.1.0")) {
        printf("pluma load 'libbubble_sort.so.1.0' failed!\n");
        return;
    }

    if (!pluma.Load("./plugin/quick_sort_last/libquick_sort.so.1.0", "QuickSort/Last")) {
        printf("pluma load 'libquick_sort.so.1.0' failed!\n");
        return;
    }

    if (!pluma.Load("./plugin/quick_sort_middle/libquick_sort.so.1.1", "QuickSort/Middle")) {
        printf("pluma load 'libquick_sort.so.1.1' failed!\n");
        return;
    }

    if (!pluma.Load("./plugin/my_sort/libmy_sort.so.1.0", "QuickSort/Last")) {
        printf("pluma load 'libmy_sort.so.1.1' failed!\n");
        return;
    }

#if 0
    assert(pluma.IsLoaded("bubble_sort", "default"));
    assert(pluma.IsLoaded("bubble_sort", "BubbleSort/Iverson1"));
    assert(pluma.IsLoaded("bubble_sort", "BubbleSort/Iverson2"));
    assert(pluma.IsLoaded("quick_sort", "QuickSort/Last"));
    assert(pluma.IsLoaded("quick_sort", "QuickSort/Middle"));
    assert(pluma.IsLoaded("my_sort", "QuickSort/Last"));
#endif

    {
        /*
         * 用来存放所有SortingArrayProvider的实例化对象
         * */
        vector<SortingArrayProvider*> providers;

        /*
         * GetProviders可以通过tag来选取相同插件的多个实现版本
         * */
        pluma.GetProviders(providers);
        printf("providers size=%lu\n", providers.size());

        vector<SortingArrayProvider*>::iterator iter = providers.begin();
        for (; iter != providers.end(); ++iter) {
            SortingArray* sorter = (*iter)->Create();

            printf("Algorithm Name: %s Conf=%s\n", sorter->Name().c_str(), (*iter)->conf().c_str());

            (*iter)->ThreadReset(sorter);

            /* TEST */
            {
                printf("1---------------------------------------------------\n");
                (*iter)->ThreadReset();
                printf("1---------------------------------------------------\n");
                sorter = (*iter)->Create();
                (*iter)->ThreadReset(sorter);

                sorter = (*iter)->Create();
                printf("2---------------------------------------------------\n");
                (*iter)->ThreadReset(sorter);
                printf("2---------------------------------------------------\n");
            }
        }
    }

    {
        /*
         * 用来存放所有SortingArrayProvider的实例化对象
         * */
        vector<SortingArrayProvider*> providers;

        /*
         * GetProviders可以通过tag来选取相同插件的多个实现版本
         * */
        pluma.GetProviders(providers, tag);
        printf("providers size=%lu\n", providers.size());

        vector<SortingArrayProvider*>::iterator iter = providers.begin();
        for (; iter != providers.end(); ++iter) {
            int array[10] = {3, 5, 1, 2, 8, 9, 0, 4, 6, 7};

            SortingArray* sorter = (*iter)->ThreadGet();
            //SortingArray* sorter = (*iter)->Get();
            if (sorter == NULL) {
                printf("ThreadGet Failed!\n");
                continue;
            }
            sorter->Sort(array, sizeof(array)/sizeof(int));

            printf("Algorithm Sort: %s\n", sorter->Name().c_str());

            (*iter)->ThreadReset();
        }
    }

    printf("------------------------------------------------------------\n");
}

int main(int argc, char** argv)
{
    int count = 1;
    string tag = "default";

    if (argc < 2) {
        printf("usage: %s tag["
                "default, "
                "BubbleSort/Iverson1, "
                "BubbleSort/Iverson2, "
                "QuickSort/Last, "
                "QuickSort/Middle] count\n", argv[0]);
        return -1;
    }

    tag = argv[1];

    if (argc == 3) {
        count = atoi(argv[2]);
    }

    for (int i = 0; i < count; ++i) {
        PlumaTest(tag);
    }

    return 0;
}
