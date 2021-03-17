#include "wsensors.h"
#include "Config.h"
#include "Log.h"

Wsensors::Wsensors(/* args */)
{
    last_check=0;
}

Wsensors::~Wsensors()
{
    for (uint8_t h;h<_snsrs.size();h++) delete _snsrs[h];
    _snsrs.clear();
}

void Wsensors::setup(Rtc1302 * r, Valve *v){ 
        rtc=r;
        vlv=v;
if (rtc->getMemory(MEM_ALARM)){
        alarm();
        }
};
 

void Wsensors::alarm()
{
    ALARM=1;
    rtc->setMemory(1,MEM_ALARM);
    vlv->close();
}

void Wsensors::disalarm()
{
    ALARM=0;
    vlv->open();
    rtc->setMemory(0,MEM_ALARM);
    for (uint8_t i = 0; i < _snsrs.size(); i++)
    {
        _snsrs[i]->alarmscount=0;
    }
    logg.logging("Disalarmed!");
}

void Wsensors::alarm_event(uint8_t n){
    if (n>=0 && n< _snsrs.size())
    {
        _snsrs[n]->alarmscount++;
        if (_snsrs[n]->alarmscount>=max_alarms){
        alarm();
        logg.logging("Alarm!!! Check "+_snsrs[n]->name+" sensor");
        }
    }
}

void Wsensors::disalarm_event(uint8_t n){
    if (n>=0 && n< _snsrs.size())
    {
        _snsrs[n]->alarmscount=0;
    }
}

void Wsensors::addSensor(uint8_t pin,String name){
sensor_t * s = new sensor_t();
s->name=name;
s->pin=pin;
s->alarmscount=0;
pinMode(pin,INPUT_PULLUP);
_snsrs.push_back(s);
}



void Wsensors::processSensors(long ms){
    if (ms-last_check<check_time) return;
    last_check=ms;
    if (ALARM>0) return;
   for (uint8_t i = 0; i < _snsrs.size(); i++)
    {
        if ( digitalRead(_snsrs[i]->pin)==LOW)
        alarm_event(i);
        else
        disalarm_event(i);
    } 
    
}