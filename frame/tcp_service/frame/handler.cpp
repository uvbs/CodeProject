#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _LINUX64
#include <unistd.h>

#endif

#include "handler.h"

void BaseHandler::setParser(PacketParser* parser)
{
    m_parser = parser;
}

PacketParser* BaseHandler::getParser()
{
    return m_parser;
};

void BaseHandler::setPort(unsigned short port)
{
    m_port = port;
}

unsigned short BaseHandler::getPort()
{
    return m_port;
}
