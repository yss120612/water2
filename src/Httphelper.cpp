#include "Httphelper.h"
//#include <LittleFS.h>
//#include <FS.h>
#include <ArduinoJson.h>
#include  "Log.h"
#include  "Config.h"
#include <Update.h>
#include <SPIFFS.h>

HttpHelper::HttpHelper()
{
	server = new AsyncWebServer(80);
	if (!SPIFFS.begin(true)){
		logg.logging("FS Error");
	}
}

HttpHelper::~HttpHelper()
{
	delete server;
	SPIFFS.end();
}

void HttpHelper::setup(WP_system *ws) {
	if (server == NULL) return;
	ws_sys=ws;
	server->onNotFound(std::bind(&HttpHelper::handleNotFound, this, std::placeholders::_1));

	server->on(
	"/update", 
	HTTP_POST, 
	[](AsyncWebServerRequest *request){
  	  request->redirect("/");
    }, 
	std::bind(&HttpHelper::handleUpdateOS, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6)
	);

server->on(
	"/spiffs", 
	HTTP_POST, 
	[](AsyncWebServerRequest *request){
	    request->send(200);
    }, 
	std::bind(&HttpHelper::handleUpdateFS, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6)
	);


	server->on("/", std::bind(&HttpHelper::handleRoot, this, std::placeholders::_1));
		
	server->on("/logdata", std::bind(&HttpHelper::handleLog, this,std::placeholders::_1));

	server->on("/upd", std::bind(&HttpHelper::handleUpdate, this, std::placeholders::_1) );

	server->serveStatic("/log", SPIFFS, "/log.htm", NULL);

	server->serveStatic("/css/bootstrap.min.css", SPIFFS, "/css/bootstrap.min.css", NULL);

	server->serveStatic("/js/bootstrap.min.js", SPIFFS, "/js/bootstrap.min.js", NULL);

	server->serveStatic("/js/jquery.min.js", SPIFFS, "/js/jquery.min.js", NULL);

	server->serveStatic("/js/export-data.js", SPIFFS, "/js/export-data.js", NULL);

	server->serveStatic("/js/exporting.js", SPIFFS, "/js/exporting.js", NULL);

	server->serveStatic("/js/highstock.js", SPIFFS, "/js/highstock.js", NULL);
			
	server->begin();

	//httpSpiffsUpdater = new ESP8266WebSpiffsUpdater();
	//httpSpiffsUpdater->setup(server);
}

void HttpHelper::handleRoot(AsyncWebServerRequest * request) {
	
	  if (!request->authenticate("Yss1", "bqt3"))
		return request->requestAuthentication();
		handleFile("/index.htm","text/html", request);
	
}

void HttpHelper::handleLog(AsyncWebServerRequest * request)
{
	String str = "{\"logdata\":\"<ul>" + logg.getAll2Web() + "</ul>\"}";
	request->send(200, "text/json",str); // Oтправляем ответ No Reset
}

void HttpHelper::handleUpdate(AsyncWebServerRequest * request) {
	  if (!request->authenticate("Yss1", "bqt3"))
		return request->requestAuthentication();
	String resp = F("<!DOCTYPE html>\n<html>\n<head>\n");
	resp += F("<meta charset = \"utf-8\">\n");
	resp += F("<title>YssSM прошивка</title>\n");
	resp += F("<meta name = \"description\" content = \"Версия 0.1\">\n");
	resp += F("<meta name = \"author\" content = \"Yss\">\n");
	resp += F("<link href = \"/css/bootstrap.min.css\" type = \"text/css\" rel = \"stylesheet\">\n");
	resp += F("<script type = \"text/javascript\" src = \"/js/jquery.min.js\"></script>\n");
	resp += F("<script type = \"text/javascript\" src = \"/js/bootstrap.min.js\"></script>\n");
	resp += F("</head>\n<body>\n");
	resp += F("<div class = \"col-md-12\">\n");
	resp += F("<a href = \"/\"class = \"btn btn-info\">Дом</a>\n");
	resp += F("</div>\n");
	resp += F("<div class = \"alert alert-info\" role = \"alert\">");
	resp += F("<h3>Прошивка</h3>\n");
	resp += F("<form method = \"POST\" action = \"/update\" enctype = \"multipart/form-data\" class=\"form-inline\">\n");
	resp += F("<div class = \"btn-group\">\n");
	resp += F("<input type = \"file\" class = \"btn\" name = \"update\">\n");
	resp += F("<input type = \"submit\" class = \"btn\" value = \"Прошить\" onclick = \"this.value = 'Подождите...';\">\n");
	resp += F("</div>\n");
	resp += F("</form>\n");
	resp += F("</div>\n");
	resp += F("<div class = \"alert alert-success\" role = \"alert\">");
	resp += F("<h3>WEB сервер</h3>\n");
	resp += F("<form method = \"POST\" action = \"/spiffs\" enctype = \"multipart/form-data\" class=\"form-inline\">");
	resp += F("<div class = \"btn-group\">\n");
	resp += F("<input type = \"file\" class = \"btn\" name = \"spiffs\">\n");
	resp += F("<input type = \"submit\" class = \"btn\" value = \"Прошить\" onclick = \"this.value = 'Подождите...';\">\n");
	resp += F("</div>\n");
	resp += F("</form>\n");
	resp += F("</div>\n");
	resp += F("</body>\n</html>\n");

	request->send(200, "text/html", resp);
}

void HttpHelper::handleFile(String path,String type, AsyncWebServerRequest *request){
	//data->ir_sleep(); 
	//Serial.println(path);
	request->send(SPIFFS,path,type);
}

void HttpHelper::handleNotFound(AsyncWebServerRequest * request) {
	request->send(200, "text/plain", "NOT FOUND!!!");
}

// boolean HttpHelper::handleFileRead(String path) {

// 	if (path.endsWith("/")) path += "index.htm";
// 	String contentType;
// 	if (path.endsWith(".htm") || path.endsWith(".html")) contentType = "text/html";
// 	else if (path.endsWith(".css")) contentType = "text/css";
// 	else if (path.endsWith(".js")) contentType = "application/javascript";
// 	else if (path.endsWith(".png")) contentType = "image/png";
// 	else if (path.endsWith(".gif")) contentType = "image/gif";
// 	else if (path.endsWith(".jpg")) contentType = "image/jpeg";
// 	else if (path.endsWith(".ico")) contentType = "image/x-icon";
// 	else if (path.endsWith(".xml")) contentType = "text/xml";
// 	else if (path.endsWith(".pdf")) contentType = "application/x-pdf";
// 	else if (path.endsWith(".zip")) contentType = "application/x-zip";
// 	else if (path.endsWith(".gz")) contentType = "application/x-gzip";
// 	else if (path.endsWith(".json")) contentType = "application/json";
// 	else contentType = "text/plain";

// 	//split filepath and extension

// 	String prefix = path, ext = "";
// 	int lastPeriod = path.lastIndexOf('.');
// 	if (lastPeriod >= 0) {
// 		prefix = path.substring(0, lastPeriod);
// 		ext = path.substring(lastPeriod);
// 	}

// 	//look for smaller versions of file
// 	//minified file, good (myscript.min.js)
	 
// 	if (SPIFFS.exists(prefix + ".min" + ext)) path = prefix + ".min" + ext;
// 	//gzipped file, better (myscript.js.gz)
// 	if (SPIFFS.exists(prefix + ext + ".gz")) path = prefix + ext + ".gz";
// 	//min and gzipped file, best (myscript.min.js.gz)
// 	if (SPIFFS.exists(prefix + ".min" + ext + ".gz")) path = prefix + ".min" + ext + ".gz";

// 	if (SPIFFS.exists(path)) {

// 		File file = SPIFFS.open(path, "r");
// 		//if (server->hasArg())
// 		if (server->hasArg("download"))
// 			server->sendHeader("Content-Disposition", " attachment;");
// 		if (server->uri().indexOf("nocache") < 0)
// 			server->sendHeader("Cache-Control", " max-age=172800");

// 		//optional alt arg (encoded url), server sends redirect to file on the web
// 		//if (WiFi.status() == WL_CONNECTED && ESP8266WebServer::hasArg("alt")) {
// 		//	ESP8266WebServer::sendHeader("Location", ESP8266WebServer::arg("alt"), true);
// 		//	ESP8266WebServer::send(302, "text/plain", "");
// 		//}
// 		//else {
// 		//	//server sends file
// 		//	size_t sent = ESP8266WebServer::streamFile(file, contentType);
// 		//}
// 		size_t sent = server->streamFile(file, contentType);
// 		file.close();
// 		return true;
// 	} //if SPIFFS.exists

// 	//server->send(200, "text/plain", SPIFFS.);
// 	return false;
// } //bool handleFileRead

void HttpHelper::handleUpdateOS(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final){
 uint32_t free_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
  if (!index){
	  
	//this->data->ir_sleep(); 
	request->redirect("/");
	counter=0;
    //Serial.println("Update");
    if (!Update.begin(free_space,U_FLASH)) {
      	logg.logging("Update Error");
		logg.logging(Update.errorString());
    }
  }

  if (Update.write(data, len) != len) {
	logg.logging("Update Error");
	logg.logging(Update.errorString());
  }else{
	  if (counter++==9) {
		  counter=0;
		  }
  }

  if (final) {
    if (!Update.end(true)){
	  	logg.logging("Update Error");
	  	logg.logging(Update.errorString());
    } else {
		ESP.restart();
    }
  }
}

void HttpHelper::handleUpdateFS(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final){
 
  if (!index){
	//this->data->ir_sleep(); 
	
	request->redirect("/");
	counter=0;
    logg.logging("Update SPIFFS");
    if (!Update.begin(UPDATE_SIZE_UNKNOWN,U_SPIFFS,LED_PIN)) {
      logg.logging(Update.errorString());
    }
  }

  if (Update.write(data, len) != len) {
    logg.logging(Update.errorString());
	
  }else{
	//  if (counter++==9) {Serial.print(".");counter=0;}
  }

  if (final) {
    if (!Update.end(true)){
      logg.logging(Update.errorString());
    } else {
      logg.logging("Update SPIFF complete. Rebooting...");
	  ESP.restart();
    }
  }
}