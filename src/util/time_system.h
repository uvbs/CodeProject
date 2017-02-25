////////////////////////////////////////////////////////////////////////////////////
// 
// @file: time_system.h
// @author: by Mr.Chen
// @date: 2014/8/3	20:44
// @brief: 时间帮助函数
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __TIME_SYSTEM_H_
#define __TIME_SYSTEM_H_

#include <time.h>
#include <string>
#include "type.h"
#include "singleton.h"

#ifdef _LINUX64
#include <sys/utsname.h>
#include <sys/time.h>
#include <unistd.h>
#endif

namespace sys_util {

class TimeSystem : public Singleton<TimeSystem>
{
public:
	void	start();			//开始计时
	void  tick();

	// 取得今日详细时间（年月日时分秒星期）
	int		getYear()			{ return (_tm.tm_year + 1900); };	//年
	int		getMonth()		{ return (_tm.tm_mon + 1); };		//月
	int		getDay()			{ return (_tm.tm_mday); };			//日
	int		getHour()			{ return (_tm.tm_hour); };				//时
	int		getMinute()		{ return (_tm.tm_min); };				//分
	int		getSecs()			{ return (_tm.tm_sec); };				//秒
	int		getWeek()		    { return (_tm.tm_wday); };			//星期，0表示星期日

public:
	uint	getToday();								//获取当前日期（精确到天，例如：20120621 表示2012年6月21号）
    uint	getCurrHourMinute();				    //获取当前日期（精确到分钟，例如：1206211930 表示12年6月21号19点半）
    int		getYearDay();								 //获得当前日期（年+当前天数，例如：12001表示2012年1月1日）
    int		getYearDay(int year, int month, int day);//把日期格式（年月日）转换成日期格式（年+当前天数），例如：20120101转成12001
	
	bool	isLeap(int year) const;						//是否闰年
	uint	getYesterday();								//获得昨天日期（例如：20120620）
	int		getBeforeNDays(int date, int n) const;	 //获取前n天（例如：20120621-2, 返回 20120619）
	int		getAfterNDays(int date, int n) const;	 //获取后n天（例如：20120619+2, 返回 20120621）

	int		getDayDistance(int date1, int date2) const;  //计算2个日期差（例如:20120606 - 20120605, 返回1）
	int		getYearDayDistance(int x1, int x2) const; //计算（年+当前天数）格式表示的两个日期之间相隔天数

public:
	uint		getRunTime();	//获得服务器运行时间
    uint64	getSystemTime();
    uint64	getTimeMicroSecs(); // get a 64 bit time
    uint64	getTimeMilliSecs(); // in ms
    uint64	getTimeSecs(); // in secs
    uint32	getTimeSecs32();

	void		getTimeTM();	//保存格式化时间
    void		getTimestamp(char* datebuf, int size);
    void		getTimestamp(char* datebuf, int size, uint64 ms);
    std::string		getCurrDayTimeStr(const time_t *timer);
    std::string		getCurrDayStr(const time_t *timer);

	void sleep(uint64 ms);

private:
	uint64 _microSecs;		//从1970到现在的微秒数
	tm _tm;						//格式化时间

#if defined(_WIN32)
	uint _startTime;			//服务启动时间

#elif defined(_LINUX64)
	//struct timeval _startTime;
	//struct timezone _tz;
	struct timespec _startTime;

#endif

};

} // namespace sys_util
using namespace sys_util;

#endif // __TIME_SYSTEM_H_
