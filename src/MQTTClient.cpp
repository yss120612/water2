#include "MQTTclient.h"



 MqttClient::MqttClient(){

 }

 MqttClient::~MqttClient(){
     delete(client);
 }
    void  MqttClient::setup(){
        wf = new WiFiClient();
        
//PubSubClient client(espClient);
        client = new PubSubClient(mqtt_server, mqtt_port,*wf);
    };



    void MqttClient::loop(long ms){

    };