// Httphelper.h

#ifndef _HTTPHELPER_h
#define _HTTPHELPER_h
#include "arduino.h"


#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "wp_system.h"
//#include "ESP8266WepSpiffsUpdater.h"



class HttpHelper
{
public:
	HttpHelper();
	~HttpHelper();
	//void clientHandle();
	void setup(WP_system *ws);

private:
	void handleNotFound(AsyncWebServerRequest * request);
	void handleFile(String path,String type, AsyncWebServerRequest *request);
	//boolean handleFileRead(String path);
	void handleRoot(AsyncWebServerRequest *request);
	void handleUpdate(AsyncWebServerRequest *request);
	void handleLog(AsyncWebServerRequest *request);
	void handleUpdateFS(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final);
	void handleUpdateOS(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final);
	AsyncWebServer * server;
	uint8_t counter;
	WP_system *ws_sys;
};

#endif


