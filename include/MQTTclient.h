#ifndef _MQTTCLIENT_h
#define _MQTTCLIENT_h

#include "arduino.h"
//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "Config.h"
#include "wp_system.h"

class MqttClient{
    public:
    MqttClient();
   ~MqttClient();
    void setup(WP_system *ws);
    void loop(long ms);
    void setValve(bool state);
    void alarm();

    private:
    PubSubClient * client;
    void reconnect();
    void callback(char* topic, byte* payload, unsigned int length);
    WiFiClient * wf;
    WP_system * ws_sys;
    const int check_time=1000; //every 3 sec
    unsigned long last_check;
    bool ignore_next_valve;

};
#endif