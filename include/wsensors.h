#ifndef __WSENSORS_H
#define __WSENSORS_H

#include <Arduino.h>
#include <QList.h>
#include "Rtc.h"
#include "valve.h"
#include "wp_system.h"

struct sensor_t{
    String name;
    uint8_t pin;
    uint8_t alarmscount;
};

class WP_system;

class Wsensors
{
    public:
    Wsensors(/* args */);
    ~Wsensors();

    void processSensors(unsigned long ms);
    void setup(WP_system *ws, uint8_t lvl);
    void addSensor(uint8_t pin,String name); 
    //bool isALARM(){return ALARM>0;}
    //void alarm();
    void disalarm();
    String getSensorName(uint8_t n);
private:
    const uint8_t max_alarms=3;

    const int check_time=3000; //every 3 sec
    const int pwr_forward=100; //включение питания перед заранее перед замером 
    uint8_t level;
    uint8_t pwr_pin;
    bool measured;
    uint8_t pwr;
    QList<sensor_t *> _snsrs;
    unsigned long last_check;
    void alarm_event(uint8_t n);
    void disalarm_event(uint8_t n);
    WP_system * wp_sys;
};



#endif