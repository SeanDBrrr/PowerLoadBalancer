#include "MQTTServer.h"

const char *name = "Kiwy";
const char *password = "aquamagic23";
const char *mqtt_module = "Group4-Charger1";
const char *broker_ip = "192.168.61.23";
const char *mqtt_topic_request = "group4/request";
short port = 1883;

EspMQTTClient client(name, password, broker_ip, mqtt_module, port);

MQTTServer::MQTTServer() : _client(client)
{
    
}

EspMQTTClient& MQTTServer::getClient()
{
    return _client;
}

void MQTTServer::send(String topic, String message)
{
    _client.publish(topic, message, 0);
}

void MQTTServer::receive()
{
    _client.loop();
}

void MQTTServer::onConnectionEstablished()
{
//   _client.subscribe(mqtt_topic_charge, [this](const String & topic, const String & payload) {
//     _id = payload.toInt();
//   });
//   _client.subscribe(mqtt_topic_powerProduced, [this](const String & topic, const String & payload) {
//     _powerProduced = payload.toInt();
//   });
//   _client.subscribe(mqtt_topic_buildingState, [this](const String & topic, const String & payload) {
//     _buildingState = (payload == "1");
//   });
}

void MQTTServer::checkDirector(int id)
{

}

int MQTTServer::supplyPower()
{
    send(mqtt_topic_request, "Start");
    return 0;
}

int MQTTServer::stopSupply()
{
    send(mqtt_topic_request, "Stop");
    return 0;
}



