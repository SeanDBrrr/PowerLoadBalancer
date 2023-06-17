#include "MQTTClientStation.h"
#include <string>

/* ----------------- Static variables */
std::queue<int> MQTTClientStation::idEvents;
std::vector<PLBEvents> MQTTClientStation::events;

/* ----------------- Constructor and Destructor */
MQTTClientStation::MQTTClientStation(int id) : 
_stationId(id), 
_directorId(0),
_stationMode(StationModes::MO_Dynamic),
_mode(PLBModes::MO_Auto),
_timer(0),
_slope(30000/11)
{
  _setStationTopics();
  _client.setMqttClientName(mqtt_module.c_str());
  _client.enableDebuggingMessages();
}

MQTTClientStation::~MQTTClientStation() {}

/* ----------------- Private functions */
void MQTTClientStation::_setStationTopics()
{
  mqtt_topic_StationId += static_cast<String>(_stationId);
  mqtt_topic_charge_station += static_cast<String>(_stationId);
  mqtt_module += static_cast<String>(_stationId);
  mqtt_topic_requestSupply += static_cast<String>(_stationId);
  mqtt_topic_stopSupply += static_cast<String>(_stationId);
  mqtt_topic_directorId += static_cast<String>(_stationId);
  mqtt_topic_directorValidate += static_cast<String>(_stationId);
}

void MQTTClientStation::_setTimer(float power)
{
  /* 30 sec to fully charge when power is max: slope (ms/kw) = 30000/11 */
  _timer = power * _slope;
}

/* ----------------- MQTT related functions */
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
  //if (_timer) _timer -= millis();
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
    idEvents.push(_stationId);
    /* -> We store the director's ID */
    _directorId = payload.toFloat();
  });
  _client.subscribe(mqtt_topic_requestSupply, [this](const String &topic, const String &payload)
  {
    /* _stationId requested power */
    events.emplace_back(PLBEvents::EV_Supply);
    /* -> We add its ID to the supplyRequest queue */
    idEvents.push(_stationId);
  });
  _client.subscribe(mqtt_topic_stopSupply, [this](const String &topic, const String &payload)
  {
    Serial.print("PLBEvents::EV_Stop: "); Serial.println(_stationId);
    /* _stationId requested a stop */
    events.emplace_back(PLBEvents::EV_Stop);
    /* -> We add its ID to the stopSupply queue */
    idEvents.push(_stationId);
    /* Remove the previous director's ID if there is one */
    if (_directorId) _directorId = 0;
  });
  _client.subscribe(mqtt_topic_stationHeartbeat, [this](const String &topic, const String &payload)
  {
    String online_payload = "ONLINE"+static_cast<String>(_stationId);
    String offline_payload = "OFFLINE"+static_cast<String>(_stationId);

    if (payload == online_payload)
    { 
      events.emplace_back(PLBEvents::EV_Connected);
      idEvents.push(_stationId);
      charge(0);
    }
    else if (payload == offline_payload) 
    { 
      events.emplace_back(PLBEvents::EV_Disconnected);
      idEvents.push(_stationId);
      charge(0);
    }
  });
  _client.subscribe(mqtt_topic_stationMode, [this](const String &topic, const String &payload)
  {
    if (payload == "DynamicMantainer") {
      Serial.println("Dynamic Received");
      _stationMode = StationModes::MO_Dynamic;
      events.emplace_back(PLBEvents::EV_SwitchStationMode);
    }
    else if (payload == "DirectorMantainer") {
      Serial.println("Director Received");
      _stationMode = StationModes::MO_Director;
      events.emplace_back(PLBEvents::EV_SwitchStationMode);
    }
    else if (payload == "FCFSMantainer") {
      Serial.println("FCFS Received");
      _stationMode = StationModes::MO_FCFS;
      events.emplace_back(PLBEvents::EV_SwitchStationMode);
    }
  });
}

/* ----------------- Interface's functions */
int MQTTClientStation::getId()
{
  return _stationId;
}

float MQTTClientStation::getDirectorId()
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
  send(mqtt_topic_charge_station, String(power));
  //if (_timer) _setTimer(power);
}

void MQTTClientStation::switchMode(StationModes mode)
{
  _stationMode = mode;
  if(mode == StationModes::MO_Director)
  {
    send(mqtt_topic_stationMode, "Director");
  }
  else if(mode == StationModes::MO_FCFS)
  {
    send(mqtt_topic_stationMode, "FCFS");
  }
  else if(mode == StationModes::MO_Dynamic)
  {
    send(mqtt_topic_stationMode, "Dynamic");
  }
}

void MQTTClientStation::notifyDashboard(String message)
{
  send(mqtt_topic_notifyDashboard, message);
}

StationModes MQTTClientStation::getStationMode()
{
  return _stationMode;
}

/* ----------------- Events Getters */
std::vector<PLBEvents>& MQTTClientStation::getEvents()
{
  return events;
}

std::queue<int>& MQTTClientStation::getIdEvents()
{
  return idEvents;
}
