#ifndef __WP_SYSTEM_H
#define __WP_SYSTEM_H\
#include <Arduino.h>
#include "Rtc.h"
#include "valve.h"
#include "wsensors.h"
#include "Speaker.h"


class Valve;
class Rtc1302;
class Wsensors;
class MqttClient;

class WP_system
{
    public:
    WP_system();
    ~WP_system();
    void setup(Valve * v, Wsensors * w, Rtc1302 * r,Speaker * s, MqttClient * mq);
    void process(unsigned long ms);
    
    void alarm(uint8_t sensor_no);
    bool open_valve();
    bool close_valve();
    bool switch_valve();
    void disalarm();
    uint8_t isALARM(){return ALARM;}
    bool valve_is_open();
    private:
    const long CHECK_TIME=1000*60*60*24;//one hour
    unsigned long last_time;
    uint8_t ALARM;
    Valve * vlv;
    Rtc1302 * rtc;
    Wsensors * ws;
    Speaker * speaker;
    MqttClient * mqtt;
};

#endif