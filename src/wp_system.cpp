#include "wp_system.h"
#include "Log.h"

WP_system::WP_system(){

}

WP_system::~WP_system(){

}

void WP_system::setup(Valve * v, Wsensors * w, Rtc1302 * r){
    vlv=v;
    ws=w;
    rtc=r;
    rtc->setup();  
    vlv->setup(this);
    ws->setup(this,LOW);
    if (rtc->getMemory(MEM_VALVE)>0)
    {
        open_valve();
    }
    else
    {
     close_valve();
    }
    ALARM=0;
    if (rtc->getMemory(MEM_ALARM)){
        alarm(rtc->getMemory(MEM_ALARM));
    }

}

void WP_system::process(long ms){
    ws->processSensors(ms);
    vlv->processValves(ms);
    rtc->loop(ms);
    if(ms-last_time>CHECK_TIME){//one
        last_time=ms;
        if (rtc->check_time(1,1)) vlv->swc();// in 1 st day of month in 1 hour of night
    }

}

// void WP_system::save_valve(bool open){
//     rtc->setMemory(open?1:0,MEM_VALVE);
// }

void WP_system::close_valve()
{
    rtc->setMemory(0,MEM_VALVE);
    vlv->close();
}

void WP_system::open_valve()
{
    rtc->setMemory(0,MEM_VALVE);
    vlv->close();
}


void WP_system::switch_valve()
{
    vlv->swc();
}


void WP_system::alarm(uint8_t sensor_no){
    ALARM=1;
    rtc->setMemory(sensor_no+1,MEM_ALARM);
    close_valve();
    //logg.logging("Alarm!");
}

void WP_system::disalarm(){
    ALARM=0;
    open_valve();
    rtc->setMemory(0,MEM_ALARM);
    ws->disalarm();
    logg.logging("Disalarmed!");
    }
