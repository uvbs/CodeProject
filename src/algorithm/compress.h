// Copyright 2016 AMAP. All Rights Reserved.
// Author:
//
// 
#ifndef _COMPRESS_H
#define _COMPRESS_H

#include "zlib.h"

namespace algorithm
{
    /* Compress data */
    int zcompress( Bytef* data, uLong ndata, Bytef* zdata, uLong* nzdata );

    /* Uncompress data */
    int zdecompress( Byte* zdata, uLong nzdata, Byte* data, uLong* ndata );

    /* Compress gzip data */
    int gzcompress( Bytef* data, uLong ndata, Bytef* zdata, uLong* nzdata );

    /* Uncompress gzip data */
    int gzdecompress( Byte* zdata, uLong nzdata, Byte* data, uLong* ndata );

    /* HTTP gzip decompress */
    int httpgzdecompress( Byte* zdata, uLong nzdata, Byte* data, uLong* ndata );
}

#endif  //_COMPRESS_H










