

#include <Arduino.h>

#include "Log.h"
#include "HttpHelper.h"
#include "MQTTclient.h"

#include "wp_system.h"

#include "Speaker.h"
#include "Rtc.h"
#include "Buttons.h"
#include "wsensors.h"
#include "valve.h"
#include "Config.h"

std::array<char *, 2> WIFI_SSID{"Yss_GIGA", "academy"};
std::array<char *, 2> WIFI_PASS{"bqt3bqt3", "123qweasd"};

const char *fw = "Running firmware v. 2.3";

unsigned long msWiFi;
boolean forceWiFi; //если не задалось с первого раза повторять каждые Х минут или нет
//BandLED band;
extern boolean connect2WiFi();
unsigned long ms;

WP_system wp_sys;
HttpHelper *http_server;
MqttClient *mqtt;
Rtc1302 rtc;
Buttons btns;
Wsensors wsens;
Speaker spk;
Valve valve;

void setup()
{

  // put your setup code here, to run once:
  Serial2.end();
#ifdef _SERIAL
  Serial.begin(115200);
  logg.logging("_SERIAL is defined");
#else
  logg.logging("_SERIAL is NOT defined");
#endif
  logg.logging(fw);

  wsens.addSensor(SENSOR1, "Kuhnya");
  wsens.addSensor(SENSOR2, "Shahta");
  wsens.addSensor(SENSOR3, "Vanna");
  wsens.addSensor(SENSOR4, "Tualett");

  //pinMode(2,OUTPUT);

  btns.add(BTN_PIN, LOW);

  forceWiFi = true;
  if (connect2WiFi())
  {

    http_server = new HttpHelper();
    http_server->setup(&wp_sys);

    mqtt = new MqttClient();
    mqtt->setup(&wp_sys);
    logg.setup(mqtt);
    msWiFi = 0;
  }

  ms = 0;
  wp_sys.setup(&valve, &wsens, &rtc, &spk, mqtt);
}
void info()
{
  logg.logging(fw);
  if (WiFi.status() != WL_CONNECTED)
  {
    logg.logging("NO WiFi connected");
  }
  else
  {
    logg.logging("WiFi:" + WiFi.SSID() + " IP:" + WiFi.localIP().toString());
  }
  if (mqtt)
  {
    logg.logging(mqtt->getStatus());
  }
}

void processButtons(long ms)
{
  event_t ev;
  if (btns.getEvent(&ev, ms))
  {
    switch (ev.state)
    {
    case BTN_CLICK:
      logg.logging("CLICK " + String(ev.button) + " count=" + String(ev.count) + " wait=" + String(ms - ev.wait_time) + " millis=" + String(ms));
      if (ev.count == 2 and ev.button == 0)
      {
        wp_sys.disalarm();
      }
      else if (ev.count == 3 and ev.button == 0)
      {
        if (wp_sys.valve_is_open())
        {

          wp_sys.close_valve();
        }
        else
        {
          wp_sys.open_valve();
        }
      }
      else if (ev.count == 4 and ev.button == 0)
      {
        wp_sys.switch_valve();
        logg.logging("swc at " + rtc.timestring());
      }
      break;
    case BTN_LONGCLICK:
      //logg.logging("LONGCLICK "+ String(ev.button)+" count="+String(ev.count));
      if (ev.button == 0)
      {
        info();
      }
      else if (ev.button == 1)
      {
         
      }
      break;
    case BTN_RELEASED:
      //logg.logging("RELEASED "+ String(ev.button));
      break;
    case BTN_DBLCLICK:
      logg.logging("XCLICK " + String(ev.button));
      break;
    case BTN_PRESSED:
      //logg.logging("PRESSED "+ String(ev.button));
      break;
    }
  }
}

void loop()
{

  ms = millis();

  processButtons(ms);

  wp_sys.process(ms);
  delay(5);

  if (mqtt)
    mqtt->loop(ms);

  if (ms - msWiFi > CHECKWIFI)
  {
    msWiFi = ms;
    if (forceWiFi && WiFi.status() != WL_CONNECTED)
    {
      if (connect2WiFi())
      {
        if (!http_server)
        {
          http_server = new HttpHelper();
          http_server->setup(&wp_sys);
        }
        if (!mqtt)
        {
          mqtt = new MqttClient();
          mqtt->setup(&wp_sys);
          logg.setup(mqtt);
        }
      }
    }
  }
}

boolean connect2WiFi()
{
  uint8_t i, k;
  uint8_t cycles = 0;
  boolean success = false;
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  uint8_t n = WiFi.scanNetworks();
  while (true)
  {
    for (k = 0; k < n; k++)
    {
      for (i = 0; i < WIFI_SSID.size(); i++)
      {
        if (WiFi.SSID(k).equals(WIFI_SSID[i]))
        {
          logg.logging("Connecting to " + String(WIFI_SSID[i]) + " ...");
          WiFi.begin(WIFI_SSID[i], WIFI_PASS[i]);
          delay(7000);
          if (WiFi.status() == WL_CONNECTED)
          {
            success = true;
          }
          else
          {
            logg.logging("Connect to " + String(WIFI_SSID[i]) + " failed");
          }
        }
        if (success)
          break;
      }
      if (success)
        break;
    }

    cycles++;
    if (success || cycles >= 3)
    {
      break; //from while
    }
    delay(5000);
  }

  if (success)
  {
    logg.logging("WiFi connected to " + String(WIFI_SSID[i]) + ". IP address: " + WiFi.localIP().toString());
  }
  else
  {
    logg.logging("WiFi error. Working without network :(");
  }
  return success;
}