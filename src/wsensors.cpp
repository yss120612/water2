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
        pwr_pin=LED_PIN;
        pwr=100;
        measured=false;
        if (pwr_pin!=0){
            pinMode(pwr_pin,OUTPUT);
            digitalWrite(pwr_pin,LOW);
        }



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
        if (pwr_pin>0) digitalWrite(pwr_pin,LOW);
        measured=false;   
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



void Wsensors::processSensors(unsigned long ms){
    
    if (wp_sys->isALARM()) return;
    if (last_check>ms) {last_check=ms;return;}

    if (pwr_pin>0){
    if (ms-last_check<(check_time-pwr_forward)) return;
    pwr=digitalRead(pwr_pin);
    if (pwr==LOW && !measured) 
    {
       digitalWrite(pwr_pin,HIGH);
       return;
    }
    if (pwr==HIGH && measured){
        last_check=ms;
        digitalWrite(pwr_pin,LOW);
        measured=false;
        return;
    }
    if (ms-last_check<check_time) return;
    measured=true;

    }else{
     if (ms-last_check<check_time) return;   
     last_check=ms;
    }
    

   for (uint8_t i = 0; i < _snsrs.size(); i++)
    {
        //logg.logging("i="+String(i)+" level="+String(digitalRead(_snsrs[i]->pin)));
        if ( digitalRead(_snsrs[i]->pin)==level)
        alarm_event(i);
        else
        disalarm_event(i);
    } 
    
    
}