#include "wp_system.h"
#include "Log.h"
#include "MQTTclient.h"
#include "Config.h"
WP_system::WP_system()
{
}

WP_system::~WP_system()
{
}

bool WP_system::valve_is_open()
{
    return vlv->is_open();
    //return true;
}

void WP_system::setup(Valve *v, Wsensors *w, Rtc1302 *r,Speaker * s, MqttClient *mq)
{
    vlv = v;
    ws = w;
    rtc = r;
    speaker=s;
    mqtt = mq;

    rtc->setup();
    vlv->setup(PIN_VOPEN, PIN_VCLOSE, LOW, this);
    ws->setup(this, LOW);
    speaker->setup(SPEAKER_PIN);
    if (mqtt) mqtt->reconnect();
    ALARM = 0;
    //logg.logging("ALARM="+String(rtc->getMemory(MEM_ALARM)));
    //logg.logging("VALVE="+String(rtc->getMemory(MEM_VALVE)));
    if (rtc->getMemory(MEM_ALARM))
    {

        alarm(rtc->getMemory(MEM_ALARM) - 1);
    }

    if (mqtt)
        mqtt->alarm();
    if (ALARM) return;
    if (rtc->getMemory(MEM_VALVE) > 0)
    {
        open_valve();
    }
    else
    {
        close_valve();
    }
}

void WP_system::process(unsigned long ms)
{
    vlv->processValves(ms);
    ws->processSensors(ms);
    rtc->processRtc(ms);
    speaker->processSpeaker(ms);
    if (ms < last_time) last_time=ms;
    if (ms - last_time > CHECK_TIME)
    { //one per hour
        last_time = ms;
        if (rtc->check_time(1, 1))
            vlv->swc(); // in 1 st day of month in 1 hour of night
    }
}

bool WP_system::close_valve()
{
    if (!vlv->close())
        return false;
    rtc->setMemory(0, MEM_VALVE);
    if (mqtt)
        mqtt->setValve(false);
    return true;
}

bool WP_system::open_valve()
{
    bool result = false;
    if (ALARM > 0)
    {
        logg.logging("Can`t open when alarm at " + ws->getSensorName(ALARM - 1) + "!!!");
        return result;
    }
    else
    {
        result = vlv->open();
    }
    if (mqtt)
        mqtt->setValve(result);
    if (result)
        rtc->setMemory(1, MEM_VALVE);
    return result;
}

bool WP_system::switch_valve()
{
    if (ALARM)
    {
        logg.logging("Can`t switch when alarm at " + ws->getSensorName(ALARM - 1) + "!!!");
        return false;
    }
    return vlv->swc();
}

void WP_system::alarm(uint8_t sensor_no)
{
    if (!close_valve())
        return;
    ALARM = sensor_no + 1;
    rtc->setMemory(ALARM, MEM_ALARM);
    if (mqtt)
        mqtt->alarm();
    speaker->activate(true);    
    logg.logging("Alarm!!! Check " + ws->getSensorName(sensor_no) + " sensor");
}

void WP_system::disalarm()
{
    ALARM = 0;
    open_valve();
    rtc->setMemory(0, MEM_ALARM);
    ws->disalarm();
    if (mqtt) mqtt->alarm();
    speaker->activate(false);
    logg.logging("Disalarmed!");
}
