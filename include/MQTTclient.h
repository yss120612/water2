#ifndef _MQTTCLIENT_h
#define _MQTTCLIENT_h

#include "arduino.h"
//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "Config.h"

class MqttClient{
    public:
    MqttClient();
   ~MqttClient();
    void setup();
    void loop(long ms);
    private:
    PubSubClient * client;
    //ESP8266WiFiClass * wifi;
    WiFiClient * wf;
};
#endif