#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/logger.h>
#include "log4cplus.h"
#include "string_util.h"

using log4cplus::Logger;

WX_IMPLEMENT_SINGLEON(LogManager);

bool LogManager::init(const char* config_file)
{
    log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_C_STR_TO_TSTRING(config_file));
    return true;
}

void LogManager::log(LOG_CPLUS_FILE_TYPE type, LOG_CPLUS_LEVEL level, const char* fmt, ...)
{
    if (type <= LOG_CPLUS_FILE_INVALID || type >= LOG_CPLUS_FILE_MAX || fmt == NULL) {
        return;
    }
    Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_C_STR_TO_TSTRING(log_cplus_file_name[type]));
    if (level < logger.getLogLevel()) {
        return;
    }
    char buffer[MAX_LOG_LEN] = {0};
    memset(buffer,0,MAX_LOG_LEN);
    va_list arg_list;
    va_start(arg_list, fmt);
    str_util::snprintf(buffer, sizeof(buffer), fmt, arg_list);
    va_end(arg_list);
    write_log(logger, type, level, buffer);
}

void LogManager::log(LOG_CPLUS_FILE_TYPE type, LOG_CPLUS_LEVEL level, void* data, int len)
{
    if (type <= LOG_CPLUS_FILE_INVALID || type >= LOG_CPLUS_FILE_MAX || 
        data == NULL || len <= 0 ) {
        return;
    }
    Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_C_STR_TO_TSTRING(log_cplus_file_name[type]));
    if (level < logger.getLogLevel()) {
        return;
    }
    string str = "";
    int index = 0;
    char* p = (char*)data;
    while (len-- > 0)
    {
        if (index != 0 && index % 2 == 0) {
            str += " ";
        }
        else if( index != 0 && index % 16 == 0) {
            str += "\n";
        }
        char tmp[4] = {0};
        snprintf(tmp, 4, "%02hhX", p[index]);
        str += tmp;
        index++;
    }
    write_log(logger, type, level, str.c_str());
}

void LogManager::write_log(log4cplus::Logger& logger, LOG_CPLUS_FILE_TYPE type, 
                           LOG_CPLUS_LEVEL level, const char* data)
{
    switch (level)
    {
        case LOG_CPLUS_LEVEL_OFF:
            break;
        case LOG_CPLUS_LEVEL_FATAL:
            {
                LOG4CPLUS_FATAL(logger, data);
            }
            break;
        case LOG_CPLUS_LEVEL_ERROR:
            {
                LOG4CPLUS_ERROR(logger, data);
            }
            break;
        case LOG_CPLUS_LEVEL_WARN:
            {
                LOG4CPLUS_WARN(logger, data);
            }
            break;
        case LOG_CPLUS_LEVEL_INFO:
            {
                LOG4CPLUS_INFO(logger, data);
            }
            break;
        case LOG_CPLUS_LEVEL_DEBUG:
            {
                LOG4CPLUS_DEBUG(logger, data);
            }
            break;
        case LOG_CPLUS_LEVEL_TRACE:
            {
                LOG4CPLUS_TRACE(logger, data);
            }
            break;
        default:
            break;
    }
}
