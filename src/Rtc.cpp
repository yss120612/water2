
#include "Rtc.h"
#include  "Config.h"
#include  "Log.h"

Rtc1302::Rtc1302(){
_short_interval=120000;
last_update=0;
upd_success=false;
}

Rtc1302::~Rtc1302(){

if (_rtc!=NULL) delete(_rtc);
if (_tw!=NULL) delete(_tw);
if (timeClient!=NULL) delete(timeClient);
if (ntpUDP!=NULL) delete(ntpUDP);
}

///qwq  e
bool Rtc1302::settime(uint8_t offset){
    
return true;
}

void Rtc1302::setup(int interval){
_tw=new ThreeWire(DS1302_DAT,DS1302_CLK,DS1302_RST);// IO, SCLK, CE   
//ThreeWire myWire(DS1302_DAT,DS1302_CLK,DS1302_RST); 

_rtc= new RtcDS1302<ThreeWire>(*_tw);
_interval=interval;
ntpUDP=new WiFiUDP();

timeClient = new NTPClient(*ntpUDP ,ntp_server , 3600*TIME_OFFSET, _interval);
timeClient->begin();
//RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
timeClient->begin();
_rtc->Begin();
_rtc->SetIsRunning(true);
_rtc->SetIsWriteProtected(false);
//logg.logging(toString(compiled));

}

void Rtc1302::loop(long ms)
{
    if (!timeClient)
        return;
   
    if (upd_success)
    {
        if (ms - last_update > _interval)
        {
            upd_success = timeClient->forceUpdate();
            if (upd_success)
            {
              
                setfrominet();
            }else 
            {
                logg.logging("Error update time on long period");
            }
            last_update=ms;
        }
    }
    else
    {
        if (ms - last_update > _short_interval)
        {
            upd_success = timeClient->forceUpdate();
            if (upd_success)
            {
             
                setfrominet();
            }else 
            {
                
                logg.logging("Error update time on short period");
            }
            last_update=ms;
        }
    }
}

void Rtc1302::setfrominet(){
    RtcDateTime d;
    
    d.InitWithEpoch64Time(timeClient->getEpochTime());
    logg.logging("Success update " +toString(d));
    _rtc->SetDateTime(d);
}



String Rtc1302::toString(const RtcDateTime& dt)
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
            dt.Second() );

    return String(datestring);
}

bool Rtc1302::setMemory(uint8_t d,uint8_t addr){
    _rtc->SetMemory(&d,addr);
}

uint8_t Rtc1302::getMemory(uint8_t addr){
    _rtc->GetMemory(addr);
}

String Rtc1302::test(){
String res=timestring();

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
    if (_rtc->IsDateTimeValid()){
        RtcDateTime dt=_rtc->GetDateTime();
        return (d==dt.Day() && h==dt.Hour());
    }
    else
        return false;
    
}


