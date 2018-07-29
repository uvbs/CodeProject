#ifndef __PLUMA_DYNAMIC_LIBRARY_H__
#define __PLUMA_DYNAMIC_LIBRARY_H__

#include <string>
#include <dlfcn.h>

namespace cloud_common {
namespace pluma {

/* Manages a Dynamic Linking Library. */
class DLibrary {
public:

    /* Load a library. */
    static DLibrary* Load(const std::string& path);

    ~DLibrary();

    /* Get a symbol from the library. */
    void* GetSymbol(const std::string& symbol);

private:
    /*
     * Default constructor
     * Library instances cannot be created, use load instead.
     * */
    DLibrary();

    /* Constructor via library handle. */
    DLibrary(void* handle);

private:
    /* Library Handle */
    void* handle_; 
};


} // namespace pluma
} // namespace cloud_common

#endif // __PLUMA_DYNAMIC_LIBRARY_H__
