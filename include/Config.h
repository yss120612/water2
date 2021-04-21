

#ifndef _CONFIG_h
#define _CONFIG_h
#include "arduino.h"


extern const char * webpass;
extern const char * webuser;
extern const char * ntp_server;
extern const char * mqtt_server; // Имя сервера MQTT
extern const int    mqtt_port; // Порт для подключения к серверу MQTT
extern const char * mqtt_user; // Логи от сервер
extern const char * mqtt_pass; // Пароль от сервера

extern const char * mqtt_str_valve;
extern const char * mqtt_str_switch;
extern const char * mqtt_str_disalarm;
extern const char * mqtt_str_ws1;
extern const char * mqtt_str_ws2;
extern const char * mqtt_str_ws3;
extern const char * mqtt_str_ws4;
extern const char * mqtt_str_log;

extern const char * fw;
extern const PROGMEM char *httpLogin;
extern const PROGMEM char *httpPass;


#define CHECKWIFI 300000

#define TIME_OFFSET 8

#define SENSOR1 15
#define SENSOR2 2
#define SENSOR3 4
#define SENSOR4 16


// CONNECTIONS:

#define BTN_PIN 32

#define SPEAKER_PIN 26

#define LED_PIN 25

#define DS1302_CLK 14
#define DS1302_DAT 12
#define DS1302_RST 13

#define PIN_VOPEN 5
#define PIN_VCLOSE 17

#define MEM_ALARM 0
#define MEM_VALVE 1


#define timeZone  8

#define NTP_TIMEOUT 1500


#endif