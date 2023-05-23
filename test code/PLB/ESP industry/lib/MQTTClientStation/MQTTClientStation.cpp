#include "MQTTClientStation.h"
#include <string>

MQTTClientStation::MQTTClientStation(int Id)
{
  _stationId = Id;
  Serial.print("NEW OBJECT CREATED:");
  Serial.println(Id);
  _setStationId();
  _client.setMqttClientName(mqtt_module.c_str());
}

MQTTClientStation::~MQTTClientStation() {}

EspMQTTClient &MQTTClientStation::getClient()
{
  return _client;
}

void MQTTClientStation::_setStationId()
{
  mqtt_topic_StationId += String(_stationId);
  mqtt_topic_charge_station += String(_stationId);
  mqtt_module += String(_stationId);
  mqtt_topic_requestSupply += String(_stationId);
  mqtt_topic_stopSupply += String(_stationId);
}

void MQTTClientStation::send(String topic, String message)
{
  _client.publish(topic, message, 0);
}

PLBEvents MQTTClientStation::receive()
{
  _client.loop();
  _event = noEvent;
  if(_isDirectorDetectedFlag)
  {
    switch (_directorId) 
    {
      case 1:
        _event = EV_Director1;
        break;
      case 2:
        _event = EV_Director2;
        break;
      case 3:
        _event = EV_Director3;
        break;
      case 4:
        _event = EV_Director4;
        break;
    }
    _isDirectorDetectedFlag = 0;
  }

  if(_isRequestSupplyFlag)
  {
    switch (_stationId)
    {
      case 1:
        _event = EV_Supply1;
        break;
      case 2:
        _event = EV_Supply2;
        break;
      case 3:
        _event = EV_Supply3;
        break;
      case 4:
        _event = EV_Supply4;
        break;
    }
    _isRequestSupplyFlag = 0;
  }

  if(_isStopSupplyFlag)
  {
    switch (_stationId)
    {
      case 1:
        _event = EV_Stop1;
        break;
      case 2:
        _event = EV_Stop2;
        break;
      case 3:
        _event = EV_Stop3;
        break;
      case 4:
        _event = EV_Stop4;
        break;
    }
    _isStopSupplyFlag = 0;
  }

  return _event;
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
  _client.subscribe(mqtt_topic_stopSupply, [this](const String &topic, const String &payload)
                  {
    charge(0);
  });
}

int MQTTClientStation::getId() //override
{
  return _stationId;
}

int MQTTClientStation::getDirectorId() //override
{
  return _directorId;
}

void MQTTClientStation::charge(float power) //override
{
  send(mqtt_topic_charge_station, (String)power);
}

void MQTTClientStation::switchMode(StationModes mode) //override
{
  send(mqtt_topic_mode, (String)mode);
}
