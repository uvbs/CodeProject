#include "time_system.h"
#include "assert.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "maths.h"

WX_IMPLEMENT_SINGLEON(sys_util::TimeSystem)

namespace sys_util {

#define GETFULLTIME(year, month, day, hour) (year*1000000 + month*10000 + day*100 + hour)
#define GETYEAR(X) (X/1000000)					//获得年
#define GETMONTH(X) ((X%1000000)/10000)	//获得月
#define GETDAY(X) ((X%10000)/100)				//获得天
#define GETHOUR(X) (X%100)						//获得小时

void TimeSystem::start()
{
	__ENTER_FUNCTION
#if defined(_WIN32)
	_startTime	= GetTickCount();

#elif defined(_LINUX64)
	//gettimeofday(&_tstart,&tz);
	 clock_gettime(CLOCK_REALTIME, &_startTime);
#endif
	tick();
	__LEAVE_FUNCTION
}

void  TimeSystem::tick()
{
	__ENTER_FUNCTION
	getSystemTime();
	getTimeTM();
	__LEAVE_FUNCTION
}

void TimeSystem::getTimeTM()
{
	__ENTER_FUNCTION
	time_t t = _microSecs / 1000000;
#if defined(_WIN32)
	tm* p = localtime(&t) ;
	if (p)
	{
		_tm = *p;
	}

#elif defined(_LINUX64)
	tm  newtm;
	tm* ptm = localtime_r(&t, &newtm);
	if( ptm )
	{
		_tm = newtm;
	}
#endif
	__LEAVE_FUNCTION
}

uint	TimeSystem::getRunTime()
{
	__ENTER_FUNCTION
	uint runTime = 0;
#if defined(_WIN32)
	uint endTime = GetTickCount();
	runTime = endTime - _startTime;

#elif defined(_LINUX64)
	//gettimeofday(&tend,&tz);
	struct timespec endTime;
	clock_gettime(CLOCK_REALTIME, &endTime);
	runTime = (endTime.tv_sec - _startTime.tv_sec) * 1000 + 
		(endTime.tv_nsec - _startTime.tv_nsec) / 1000000;
#endif
	return runTime;
	__LEAVE_FUNCTION
	return 0;
}

uint TimeSystem::getToday()
{
	int year = getYear();
	int month = getMonth();
	int day = getDay();
	return GETFULLTIME(year, month, day, 0)/100;
}

uint	TimeSystem::getCurrHourMinute()
{
	int year = getYear();
	year %= 100; //只要最后两位数字
	int month = getMonth();
	int day = getDay();
	int hour = getHour();
	int minute = getMinute();
	uint uRet = GETFULLTIME(year, month, day, hour);
	uRet = uRet*100 + minute;
	return uRet;
}

bool TimeSystem::isLeap(int year) const
{
	if((year % 4 == 0 && year % 100 != 0) || 
		(year % 400 == 0))
	{
	   return true;
	}
	return false;
}

int TimeSystem::getYearDay()
{
	int year = getYear();
	int month = getMonth();
	int day = getDay();
	return getYearDay(year,month,day);
}

int TimeSystem::getYearDay(int year,int month,int day)
{
	int monthDays[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	int leapMonthDays[] = {31,29,31,30,31,30,31,31,30,31,30,31};
	int sum = 0;

	if (isLeap(year))
	{
		for (int i = 0; i < month - 1; i++) {
			sum += leapMonthDays[i];
		}
	}
	else
	{
		for(int i = 0; i < month - 1; i++) {
			sum += monthDays[i];
		}
	}
	sum += day;
	return (sum + (year - 2000) * 1000);
}

int TimeSystem::getYearDayDistance(int date1, int date2) const
{
	// 例如：date1 = 12001, date2 = 12101
	//		 date1 = 12360, date2 = 13001
	if (date2 < date1)
	{
		swap<int>(date1, date2);
	}
	int year1 = date1 / 1000;
	int year2 = date2 / 1000;
	if ( year1 == year2)
	{//如果同年
		return date2 - date1;
	}
	else
	{//如果不同年
		int ret1 = 0;
		if (isLeap(year1+2000)) {
			ret1 = 366 - date1 % 1000;
		}
		else {
			ret1 = 365 - date1 % 1000;
		}
		int ret2 = date2 % 1000;
		return ret1 + ret2;
	}
}

uint TimeSystem::getYesterday()
{
	time_t yesterdayTime = _microSecs / 1000000 - 24 * 3600;
	tm yesterTMTime;
#if defined(_WIN32)
	tm* pTMTime = localtime(&yesterdayTime) ;
	yesterTMTime = *pTMTime ;

#elif defined(_LINUX64)
	tm  newtm;
	tm* ptm = localtime_r(&yesterdayTime,&newtm);
	if( ptm )
	{
		yesterTMTime = newtm;
	}
#endif
	int year = yesterTMTime.tm_year + 1900;
	int month = yesterTMTime.tm_mon + 1;
	int day = yesterTMTime.tm_mday;
	return GETFULLTIME(year, month, day, 0)/100;
}

int TimeSystem::getBeforeNDays(int date, int n) const
{
	if ( n < 1) {
		return date;
	}
	date*=100;
	struct tm time1, time2;
	time_t timeT1,timeT2;

	time1.tm_year=GETYEAR(date)-1900;
	time1.tm_mon=GETMONTH(date)-1;
	time1.tm_mday=GETDAY(date);
	time1.tm_hour=0;
	time1.tm_min=0;
	time1.tm_sec=0;
	time1.tm_isdst=-1;
	timeT1=mktime(&time1);
	
	timeT2 = timeT1-n*24*3600;
#if defined(_WIN32)
	tm* pTMTime = localtime( &timeT2 ) ;
	time2 = *pTMTime ;
#elif defined(_LINUX64)
	tm  newtm;
	tm* ptm = localtime_r(&timeT2,&newtm);
	if( ptm ) time2 = newtm;
#endif
	int year = time2.tm_year + 1900;
	int month = time2.tm_mon + 1;
	int day = time2.tm_mday;
	return GETFULLTIME(year, month, day, 0)/100;
}

int TimeSystem::getAfterNDays(int date,int n) const
{
	__ENTER_FUNCTION

	if (n < 1)
	{
		return date;
	}
	date *= 100;
	struct tm time1, time2;
	time_t timeT1, timeT2;

	time1.tm_year = GETYEAR(date)-1900;
	time1.tm_mon = GETMONTH(date)-1;
	time1.tm_mday = GETDAY(date);
	time1.tm_hour = 0;
	time1.tm_min = 0;
	time1.tm_sec = 0;
	time1.tm_isdst = -1;
	timeT1 = mktime(&time1);

	timeT2 = timeT1 + n * 24 * 3600;
#if defined(_WIN32)
	tm* pTMTime = localtime( &timeT2 ) ;
	time2 = *pTMTime ;
#elif defined(_LINUX64)
	tm  newtm;
	tm* ptm = localtime_r(&timeT2,&newtm);
	if( ptm ) time2 = newtm;
#endif

	int year = time2.tm_year + 1900;
	int month = time2.tm_mon + 1;
	int day = time2.tm_mday;
	return GETFULLTIME(year, month, day, 0)/100;

	__LEAVE_FUNCTION
	return 0;
}

int TimeSystem::getDayDistance(int date1, int date2 ) const
{
	__ENTER_FUNCTION

	date1 *= 100;
	date2 *= 100;
	struct tm time1, time2;
	time_t timeT1, timeT2;
	
	int days=0;
	time1.tm_year = GETYEAR(date1) - 1900;
	time1.tm_mon = GETMONTH(date1) - 1;
	time1.tm_mday = GETDAY(date1);
	time1.tm_hour = 0;
	time1.tm_min = 0;
	time1.tm_sec = 0;
	time1.tm_isdst = -1;
	timeT1 = mktime(&time1);

	time2.tm_year = GETYEAR(date2) - 1900;
	time2.tm_mon = GETMONTH(date2) - 1;
	time2.tm_mday = GETDAY(date2);
	time2.tm_hour = 0;
	time2.tm_min = 0;
	time2.tm_sec = 0;
	time2.tm_isdst = -1;
	timeT2 = mktime(&time2);

	days = static_cast<int>(difftime(timeT2, timeT1) / (24 * 60 * 60));
	return days;
	__LEAVE_FUNCTION
	return 0;
}

uint64
TimeSystem::getSystemTime()
{
    Assert( sizeof(uint64) == 64/8 );

#if defined(_WIN32)  
	static const uint64 epoch = 116444736000000000;

	FILETIME ft;
	SYSTEMTIME st;
	ULARGE_INTEGER li;
	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
	//GetSystemTimeAsFileTime(&ft);
	//QuadPart is nansecs
	//time = li.QuadPart/10;
	struct { time_t tv_sec; int tv_usec; } tv = {0,0};
	tv.tv_sec = (long) ((li.QuadPart - epoch) / 10000000L);
	tv.tv_usec = (long) (st.wMilliseconds * 1000);

#else
    struct timeval tv;
    gettimeofday( &tv , NULL );

#endif

	_microSecs = tv.tv_sec *1000000 + tv.tv_usec;
    return _microSecs;
}

uint64 TimeSystem::getTimeMicroSecs()
{
    return getSystemTime();
}

uint64 TimeSystem::getTimeMilliSecs()
{
    return getSystemTime() / 1000;
}

uint64 TimeSystem::getTimeSecs()
{
    return getSystemTime() / 1000000;
}

uint32 TimeSystem::getTimeSecs32()
{
	time_t t;
	time(&t);
	return t;
}

void  TimeSystem::getTimestamp(char* datebuf, int datebufSize)
{
#ifdef _WIN32 
   int result = 1; 
   SYSTEMTIME systemTime;
   struct { time_t tv_sec; int tv_usec; } tv = {0,0};
   time(&tv.tv_sec);
   GetLocalTime(&systemTime);
   tv.tv_usec = systemTime.wMilliseconds * 1000; 

#else 
   struct timeval tv; 
   int result = gettimeofday (&tv, NULL);
#endif

   int outTextLen = 0;
   if (result == -1)
   {
      datebuf[0] = 0;
      return;
   }
   else
   {
      /* The tv_sec field represents the number of seconds passed since
         the Epoch, which is exactly the argument gettimeofday needs. */
      const time_t timeInSeconds = (time_t) tv.tv_sec;
      outTextLen = (int)strftime (datebuf,
                             datebufSize,
							 "%Y-%m-%d %H:%M:%S", /* guaranteed to fit in 256 chars,
                                                 hence don't check return code */
                             localtime (&timeInSeconds));
   }
   
   char msbuf[5] = {0};
   /* Dividing (without remainder) by 1000 rounds the microseconds
      measure to the nearest millisecond. */
   int msLen = sprintf(msbuf, ".%3.3ld", long(tv.tv_usec / 1000));
   memcpy(datebuf + outTextLen, msbuf, msLen);
   datebuf[outTextLen + msLen] = '\0';
}

void TimeSystem::getTimestamp(char* datebuf, int datebufSize, uint64 ms)
{
   const time_t timeInSeconds = ms/1000;
   strftime (datebuf,
             datebufSize,
             "%Y-%m-%d %H:%M:%S", /* guaranteed to fit in 256 chars,
                                 hence don't check return code */
             localtime (&timeInSeconds));
}

void TimeSystem::sleep(uint64 ms)
{
#ifdef _WIN32
   Sleep((DWORD)ms);
#else
   usleep(1000 * ms);   
#endif
}

std::string TimeSystem::getCurrDayTimeStr(const time_t *timer)
{
	char buf[32] = {0};
	time_t long_time;
	if (timer == NULL)
	{
		time( &long_time );                /* Get time as long integer. */
	}
	else
	{
		long_time = *timer;
	}
	struct tm* curtime = localtime(&long_time);
	sprintf(buf,"%04d-%02d-%02d %02d:%02d:%02d",curtime->tm_year+1900,
		++curtime->tm_mon, curtime->tm_mday, curtime->tm_hour+1,
		curtime->tm_min, curtime->tm_sec);
    return std::string(buf);
}

std::string TimeSystem::getCurrDayStr(const time_t *timer)
{
	char buf[32] = {0};
	time_t long_time;
	if (timer == NULL)
	{
		time( &long_time );                /* Get time as long integer. */
	}
	else
	{
		long_time = *timer;
	}
	struct tm* curtime = localtime(&long_time);
	sprintf(buf, "%04d-%02d-%02d", curtime->tm_year + 1900,
		++curtime->tm_mon, curtime->tm_mday);
    return std::string(buf);
}


} // namespace sys_util
