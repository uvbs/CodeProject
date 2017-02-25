////////////////////////////////////////////////////////////////////////////////////
// 
// @file: observer.cpp
// @author: by Mr.Chen
// @date: 2015/5/2	01:52
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "observer.h"

namespace observer_pattern
{

//WeatherStation
void test()
{
    WeatherData* weatherData = new WeatherData();
    CurrentConditionsDisplay* currDisplay = new CurrentConditionsDisplay(weatherData);
    weatherData->setMeasurements(80,65,30.4f);
    weatherData->setMeasurements(82,70,29.2f);
    weatherData->setMeasurements(78,90,29.2f);
}

} // namespace observer_pattern
