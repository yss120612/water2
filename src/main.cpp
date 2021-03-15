

#include <Arduino.h>


#include "Log.h"
#include "HttpHelper.h"
#include "Rtc.h"
#include "Buttons.h"
#include "wsensors.h"
#include "valve.h"
#include "Config.h"

HttpHelper httph;
const char* WIFI_SSID = "Yss_GIGA";
const char* WIFI_PASS = "bqt3bqt3";
const char* fw = "Running firmware v. 2.1";


HttpHelper * http_server;
unsigned long msWiFi;
boolean forceWiFi;//если не задалось с первого раза повторять каждые Х минут или нет
//BandLED band;
extern boolean connect2WiFi();

Rtc1302 rtc;
Buttons btns;
Wsensors wsens;
Valve valve(PIN_VOPEN,PIN_VCLOSE);
//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);//0ne per day
//NTPClient timeClient(ntpUDP);//0ne per day
int8_t timeZone = 8;

const PROGMEM char *ntpServer = "pool.ntp.org";
#define NTP_TIMEOUT 1500



void setup() {
  // put your setup code here, to run once:
#ifdef _SERIAL
	Serial.begin(74800);
	logg.logging("_SERIAL is defined");
#else
	logg.logging("_SERIAL is NOT defined");
#endif
  //pinMode(LED_BUILTIN,OUTPUT);
  //digitalWrite(LED_BUILTIN,HIGH);
 
  
  httph.setup();
  btns.add(BTN_PIN,LOW);
  wsens.addSensor(SENSOR1,"Kuhnya");
  wsens.addSensor(SENSOR2,"Shahta");
  wsens.addSensor(SENSOR3,"Tualett");
  //pinMode(D9,OUTPUT);
  //pinMode(D7,INPUT_PULLUP);
  //btns.add(D8,HIGH);
  rtc.setup();  
  valve.setup(&rtc);
  wsens.setup(&rtc, &valve);


    

  //timeClient.begin();
  //NTP.setInterval (63);
  //NTP.setNTPTimeout (NTP_TIMEOUT);
  //NTP.begin (ntpServer, timeZone, false);

  
}

void processButtons(long ms){
event_t ev;
if (btns.getEvent(&ev,ms)){
  switch (ev.state)
  {
  case BTN_CLICK:
    logg.logging("CLICK "+ String(ev.button)+" count="+String(ev.count)+" wait="+String(ms-ev.wait_time)+ " millis="+String(ms));
    if (ev.count==1){

      wsens.alarm();
    }
    else if (ev.count==2) {
      wsens.disalarm();
    }
    if (ev.count==3) {
      valve.swc();
    }
    break;
  case BTN_LONGCLICK:
    logg.logging("LONGCLICK "+ String(ev.button)+" count="+String(ev.count));
    
    break;
    case BTN_RELEASED:
    //logg.logging("RELEASED "+ String(ev.button));
    break;
    case BTN_DBLCLICK:
    logg.logging("XCLICK "+ String(ev.button));
    break;
    case BTN_PRESSED:
      //logg.logging("PRESSED "+ String(ev.button));
    break;
  }
}
}

void processMQQT(char* topic, byte* payload, unsigned int length) {//callback function
  String res="Message arrived ["+String(topic)+"] :";
  logg.logging("Message arrived ["+String(topic)+"] ");
  for (int i = 0; i < length; i++) {
    res+=(char)payload[i];
  }
  
logg.logging(res);
}



int i=0;
int k=1;

void loop() {
  // put your main code here, to run repeatedly:
  long ms = millis();
  //httph.clientHandle();
  processButtons(ms); 
  wsens.processSensors(ms);
  valve.processValves(ms);
  rtc.loop(ms);
  delay(5);
  i+=5*k;
  if (i>=1020) k=-1;
  else if (i<=5) k=1;
  //analogWrite(LED_BUILTIN,i);
  
   
  if (i==500) {
    //tone(SPEAKER_PIN,1000,200);
   //logg.logging("PinLevel="+String(digitalRead(D7)));
  //if (k==0)    rtc.setup();
  //k=1;
    //logg.logging(timeClient.getFormattedTime());
    //logg.logging(String(timeClient.getMinutes()));
    //i=0;
    
    //digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  }
  if (ms-msWiFi>CHECKWIFI){
    msWiFi=ms;
    if (forceWiFi && WiFi.status()!=WL_CONNECTED){
      connect2WiFi();
    }
  }
}

boolean connect2WiFi(){
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);
    logg.logging("Connecting to "+String(WIFI_SSID));
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    //WiFi.disconnect();
   // delay(100);
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("Connecting...");
    uint8_t cycles=0;
    while (WiFi.status() != WL_CONNECTED) {
      // Check to see if connecting failed.
      // This is due to incorrect credentials
      if (WiFi.status() == WL_CONNECT_FAILED) {
        Serial.println("Failed to connect to WIFI. Please verify credentials: ");
        Serial.print("SSID: ");
        Serial.println(WIFI_SSID);
        Serial.print("Password: ");
        Serial.println(WIFI_PASS);

      }
      delay(5000);
      cycles++;
      if (cycles>4) 
      {
      Serial.println("Working witout WiFi :(");  
      return false;
      break;
      }
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    logg.logging("WiFi connected.IP address: "+WiFi.localIP().toString());
    Serial.println("I'm connected to the internets!!");
    return true;
    
}