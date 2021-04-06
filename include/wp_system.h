#ifndef __WP_SYSTEM_H
#define __WP_SYSTEM_H\
#include <Arduino.h>
#include "Rtc.h"
#include "valve.h"
#include "wsensors.h"

class WP_system
{
    public:
    WP_system();
    ~WP_system();
    void setup(Valve * v, Wsensors * w, Rtc1302 * r);
    void process(long ms);
   
    private:
    Valve * vlv;
    Rtc1302 * rtc;
    Wsensors * ws;
};

#endif