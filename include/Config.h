

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
extern const char * ntpServer;
extern const char * fw;


#define CHECKWIFI 300000

#define TIME_OFFSET 8

#define SENSOR1 27
#define SENSOR2 14
#define SENSOR3 12
#define SENSOR4 13


// CONNECTIONS:

#define BTN_PIN 32

#define SPEAKER_PIN 26

#define LED_PIN 25

#define DS1302_CLK 17 
#define DS1302_DAT 4
#define DS1302_RST 16

#define PIN_VOPEN 2
#define PIN_VCLOSE 15

#define MEM_ALARM 0
#define MEM_VALVE 1


#define timeZone  8

#define NTP_TIMEOUT 1500


#endif