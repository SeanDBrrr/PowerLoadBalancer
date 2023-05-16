#include "MQTTClientStation.h"
#include <string>

MQTTClientStation::MQTTClientStation(int Id)
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
  String mqtt_topic_StationId_str = String(mqtt_topic_StationId);
  mqtt_topic_StationId_str += String(_stationId);
  mqtt_topic_StationId = mqtt_topic_StationId_str.c_str();

  String mqtt_topic_charge_station_str = String(mqtt_topic_charge_station);
  mqtt_topic_charge_station_str += String(_stationId);
  mqtt_topic_charge_station = mqtt_topic_charge_station_str.c_str();

  String mqtt_module_str = String(mqtt_module);
  mqtt_module_str += String(_stationId);
  mqtt_module = mqtt_module_str.c_str();

  String mqtt_topic_requestSupply_str = String(mqtt_topic_requestSupply);
  mqtt_topic_requestSupply_str += String(_stationId);
  mqtt_topic_requestSupply = mqtt_topic_requestSupply_str.c_str();
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
