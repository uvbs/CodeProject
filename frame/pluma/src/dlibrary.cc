#include <cstdio>
#include <string>
#include "pluma/dlibrary.h"

using namespace std;

namespace cloud_common {
namespace pluma {

DLibrary* DLibrary::Load(const string& path)
{
    if (path.empty()) {
        return NULL;
    }

    void* handle = NULL;

    //handle = dlopen(path.c_str(), RTLD_NOW);
    handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
    //handle = dlopen(path.c_str(), RTLD_LAZY);
    //handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (handle == NULL) {
        fprintf(stderr, "dlopen %s error: %s\n", path.c_str(), dlerror());
        return NULL;
    }

    return new DLibrary(handle);
}

DLibrary::DLibrary(void* handle):
    handle_(handle)
{
}

DLibrary::~DLibrary()
{
    if (handle_ != NULL) {
        dlclose(handle_);
    }
}

void* DLibrary::GetSymbol(const string& symbol)
{
    if (handle_ == NULL) {
        return NULL;
    }

    void* func = dlsym(handle_, symbol.c_str());
    if (func == NULL) {
        return NULL;
    }

    return func;
}

}   // namespace pluma
}   // namespace cloud_common
