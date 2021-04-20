#include "wp_system.h"
#include "Log.h"
#include "MQTTclient.h"

WP_system::WP_system(){

}

WP_system::~WP_system(){

}

bool WP_system::valve_is_open()
{
    return vlv->is_open();
    //return true;
}

void WP_system::setup(Valve * v, Wsensors * w, Rtc1302 * r, MqttClient * mq){
    vlv=v;
    ws=w;
    rtc=r;
    mqtt=mq;

    rtc->setup();  
    vlv->setup(this);
    ws->setup(this,LOW);
        
    ALARM=0;
    if (rtc->getMemory(MEM_ALARM)){
    
       alarm(rtc->getMemory(MEM_ALARM)-1);
    
    }
    if (mqtt) mqtt->alarm();

    if (rtc->getMemory(MEM_VALVE)>0)
    {
       open_valve();
    }
    else
    {
       close_valve();
    }
    
}

void WP_system::process(long ms){
    ws->processSensors(ms);
    vlv->processValves(ms);
    rtc->loop(ms);
    if(ms-last_time>CHECK_TIME){//one per hour
        last_time=ms;
        if (rtc->check_time(1,1)) vlv->swc();// in 1 st day of month in 1 hour of night
    }
}

// void WP_system::save_valve(bool open){
//     rtc->setMemory(open?1:0,MEM_VALVE);
// }

bool WP_system::close_valve()
{
    if (!vlv->close()) return false;
    rtc->setMemory(0,MEM_VALVE);
    if (mqtt) mqtt->setValve(false);
    return true;
}

bool WP_system::open_valve()
{
    bool result=false;
    if (ALARM>0) {
        logg.logging("Can`t open when alarm at "+ws->getSensorName(ALARM-1)+"!!!");
    }
    else{
        result=vlv->open();
    }
    if (mqtt) mqtt->setValve(result);
    if (result) rtc->setMemory(1,MEM_VALVE);
    return result;
}


bool WP_system::switch_valve()
{
    if (ALARM){
        logg.logging("Can`t switch when alarm at "+ws->getSensorName(ALARM-1)+"!!!");
        return false;
    }
    return vlv->swc();
}


void WP_system::alarm(uint8_t sensor_no){
    if (!close_valve()) return;
    ALARM=sensor_no+1;
    rtc->setMemory(ALARM,MEM_ALARM);
    if (mqtt) mqtt->alarm();
    logg.logging("Alarm!!! Check " + ws->getSensorName(sensor_no) + " sensor");
}

void WP_system::disalarm(){
    ALARM=0;
    open_valve();
    rtc->setMemory(0,MEM_ALARM);
    ws->disalarm();
    mqtt->alarm();
    logg.logging("Disalarmed!");
    }
