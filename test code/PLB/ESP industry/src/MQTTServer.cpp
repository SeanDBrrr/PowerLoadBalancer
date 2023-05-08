#include "MQTTServer.h"

const char *name = "Kiwy";
const char *password = "aquamagic23";
const char *mqtt_module = "Group4-PLB";
const char *broker_ip = "192.168.61.23";
const char *mqtt_topic_charge = "group4/charge";
short port = 1883;

const char *mqtt_topic_mode = "group4/mode";
const char *mqtt_topic_id = "group4/id";
const char *mqtt_topic_request = "group4/requestPower";
const char *mqtt_topic_powerProduced = "group4/powerProduced";
const char *mqtt_topic_buildingState = "group4/buildingState";

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
  _client.subscribe(mqtt_topic_id, [this](const String & topic, const String & payload) {
    _id = payload.toInt();
  });
  _client.subscribe(mqtt_topic_powerProduced, [this](const String & topic, const String & payload) {
    _powerProduced = payload.toInt();
  });
  _client.subscribe(mqtt_topic_buildingState, [this](const String & topic, const String & payload) {
    _buildingState = (payload == "1");
  });
    _client.subscribe(mqtt_topic_request, [this](const String & topic, const String & payload) {
    _receivedRequest = true;
    if(payload == "charger1")
    {
      _stationId = 1;
    }
    else if(payload == "charger2")
    {
      _stationId = 2;
    }
    else if(payload == "charger3")
    {
      _stationId = 3;
    }
        if(payload == "charger4")
    {
      _stationId = 4;
    }
  });
}

int MQTTServer::getId()
{
    return _id;
}

int MQTTServer::getStationId()
{
    return _stationId;
}

bool MQTTServer::getSupplyRequest()
{
    return _receivedRequest;
}

void MQTTServer::clearSupplyRequest()
{
    _receivedRequest = false;
}

void MQTTServer::charge(int power)
{
    send(mqtt_topic_charge, (String)power);
}

void MQTTServer::switchMode(StationModes mode)
{
    send(mqtt_topic_mode, (String)mode);
}

int MQTTServer::getPowerProduced()
{
    return _powerProduced;
}

bool MQTTServer::getState()
{
    return _buildingState;
}



