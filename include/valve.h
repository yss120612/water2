#ifndef __VALVE_H
#define __VALVE_H

#include <Arduino.h>
#include "Config.h"
#include "wp_system.h"

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

class WP_system;

class Valve
{
private:
    uint8_t VOPEN;
    uint8_t VCLOSE;
    bool in_progress;
    const uint16_t ACTION_TIME=1000*8;//8 second
    //const long CHECK_TIME=1000*60*60*24;//one hour
    long start_action_time;
    //long last_time;
    uint8_t level;
    ValveStatus status;
    ValveAction action;
    void run();
    void stop();
    WP_system * wp_sys;
public:
void setup(WP_system * wp);
void open();
void swc();
void close();
void processValves(long m);
    Valve(uint8_t OP, uint8_t CL, uint8_t lv=HIGH);
    ~Valve();
};


#endif