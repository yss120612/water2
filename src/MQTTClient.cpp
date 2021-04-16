#include "MQTTclient.h"
#include "Log.h"


 MqttClient::MqttClient(){

 }

 MqttClient::~MqttClient(){
     delete(client);
     delete (wf);
 }



    void  MqttClient::setup(WP_system *ws){
        wf = new WiFiClient();
        ws_sys=ws;
        client = new PubSubClient(mqtt_server, mqtt_port,std::bind(&MqttClient::callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),*wf);
    };

    void MqttClient::callback(char* topic, byte* payload, unsigned int length){
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i=0;i<length;i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    }

void MqttClient::reconnect() {
  // Loop until we're reconnected
  uint8_t err=0;
  while (!client->connected()) {
     logg.logging("Attempting MQTT connection...");
    // Attempt to connect
    if (client->connect("ESP32Client-233",mqtt_user,mqtt_pass)) {
      logg.logging("connected");
      // Once connected, publish an announcement...
      client->publish("/user/yss1/161/outTopic","hello world");
      // ... and resubscribe
      client->subscribe("/user/yss1/161/valve");
    } else {
      logg.logging("failed, rc="+client->state()+String(" try again in 5 seconds"));
      err++;
      delay(5000);
    }
    if(err>2) break;
  }
}


    void MqttClient::loop(long ms){
        if(ms-last_check<check_time) return;
        last_check=ms;
        if (!client->connected()) reconnect();

    };