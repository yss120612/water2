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

void Wsensors::setup(WP_system * ws, uint8_t lvl){ 
        //rtc=r;
        //vlv=v;
        wp_sys=ws;
        level=lvl;

};


void Wsensors::disalarm()
{
    for (uint8_t i = 0; i < _snsrs.size(); i++)
    {
        _snsrs[i]->alarmscount=0;
    }
}

void Wsensors::alarm_event(uint8_t n){
    if (n>=0 && n< _snsrs.size())
    {
        _snsrs[n]->alarmscount++;
        if (_snsrs[n]->alarmscount>=max_alarms){
        wp_sys->alarm(n);
        }
    }
}

String Wsensors::getSensorName(uint8_t n){
if (n>=0 && n< _snsrs.size())
    {
        return _snsrs[n]->name;
    }
    else{
        return "Unknoun";
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
pinMode(pin,level?INPUT_PULLDOWN:INPUT_PULLUP);
_snsrs.push_back(s);
}



void Wsensors::processSensors(long ms){
    
    if (ms-last_check<check_time) return;
    last_check=ms;
    if (wp_sys->isALARM()) return;
   for (uint8_t i = 0; i < _snsrs.size(); i++)
    {
        //logg.logging("i="+String(i)+" level="+String(digitalRead(_snsrs[i]->pin)));
        if ( digitalRead(_snsrs[i]->pin)==level)
        alarm_event(i);
        else
        disalarm_event(i);
    } 
    
}