////////////////////////////////////////////////////////////////////////////////////
// 
// @file: observer.h
// @author: by Mr.Chen
// @date: 2015/5/2	00:25
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef _OBSERVER_H
#define  _OBSERVER_H

#include "type.h"
#include <typeinfo>
#include <list>
#include <string.h>
#include <stdio.h>

using std::list;
using std::type_info;

namespace observer_pattern
{

class Observable;

//观察者(虚基类)
class Observer
{
public:
    virtual void update(Observable*,void*) = 0;
};

//可观察者(基类)
class Observable
{
public:
    virtual void registerObserver(Observer* obj) {
        _observers.push_back(obj);
    }
    virtual void removeObserver(Observer* obj) {
        _observers.remove(obj);
    }
    virtual void notifyObservers(void* data) {
        if (_changed) {
            for (list<Observer*>::iterator iter=_observers.begin(); iter!=_observers.end(); ++iter) {
                Observer* pObserver = *iter;
                if (pObserver)
                    pObserver->update(this, data);
            }
            _changed = false;
        }
    }
    virtual void notifyObservers() { 
        notifyObservers(NULL);
    }
    void setChanged() { _changed = true;}
    bool hasChanged() {return _changed; }

private:
    list<Observer*> _observers;
    bool _changed;
};

//绘制基类
class DisplayElement
{
public:
    virtual void display() = 0;
};

//可观察者(派生类)
class WeatherData : public Observable
{
public:
    void measurementsChanged() {
        setChanged();
        notifyObservers();
    }
    void setMeasurements(float temperature, float humidity, float pressure) {
        this->_temperature = temperature;
        this->_humidity = humidity;
        this->_pressure = pressure;
        measurementsChanged();
    }
    float getTemperature() const {return _temperature;}
    float getHumidity() const {return _humidity; }
    float getPressure() const {return _pressure;}

private:
    float _temperature;
    float _humidity;
    float _pressure;
};

//观察者(派生类)
class CurrentConditionsDisplay : public Observer, public DisplayElement
{
public:
    CurrentConditionsDisplay(Observable* observable) {
        this->_observable = observable;
        if(observable)
            observable->registerObserver(this);
    }
    void update(Observable* observable, void* data) {
        //if (typeid(*observable)==typeid(WeatherData)) {
        const char* classname = typeid(*observable).name();
        //如果可观察者是WeatherData
		if(::strcmp(classname, typeid(WeatherData).name()) == 0) {
            WeatherData *data = (WeatherData*)observable;
            if (data)
            {
                this->_temperature = data->getTemperature();
                this->_humidity = data->getHumidity();
                display();
            }
        }
    }
    void display() {
        printf("Current conditions: %.2f F degrees and %.2f%% humidity\n", _temperature, _humidity);
    }

private:
    float _temperature;
    float _humidity;
    Observable* _observable;
};

void test();

} //namespace observer_pattern
using namespace observer_pattern;

#endif //_OBSERVER_H
