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
    _isDirectorDetectedFlag = true;
    _directorId = payload.toInt(); });
  // _client.subscribe(mqtt_topic_solarPower, [this](const String &topic, const String &payload)
  //                   { _solarPower = payload.toInt(); });
  _client.subscribe(mqtt_topic_requestSupply, [this](const String &topic, const String &payload)
                    {
    _isRequestSupplyFlag = true;
    _stationId = payload.toInt();
    });
}

int MQTTClientStation::getId() override
{
  return _stationId;
}

int MQTTClientStation::getDirectorId()
{
  return _directorId;
}

bool MQTTClientStation::isSupplyRequest()
{
  return _isRequestSupplyFlag;
}

bool MQTTClientStation::isDirectorChecked()
{
  return _isDirectorDetectedFlag;
}

void MQTTClientStation::clearSupplyRequest()
{
  _isRequestSupplyFlag = false;
}

void MQTTClientStation::clearDirectorCheck()
{
  _isDirectorDetectedFlag = false;
}

void MQTTClientStation::charge(float power)
{
  send(mqtt_topic_charge, (String)power);
}

void MQTTClientStation::switchMode(StationModes mode)
{
  send(mqtt_topic_mode, (String)mode);
}

int MQTTClientStation::loop()
{
  receive();
  if(isDirectorChecked())
  {
    switch (getDirectorId()) 
    {
        case 1:
        _event = EV_director1;
        break;
        case 2:
        _event = EV_director2;
        break;
        case 3:
        _event = EV_director3;
        break;
        case 4:
        _event = EV_director4;
        break;
    }
    clearDirectorCheck();
  }
}
