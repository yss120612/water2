#include "valve.h"
#include "Log.h"

Valve::Valve(uint8_t OP, uint8_t CL, uint8_t lvl)
{
   VOPEN=OP;
   VCLOSE=CL;
   pinMode(VOPEN,OUTPUT);
   pinMode(VCLOSE,OUTPUT);
   level=lvl;
   digitalWrite(VOPEN,!level);
   digitalWrite(VCLOSE,!level);
}

void Valve::setup(WP_system * wp)
{
    in_progress=false;
    action=RELAXED;
    status=CLS;
    wp_sys=wp;
}

Valve::~Valve()
{
}

void Valve::open()
{
    action=INOPEN;
    digitalWrite(VOPEN,level);
    run();
}

void Valve::close()
{
    action=INCLOSE;
    digitalWrite(VCLOSE,level);
    run();
}

void Valve::swc()
{
    if (status!=OPN) return;
    digitalWrite(VCLOSE,level);
    action=INSWITCH;
    run();
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

void Valve::processValves(long m){
    
    if (!in_progress) return;
    if (m-start_action_time>ACTION_TIME){
        stop();
    }
}