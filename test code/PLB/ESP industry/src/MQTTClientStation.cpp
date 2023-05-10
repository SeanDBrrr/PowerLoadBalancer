#include "MQTTClientStation.h"

const char *name = "Kiwy";
const char *password = "aquamagic23";
const char *mqtt_module = "Group4-PLB";
const char *broker_ip = "192.168.61.23";
const char *mqtt_topic_charge = "group4/charge";
short port = 1883;

EspMQTTClient client(name, password, broker_ip, mqtt_module, port);

MQTTClientStation::MQTTClientStation() : _client(client) {}

MQTTClientStation::~MQTTClientStation() {}

EspMQTTClient &MQTTClientStation::getClient()
{
  return _client;
}

void MQTTClientStation::send(String topic, String message)
{
  _client.publish(topic, message, 0);
}

void MQTTClientStation::receive()
{
  _client.loop();
}

void MQTTClientStation::onConnectionSubscribe()
{
  _client.subscribe(mqtt_topic_directorId, [this](const String &topic, const String &payload)
                    {
    _isDirectorDetected = true;
    _directorId = payload.toInt(); });
  _client.subscribe(mqtt_topic_solarPower, [this](const String &topic, const String &payload)
                    { _solarPower = payload.toInt(); });
  _client.subscribe(mqtt_topic_requestSupply, [this](const String &topic, const String &payload)
                    {
    _isSupplyRequest = true;
    _stationId = payload.toInt();
    });
}
