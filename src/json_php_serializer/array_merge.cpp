// array_merge.cc (2016-05-18)
// Yan Gaofeng (yangaofeng@360.cn)

#include <iostream>
#include "iarray.h"
#include "exception.h"

namespace serializer {

void VectorMerge(Object &out, const Object &in);
void MapMerge(Object &out, const Object &in);

void ArrayMerge(Object &out, const Object &in)
{
    if (out.IsVector()) {
        VectorMerge(out, in);
    } else if (out.IsMap()) {
        MapMerge(out, in);
    } else {
        throw exception("TypeError: expected object type vector/map");
    }
}

void VectorMerge(Object &out, const Object &in)
{
    if (!in.IsVector()) {
        throw exception("TypeError: expected object type vector");
    }

    Object::const_iterator it = in.begin();
    for (; it != in.end(); it++) {
        out.push_back(it.value());
    }
}

void MapMerge(Object &out, const Object &in)
{
    if (!in.IsMap()) {
        throw exception("TypeError: expected object type map");
    }

    Object::const_iterator it = in.begin();
    for (; it != in.end(); it++) {
        Object::iterator pos = out.find(it.key());
        if (pos == out.end()) {
            out.insert(it.key(), it.value());
        } else {
            pos.value() = it.value();
        }
    }
}

}
