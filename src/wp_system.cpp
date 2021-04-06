#include "wp_system.h"

WP_system::WP_system(){

}

WP_system::~WP_system(){

}

void WP_system::setup(Valve * v, Wsensors * w, Rtc1302 * r){
    vlv=v;
    ws=w;
    rtc=r;
    rtc->setup();  
    vlv->setup(rtc);
    ws->setup(rtc, vlv);
}

void WP_system::process(long ms){
    ws->processSensors(ms);
    vlv->processValves(ms);
    rtc->loop(ms);

}