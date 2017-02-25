/***************************************************
** @file: dbhelper.h
** @author: Mr.Chen
** @date: 2016/7/25
** @brief: 
***************************************************/
#ifndef _DB_HELPER_H
#define _DB_HELPER_H

#include "macrodef.h"
#include "shm_def.h"
#include "log.h"
#include "data_struct.h"

namespace db_helper
{

//读文件宏
#define FILE_DB_READ(data) \
    readSize = fread(&data, 1, sizeof(data), f);\
    LogSystem::getSinglePtr()->saveLog(LOG_FILE_DEBUG,"Read = %d, sizeof(%s) = %d", readSize, #data, sizeof(data)); \
    if(readSize == 0) \
    Assert(0);\
    totalReadSize += readSize; \

//写文件宏
#define FILE_DB_WRITE(data) \
    writeSize = fwrite(&data, 1, sizeof(data), f);\
    LogSystem::getSinglePtr()->saveLog(LOG_FILE_DEBUG,"Write = %d, sizeof(%s) = %d", writeSize, #data, sizeof(data)); \
    if(writeSize == 0) \
    Assert(0);\

    bool  saveUserData(const FullUserData& value, const char* dir = "./data/db");

}


#endif	//_DB_HELPER_H
