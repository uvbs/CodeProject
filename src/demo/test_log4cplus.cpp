/***************************************************
** @file: test_log4cplus.cpp
** @author: Mr.Chen
** @date: 2016/8/2
** @brief: 
***************************************************/
#include "socketdef.h"
#include "test_log4cplus.h"

void test_log4cplus()
{
    LogManager::getSinglePtr()->init("./config/log.cfg");
    LogManager::getSinglePtr()->log(LOG_CPLUS_FILE_INFO, LOG_CPLUS_LEVEL_INFO, 
        "hello world");
}

