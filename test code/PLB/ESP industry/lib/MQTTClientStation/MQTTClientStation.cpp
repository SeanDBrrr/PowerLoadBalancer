#include "MQTTClientStation.h"
#include <string>

std::queue<int> MQTTClientStation::supplyRequestEvents;
std::queue<int> MQTTClientStation::stopSupplyEvents;
std::queue<int> MQTTClientStation::directorEvents;
std::queue<int> MQTTClientStation::connectionEvents;
std::vector<PLBEvents> MQTTClientStation::events;

MQTTClientStation::MQTTClientStation(int id) : 
_stationId(id), 
_directorId(0)
{
  _setStationTopics();
  _client.setMqttClientName(mqtt_module.c_str());
}

MQTTClientStation::~MQTTClientStation() {}

EspMQTTClient &MQTTClientStation::getClient()
{
  return _client;
}

void MQTTClientStation::_setStationTopics()
{
  mqtt_topic_StationId += static_cast<String>(_stationId);
  mqtt_topic_charge_station += static_cast<String>(_stationId);
  mqtt_module += static_cast<String>(_stationId);
  mqtt_topic_requestSupply += static_cast<String>(_stationId);
  mqtt_topic_stopSupply += static_cast<String>(_stationId);
  mqtt_topic_directorId += static_cast<String>(_stationId);
  mqtt_topic_directorValidate += static_cast<String>(_stationId);
  mqtt_topic_stationHeartbeat += static_cast<String>(_stationId);
}

void MQTTClientStation::send(String topic, String message)
{
  _client.publish(topic, message, 0);
}

void MQTTClientStation::receive()
{
  _client.loop();
  checkConnection(7500);
}

void MQTTClientStation::checkConnection(int disconnectionTimeout)
{
  if(millis() - _lastHeartbeat >= disconnectionTimeout)
  {
    _stationConnected = false;
  }
  else
  {
    _stationConnected = true;
  }

  if(_stationConnectedLast != _stationConnected)
  {
    if(_stationConnected)
    {
      events.emplace_back(PLBEvents::EV_Connected);
    }
    else
    {
      events.emplace_back(PLBEvents::EV_Disconnected);
    }
    connectionEvents.push(_stationId);
    Serial.println(_stationId);
    _stationConnectedLast = _stationConnected;
  }
}

/*
 * Each station is subscribed to its own topics (in relation to their ID), 
 * but every event is registered in global containers (one per event) 
*/
void MQTTClientStation::onConnectionSubscribe()
{
  _client.subscribe(mqtt_topic_directorId, [this](const String &topic, const String &payload)
  {
    /* A director tapped its RFID */
    events.emplace_back(PLBEvents::EV_Director);
    directorEvents.push(_stationId);
    /* -> We store the director's ID */
    _directorId = payload.toInt();
  });
  _client.subscribe(mqtt_topic_requestSupply, [this](const String &topic, const String &payload)
  {
    /* _stationId requested power */
    events.emplace_back(PLBEvents::EV_Supply);
    /* -> We add its ID to the supplyRequest queue */
    supplyRequestEvents.push(_stationId);
    Serial.print("MQTTClientStation: EV_Supply -> ID = "); Serial.println(supplyRequestEvents.front());
  });
  _client.subscribe(mqtt_topic_stopSupply, [this](const String &topic, const String &payload)
  {
    /* _stationId requested a stop */
    events.emplace_back(PLBEvents::EV_Stop);
    /* -> We add its ID to the stopSupply queue */
    stopSupplyEvents.push(_stationId);
    Serial.print("MQTTClientStation: EV_Supply -> ID = "); Serial.println(stopSupplyEvents.front());
    /* Remove the previous director's ID if there is one */
    if (_directorId) _directorId = 0;
  });
  _client.subscribe(mqtt_topic_stationHeartbeat, [this](const String &topic, const String &payload)
  {
    _lastHeartbeat = millis();
  });
}

int MQTTClientStation::getId()
{
  return _stationId;
}

uint32_t MQTTClientStation::getDirectorId()
{
  return _directorId;
}

void MQTTClientStation::validateDirector(DirectorState directorState)
{
  if(directorState == DirectorState::VALID)
  {
    send(mqtt_topic_directorValidate, "VALID");
  }
  else if(directorState == DirectorState::INVALID)
  {
    send(mqtt_topic_directorValidate, "INVALID");
  }
  else if(directorState == DirectorState::ALREADY_CHECKED_IN)
  {
    send(mqtt_topic_directorValidate, "ALREADY CHECKED IN");
  }
}

void MQTTClientStation::charge(float power)
{
  Serial.print("Charge Station with "); Serial.println(power);
  send(mqtt_topic_charge_station, String(power));
}

void MQTTClientStation::switchMode(StationModes mode)
{
  if(mode == StationModes::MO_Director)
  {
    send(mqtt_topic_mode, "Director Mode");
  }
  else if(mode == StationModes::MO_FCFS)
  {
    send(mqtt_topic_mode, "FCFS Mode");
  }
  else if(mode == StationModes::MO_Dynamic)
  {
    send(mqtt_topic_mode, "Dynamic Mode");
  }
}

void MQTTClientStation::notifyDashboard(String message)
{
  send(mqtt_topic_notifyDashboard, message);
}

/* ----------- Events Getters */
std::vector<PLBEvents>& MQTTClientStation::getEvents()
{
  return events;
}

std::queue<int>& MQTTClientStation::getSupplyEvents()
{
  return supplyRequestEvents;
}

std::queue<int>& MQTTClientStation::getStopEvents()
{
  return stopSupplyEvents;
}

std::queue<int>& MQTTClientStation::getDirectorEvents()
{
  return directorEvents;
}
