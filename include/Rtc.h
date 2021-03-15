// Rtc.h
#ifndef _RTC_h
#define _RTC_h
#include "arduino.h"

//#include  "Config.h"

#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include <WiFiUdp.h>
#include <NtpClient.h>

class Rtc1302
{
public:
Rtc1302();
~Rtc1302();
void setup(int interval= 60000*60*24);//default one per day
void loop(long ms);
bool settime(uint8_t offset);
String toString(const RtcDateTime& dt);
String timestring();
bool isSuccess(){return upd_success;}
String test();
bool setMemory(uint8_t d,uint8_t addr);
uint8_t getMemory(uint8_t addr);
bool check_time(uint8_t d, uint8_t h);
private:
void setfrominet();

RtcDS1302<ThreeWire> * _rtc;
ThreeWire * _tw;
//RtcDateTime compiled;
int _interval,_short_interval;
long last_update;
bool upd_success;
WiFiUDP * ntpUDP;
NTPClient * timeClient;
};
#endif