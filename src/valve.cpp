#include "valve.h"
#include "Log.h"

Valve::Valve()
{
   
}

void Valve::setup(uint8_t OP, uint8_t CL, uint8_t lv, WP_system * wp)
{
    in_progress=false;
    action=RELAXED;
    status=CLS;
    wp_sys=wp;
    VOPEN=OP;
   VCLOSE=CL;
   pinMode(VOPEN,OUTPUT);
   pinMode(VCLOSE,OUTPUT);
   level=lv;
   digitalWrite(VOPEN,!level);
   digitalWrite(VCLOSE,!level);
}

Valve::~Valve()
{
}

bool Valve::open()
{
    if (action!=RELAXED) return false;
    action=INOPEN;
    digitalWrite(VOPEN,level);
    run();
    logg.logging("open");
    return true;
}

bool Valve::close()
{
    if (action!=RELAXED) return false;
    action=INCLOSE;
    digitalWrite(VCLOSE,level);
    run();
    logg.logging("close");
    return true;
}

bool Valve::swc()
{
    if (status!=OPN || action!=RELAXED) return false;
    action=INSWITCH;
    digitalWrite(VCLOSE,level);
    run();
    return true;
}

void Valve::run()
{
    in_progress=true;
    start_action_time=millis();
}

void Valve::stop()
{
    in_progress=false;
    digitalWrite(VOPEN,!level);
    digitalWrite(VCLOSE,!level);
    switch (action)
    {
    case INOPEN:
        status=OPN;
        action=RELAXED;
        break;
    case INCLOSE:
        status=CLS;
        action=RELAXED;
        break;
    case INSWITCH:
        status=CLS;
        action=RELAXED;
        open();
        break;
    default:
        break;
    }
    
}

void Valve::processValves(unsigned long ms){
    
    if (!in_progress) return;
    if (start_action_time>ms) start_action_time=ms;
    if (ms-start_action_time>ACTION_TIME){
        stop();
    }
}