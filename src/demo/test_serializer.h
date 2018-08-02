#include <string>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <typeinfo>
#include "serialize.h"
#include "object.h"
//#include <google/profiler.h>

namespace serializer_php {
    int test_serializer_php();
}

namespace serializer_json {
    int test_serializer_json();
}

using namespace serializer_json;
using namespace serializer_php;

