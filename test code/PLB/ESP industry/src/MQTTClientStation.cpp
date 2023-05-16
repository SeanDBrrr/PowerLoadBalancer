#include "MQTTClientStation.h"
#include <string>

const char *name = "Kiwy";
const char *password = "aquamagic23";
const char *mqtt_module = "Group4-PLB-Station";
const char *broker_ip = "192.168.61.23";
short port = 1883;

EspMQTTClient client(name, password, broker_ip, mqtt_module, port);

MQTTClientStation::MQTTClientStation(int Id) : _client(client)
{
  _stationId = Id;
  _setStationId();
  _client.setMqttClientName(mqtt_module);
}

MQTTClientStation::~MQTTClientStation() {}

EspMQTTClient &MQTTClientStation::getClient()
{
  return _client;
}

void MQTTClientStation::_setStationId()
{
  char topic_StationId[30];
  sprintf(topic_StationId, mqtt_topic_StationId);
  sprintf(topic_StationId, "%d", _stationId);
  // char topic_ChargeStation_[30];
  // char topic_Module_[30];
  // char topic_RequestSupply_[30];
  
  // mqtt_topic_charge_station += (String)_stationId;
  // mqtt_module += (String)_stationId;
  // mqtt_topic_requestSupply += (String)_stationId;
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
  _client.subscribe(mqtt_topic_requestSupply, [this](const String &topic, const String &payload)
                    {
    _isRequestSupplyFlag = true;
    _stationId = payload.toInt();
    });
}

int MQTTClientStation::getId() //override
{
  return _stationId;
}

void MQTTClientStation::charge(float power) //override
{
  send(mqtt_topic_charge_station, (String)power);
}

void MQTTClientStation::switchMode(StationModes mode) //override
{
  send(mqtt_topic_mode, (String)mode);
}

PLBEvents MQTTClientStation::loop()
{
  receive();
  if(_isDirectorDetectedFlag)
  {
    switch (_directorId) 
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
    _isDirectorDetectedFlag = 0;
  }

  if(_isRequestSupplyFlag)
  {
    switch (_stationId)
    {
      case 1:
        _event = EV_supply1;
        break;
      case 2:
        _event = EV_supply2;
        break;
      case 3:
        _event = EV_supply3;
        break;
      case 4:
        _event = EV_supply4;
        break;
    }
    _isRequestSupplyFlag = 0;
  }

  return _event;
}
