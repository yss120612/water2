#include "valve.h"

Valve::Valve(uint8_t OP, uint8_t CL)
{
   VOPEN=OP;
   VCLOSE=CL;
   
}

void Valve::setup(Rtc1302 * r)
{
    pinMode(VOPEN,OUTPUT);
    pinMode(VCLOSE,OUTPUT);
    in_progress=false;
    action=RELAXED;
    status=CLS;
    rtc=r;
    if (rtc->getMemory(MEM_VALVE)>0)
    {
     open();
    }
    else
    {
     close();
    }
}



Valve::~Valve()
{
}

void Valve::open()
{
    action=INOPEN;
    pinMode(VOPEN,HIGH);
    rtc->setMemory(1,MEM_VALVE);
    run();
}

void Valve::close()
{
    action=INCLOSE;
    pinMode(VCLOSE,HIGH);
    rtc->setMemory(0,MEM_VALVE);
    run();
}

void Valve::swc()
{
    if (status!=OPN) return;
    pinMode(VCLOSE,HIGH);
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
    pinMode(VOPEN,LOW);
    pinMode(VCLOSE,LOW);
    action=RELAXED;
    switch (action)
    {
    case INOPEN:
        status=OPN;
        break;
    case INCLOSE:
        status=CLS;
        break;
    case INSWITCH:
        status=CLS;
        open();
        break;
    default:
        break;
    }
}

void Valve::processValves(long m){
    
    if(m-last_time>CHECK_TIME){
        last_time=m;
        if (rtc->check_time(1,1)) swc();// in 1 st day of month in 1 hour of night
    }
    if (!in_progress) return;
    if (m-start_action_time>ACTION_TIME){
        stop();
    }
}