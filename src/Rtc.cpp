
#include "Rtc.h"
#include "Config.h"
#include "Log.h"

Rtc1302::Rtc1302()
{
    _short_interval = 120000;
    last_update = 0;
    upd_success = false;
}

Rtc1302::~Rtc1302()
{

    if (_rtc != NULL)
        delete (_rtc);
    if (_tw != NULL)
        delete (_tw);
    if (timeClient != NULL)
        delete (timeClient);
    if (ntpUDP != NULL)
        delete (ntpUDP);
}

///qwq  e
bool Rtc1302::settime(uint8_t offset)
{

    return true;
}

void Rtc1302::setup(int interval)
{
    _tw = new ThreeWire(DS1302_DAT, DS1302_CLK, DS1302_RST); // IO, SCLK, CE

    _rtc = new RtcDS1302<ThreeWire>(*_tw);
    _interval = interval;
    _short_interval = 1000 * 240; //5 min

    _rtc->Begin();
    _rtc->SetIsRunning(true);
    _rtc->SetIsWriteProtected(false);
    initNtp();
}

void Rtc1302::processRtc(unsigned long ms)
{

    if (ms < last_update)
        last_update = ms;
    if (ms - last_update > (upd_success ? _interval : _short_interval))
    {
        if (!timeClient)
        {
            initNtp();
            if (!timeClient)
                return;
        }
        upd_success = timeClient->forceUpdate();
        if (upd_success)
        {
            logg.logging("ms=" + String(ms) + " Last=" + String(last_update));
            setfrominet();
        }
        else
        {
            logg.logging("Error update time.");
        }
        last_update = ms;
    }
}

void Rtc1302::setfrominet()
{
    RtcDateTime d;
    d.InitWithEpoch64Time(timeClient->getEpochTime());
    logg.logging("Success update " + toString(d));
    _rtc->SetDateTime(d);
}

bool Rtc1302::initNtp()
{
    if (!ntpUDP)
        ntpUDP = new WiFiUDP();
    timeClient = new NTPClient(*ntpUDP, ntp_server, 3600 * TIME_OFFSET, _interval);
    timeClient->begin();
    return timeClient != NULL;
}

String Rtc1302::toString(const RtcDateTime &dt)
{

    char datestring[20];
    String s;

    snprintf_P(datestring,
               20,
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.Day(),
               dt.Month(),
               dt.Year(),
               dt.Hour(),
               dt.Minute(),
               dt.Second());

    return String(datestring);
}

void Rtc1302::setMemory(uint8_t d, uint8_t addr)
{
    _rtc->SetMemory(addr,d);
}

uint8_t Rtc1302::getMemory(uint8_t addr)
{
    return _rtc->GetMemory(addr);
}

String Rtc1302::test()
{
    String res = timestring();

    return res;
}

String Rtc1302::timestring()
{
    if (_rtc->IsDateTimeValid())
        return toString(_rtc->GetDateTime());
    else
        return "Incorrect DateTime";
}

bool Rtc1302::check_time(uint8_t d, uint8_t h)
{
    if (_rtc->IsDateTimeValid())
    {
        RtcDateTime dt = _rtc->GetDateTime();
        return (d == dt.Day() && h == dt.Hour());
    }
    else
        return false;
}
