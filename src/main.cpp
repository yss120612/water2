

#include <Arduino.h>


#include "Log.h"
#include "HttpHelper.h"
#include "MQTTclient.h""
#include "wp_system.h"

#include "Rtc.h"
#include "Buttons.h"
#include "wsensors.h"
#include "valve.h"
#include "Config.h"


std::array <char*,2> WIFI_SSID {"Yss_GIGA","academy"};
std::array <char*,2> WIFI_PASS {"bqt3bqt3","123qweasd"};

const char* fw = "Running firmware v. 2.2";



unsigned long msWiFi;
boolean forceWiFi;//если не задалось с первого раза повторять каждые Х минут или нет
//BandLED band;
extern boolean connect2WiFi();
long ms;

WP_system wp_sys;
HttpHelper * http_server;
MqttClient * mqtt;
Rtc1302 rtc;
Buttons btns;
Wsensors wsens;
Valve valve(PIN_VOPEN,PIN_VCLOSE,LOW);

//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);//0ne per day
//NTPClient timeClient(ntpUDP);//0ne per day




void setup() {
 
  // put your setup code here, to run once:
#ifdef _SERIAL
	Serial.begin(115200);
	logg.logging("_SERIAL is defined");
#else
	logg.logging("_SERIAL is NOT defined");
#endif
logg.logging(fw);  

 wsens.addSensor(SENSOR1,"Kuhnya");
 wsens.addSensor(SENSOR3,"Shahta");
 wsens.addSensor(SENSOR4,"Tualett");
  
 wp_sys.setup(&valve,&wsens,&rtc);

  //pinMode(2,OUTPUT);
 
 btns.add(BTN_PIN, LOW);

    forceWiFi=true;
    if (connect2WiFi())
    {
      http_server = new HttpHelper();
      http_server->setup(&wp_sys);
      mqtt = new MqttClient();
      mqtt->setup(&wp_sys);
      msWiFi=0;
    }
    ms=0;
  
  //httph.setup();
  //btns.add(BTN_PIN,HIGH);
 
 
  
}

void processButtons(long ms){
event_t ev;
if (btns.getEvent(&ev,ms)){
  switch (ev.state)
  {
  case BTN_CLICK:
    logg.logging("CLICK "+ String(ev.button)+" count="+String(ev.count)+" wait="+String(ms-ev.wait_time)+ " millis="+String(ms));
    if (ev.count==2 and ev.button==0){
      wp_sys.close_valve();
    }
    else if (ev.count==3 and ev.button==0) {
      wp_sys.disalarm();
    }
    if (ev.count==4 and ev.button==0) {
      wp_sys.switch_valve();
      logg.logging("swc at "+rtc.timestring());
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

// void processMQQT(char* topic, byte* payload, unsigned int length) {//callback function
//   String res="Message arrived ["+String(topic)+"] :";
//   logg.logging("Message arrived ["+String(topic)+"] ");
//   for (int i = 0; i < length; i++) {
//     res+=(char)payload[i];
//   }
  
// logg.logging(res);
// }



int i=0;
int k=1;

void loop() {
  // put your main code here, to run repeatedly:
 
  ms = millis();
  //httph.clientHandle();
  processButtons(ms); 
  // wsens.processSensors(ms);
  // valve.processValves(ms);
  // rtc.loop(ms);
  wp_sys.process(ms);
  delay(5);
//   i+=5*k;
//   if (i>=1020) k=-1;
//   else if (i<=5) k=1;
//  digitalWrite(2,digitalRead(BTN_PIN));
  
   
  // if (i==500) {
    //tone(SPEAKER_PIN,1000,200);
   //logg.logging("PinLevel="+String(digitalRead(D7)));
  //if (k==0)    rtc.setup();
  //k=1;
    //logg.logging(timeClient.getFormattedTime());
    //logg.logging(String(timeClient.getMinutes()));
    //i=0;
    
    //digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  // }

 if (mqtt) mqtt->loop(ms);

  if (ms-msWiFi>CHECKWIFI){
    msWiFi=ms;
    if (forceWiFi && WiFi.status()!=WL_CONNECTED){
      if (connect2WiFi())
      {
      if(!http_server)
      {
        http_server = new HttpHelper();
        http_server->setup(&wp_sys);
      }
      if (!mqtt){
        mqtt = new MqttClient();
        mqtt->setup(&wp_sys);
      }
    }
    }
    }
  }


boolean connect2WiFi(){
    uint8_t i=0;
    uint8_t cycles=0;
    boolean success=false;
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    
	
    while (true)
    {
    for (i=0;i<WIFI_SSID.size();i++)
    {
      logg.logging("Connecting to "+String(WIFI_SSID[i])+" ...");
      WiFi.begin(WIFI_SSID[i], WIFI_PASS[i]);
      delay(5000);
      if (WiFi.status() != WL_CONNECTED){
        if (WiFi.status() == WL_CONNECT_FAILED) {
                logg.logging("Failed to connect to WIFI. Please verify credentials SSID: "+String(WIFI_SSID[i])+" Password: "+String(WIFI_PASS[i]));
               }else{//other error
                logg.logging("Connect to "+String(WIFI_SSID[i])+" failed");
              }
        }
          else{
            success=true;
            break;//from for
        }
      }

      cycles++;
      if (success || cycles>=3) 
      {
        break;//from while
      }
      delay(5000);
    }

    if (success)
    {
      logg.logging("WiFi connected to "+String(WIFI_SSID[i])+". IP address: "+WiFi.localIP().toString());
    }
    else{
      logg.logging("WiFi error. Working without network :(");
    }
    return success;
 }