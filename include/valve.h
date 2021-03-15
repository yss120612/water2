#ifndef __VALVE_H
#define __VALVE_H

#include <Arduino.h>
#include "Rtc.h"
#include "Config.h"

enum ValveStatus {
    OPN,
    CLS
};


enum ValveAction {
    RELAXED,
    INOPEN,
    INCLOSE,
    INSWITCH
};


class Valve
{
private:
    uint8_t VOPEN;
    uint8_t VCLOSE;
    bool in_progress;
    const uint16_t ACTION_TIME=1000*8;//8 second
    const long CHECK_TIME=1000*60*60*24;//one hour
    long start_action_time;
    long last_time;
    ValveStatus status;
    ValveAction action;
    void run();
    void stop();
    Rtc1302 * rtc;
public:
void setup(Rtc1302 * r);
void open();
void swc();
void close();
void processValves(long m);
    Valve(uint8_t OP, uint8_t CL);
    ~Valve();
};


#endif