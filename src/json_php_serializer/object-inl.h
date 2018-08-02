#ifndef __SERIALIZER_OBJECT_INL_H__
#define __SERIALIZER_OBJECT_INL_H__

namespace std {

template <>
struct hash<serializer::Object> 
{
    std::size_t operator()(const serializer::Object& o) const
    {
        return o.hash();
    }
};

}

#endif
