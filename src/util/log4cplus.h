/***************************************************
** @file: log4cplus.h
** @author: Mr.Chen
** @date: 2016/8/1
** @brief: 日志相关类
***************************************************/
#ifndef _LOG_MANAGER_H
#define _LOG_MANAGER_H

#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>
#include "singleton.h"

#define MAX_LOG_LEN  (1024*4)

enum LOG_CPLUS_FILE_TYPE
{
    LOG_CPLUS_FILE_INVALID = -1,
    LOG_CPLUS_FILE_FATAL, 
    LOG_CPLUS_FILE_ERROR,
    LOG_CPLUS_FILE_WARN,
    LOG_CPLUS_FILE_INFO,
    LOG_CPLUS_FILE_DEBUG,
    LOG_CPLUS_FILE_TRACE,
    LOG_CPLUS_FILE_MAX,
};

static const char* log_cplus_file_name[LOG_CPLUS_FILE_MAX] = {
    "fatal_xx", //LOG_CPLUS_FILE_FATAL
    "error_xx", //LOG_CPLUS_FILE_ERROR
    "warn_xx", //LOG_CPLUS_FILE_WARN
    "info_xx", //LOG_CPLUS_FILE_INFO
    "debug_xx", //LOG_CPLUS_FILE_DEBUG
    "trace_xx", //LOG_CPLUS_FILE_TRACE
};

enum LOG_CPLUS_LEVEL
{
    LOG_CPLUS_LEVEL_OFF   = log4cplus::OFF_LOG_LEVEL,
    LOG_CPLUS_LEVEL_FATAL = log4cplus::FATAL_LOG_LEVEL,
    LOG_CPLUS_LEVEL_ERROR = log4cplus::ERROR_LOG_LEVEL,
    LOG_CPLUS_LEVEL_WARN  = log4cplus::WARN_LOG_LEVEL,
    LOG_CPLUS_LEVEL_INFO  = log4cplus::INFO_LOG_LEVEL,
    LOG_CPLUS_LEVEL_DEBUG = log4cplus::DEBUG_LOG_LEVEL,
    LOG_CPLUS_LEVEL_TRACE = log4cplus::TRACE_LOG_LEVEL,
};

#if defined (_LINUX64)
#define LOG_CPLUS_TRACE(fmt, args...) { LogManager::getSinglePtr()->log(LOG_CPLUS_FILE_TRACE, \
    LOG_CPLUS_LEVEL_TRACE, fmt, ##args); }
#define LOG_CPLUS_DEBUG(fmt, args...) { LogManager::getSinglePtr()->log(LOG_CPLUS_FILE_DEBUG, \
    LOG_CPLUS_LEVEL_DEBUG, fmt, ##args); }
#define LOG_CPLUS_DEBUG_BIN(data, len) { LogManager::getSinglePtr()->log(LOG_CPLUS_FILE_DEBUG, \
    LOG_CPLUS_LEVEL_DEBUG, data, len); }
#define LOG_CPLUS_INFO(fmt, args...) { LogManager::getSinglePtr()->log(LOG_CPLUS_FILE_INFO, \
    LOG_CPLUS_LEVEL_INFO, fmt, ##args); }
#define LOG_CPLUS_WARN(fmt, args...) { LogManager::getSinglePtr()->log(LOG_CPLUS_FILE_WARN, \
    LOG_CPLUS_LEVEL_WARN, fmt, ##args); }
#define LOG_CPLUS_ERROR(fmt, args...) { LogManager::getSinglePtr()->log(LOG_CPLUS_FILE_ERROR, \
    LOG_CPLUS_LEVEL_ERROR, fmt, ##args); }
#define LOG_CPLUS_FATAL(fmt, args...) { LogManager::getSinglePtr()->log(LOG_CPLUS_FILE_FATAL, \
    LOG_CPLUS_LEVEL_FATAL, fmt, ##args); }
#endif


class LogManager : public Singleton<LogManager>
{
public:
    // 初始化log处理类，主要是将log_file_type对应的配置文件实例化
    bool init(const char* config_file);
    // 普通字符串日志
    void log(LOG_CPLUS_FILE_TYPE type, LOG_CPLUS_LEVEL level, const char* fmt, ...);
    // 二进制格式化日志
    void log(LOG_CPLUS_FILE_TYPE type, LOG_CPLUS_LEVEL level, void* data, int len);

private:
    void write_log(log4cplus::Logger& logger, LOG_CPLUS_FILE_TYPE type, LOG_CPLUS_LEVEL level, 
        const char* data);

};


#endif //_LOG_MANAGER_H
