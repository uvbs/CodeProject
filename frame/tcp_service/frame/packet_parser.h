#ifndef __packet_parser__h__
#define __packet_parser__h__

#include <stdio.h>
#include <stdlib.h>

class PacketParser
{
public:
    PacketParser();
    virtual ~PacketParser();
public:
    virtual int parseHeader(const char* buf, size_t buf_len, size_t& headerlen, size_t& bodylen, size_t& packetlen) = 0;
};

#endif
