#include "MQTTClientPLB.h"

MQTTClientPLB::MQTTClientPLB( // Few objects are not initialized yet.
    int id)
    : _id(id),
      _powerReceived(0),
      _isPowerReceivedFlag(0),
      _isDirectorResponseFlag(0),
      _isModeChangedFlag(0),
      _wifiConnectedFlag(0),
      _mqttConnectedFlag(0),
      _directorState(DirectorState::noState),
      _previousTime(0),
      _wifiConnected(false),
      _mqttConnected(false),
      _wifiTrials(0),
      _mqttTrials(0)
{
  Serial.print("OBJECT CREATED: ");
  Serial.println(_id);
  _client.enableDebuggingMessages();
}

EspMQTTClient &MQTTClientPLB::getClient()
{
  return _client;
}

void MQTTClientPLB::send(String topic, String message)
{
  _client.publish(topic, message, 0);
}

void MQTTClientPLB::receive()
{
  _client.loop();
  static unsigned long lastTime = 0;
  if (millis() - lastTime >= 5000)
  {
    send(mqtt_topic_heartbeat, static_cast<String>(_id));
    lastTime = millis();
  }

  _event = Event::noEvent;
  _event = getConnectionStatusEvent();
  // checkConnection();

  if (_directorState == DirectorState::TIMED_OUT)
  {
    Serial.println("SHOULD HAVE TIME FDOUT");
    _event = Event::EV_RFID_TIMED_OUT;
    _directorState = DirectorState::noState;
  }
  if (_isDirectorResponseFlag)
  {
    if (_directorState == DirectorState::INVALID)
    {
      _event = Event::EV_RFID_INVALID;
    }
    else if (_directorState == DirectorState::VALID)
    {
      _event = Event::EV_RFID_VALID;
    }
    else if (_directorState == DirectorState::ALREADY_CHECKED_IN)
    {
      _event = Event::EV_RFID_ALREADY_CHECKED_IN;
    }
    _isDirectorResponseFlag = 0;
    // rESET DIRECTORSTATE?
  }

  if (_isModeChangedFlag)
  {
    if (_stationMode == StationMode::Director)
    {
      _event = Event::EV_MODE_CHANGED_DIRECTOR;
    }
    else if (_stationMode == StationMode::FCFS)
    {
      _event = Event::EV_MODE_CHANGED_FCFS;
    }
    else if (_stationMode == StationMode::Dynamic)
    {
      _event = Event::EV_MODE_CHANGED_DYNAMIC;
    }
    _isModeChangedFlag = 0;
  }
}

void MQTTClientPLB::onConnectionSubscribe()
{
  _client.subscribe(mqtt_topic_receivePower, [this](const String &topic, const String &payload)
                    {
    _isPowerReceivedFlag = true;
    _powerReceived = payload.toFloat(); });
  _client.subscribe(mqtt_topic_heartbeat, [this](const String &topic, const String &payload) {});
  _client.subscribe(mqtt_topic_directorResponse, [this](const String &topic, const String &payload)
                    {
    _isDirectorResponseFlag = true;
    if (payload == "VALID")
    {
      _directorState = DirectorState::VALID;
    }
    else if (payload == "INVALID")
    {
      _directorState = DirectorState::INVALID;
    }
    else if (payload == "ALREADY CHECKED IN")
    {
      _directorState = DirectorState::ALREADY_CHECKED_IN;
    } });
  _client.subscribe(mqtt_topic_mode, [this](const String &topic, const String &payload)
                    {
    _isModeChangedFlag = true;
    if (payload == "Director Mode")
    {
      _stationMode = StationMode::Director;
    }
    else if (payload == "FCFS Mode")
    {
      _stationMode = StationMode::FCFS;
    }
    else if (payload == "Dynamic Mode")
    {
      _stationMode = StationMode::Dynamic;
    } });
}

void MQTTClientPLB::checkDirector(uint32_t id)
{
  send(mqtt_topic_directorId, static_cast<String>(id));
}

void MQTTClientPLB::supplyPowerToStation(int id)
{
  send(mqtt_topic_requestPower, static_cast<String>(id));
}

void MQTTClientPLB::stopSupplyToStation(int id)
{
  send(mqtt_topic_stopSupply, static_cast<String>(id));
}

Event MQTTClientPLB::getEvent()
{
  return _event;
}

float MQTTClientPLB::getPowerReceived()
{
  return _powerReceived;
}

void MQTTClientPLB::directorTimeout(int waitingTime)
{
  unsigned long lastTime = millis();
  while (!_isDirectorResponseFlag)
  {
    if (millis() - lastTime >= waitingTime)
    {
      Serial.println("TIMED_OUT");
      _isDirectorResponseFlag = 0; // unsure, was at the end of while loop
      _directorState = DirectorState::TIMED_OUT;
      break;
    }
  }
}

Event MQTTClientPLB::getConnectionStatusEvent()
{
  Event ev = Event::noEvent;

  if (millis() - _previousTime >= _INTERVAL)
  {
    _previousTime = millis();

    if (!_client.isWifiConnected() && _wifiTrials < _TRIALS)
    {
      _wifiTrials++;
      ev = Event::EV_WIFI_TRIALS;
      _wifiConnected = false;
    }
    else if (!_client.isWifiConnected() && _wifiTrials == _TRIALS)
    {
      _wifiTrials++;
      ev = Event::EV_WIFI_NOT_CONNECTED;
    }
    if (!_client.isMqttConnected() && _wifiConnected && _mqttTrials < _TRIALS)
    {
      _mqttTrials++;
      _mqttConnected = false;
      ev = Event::EV_MQTT_TRIALS;
    }
    else if (!_client.isMqttConnected() && _wifiConnected && _mqttTrials == _TRIALS)
    {
      _mqttTrials++;
      ev = Event::EV_MQTT_NOT_CONNECTED;
    }
  }

  if (_client.isWifiConnected() && _wifiTrials > 0 && _wifiTrials < _TRIALS)
  {
    _wifiTrials = 0;
    _mqttTrials = 0;
    ev = Event::EV_WIFI_CONNECTED;
    _wifiConnected = true;
  }

  if (_client.isMqttConnected() && _wifiConnected && _mqttTrials > 0 && _mqttTrials < _TRIALS)
  {
    _mqttTrials = 0;
    _mqttConnected = true;
    ev = Event::EV_MQTT_CONNECTED;
  }

  return ev;
}

// void MQTTClientPLB::checkConnection()
// {
//   if (!_client.isWifiConnected() && !_wifiConnectedFlag)
//   {
//     _event = Event::EV_DISCONNECTED_WIFI;
//     _wifiConnectedFlag = true;
//   }
//   else if (_client.isWifiConnected() && _wifiConnectedFlag)
//   {
//     _event = Event::noEvent;
//     _wifiConnectedFlag = false;
//   }

//   if (!_client.isMqttConnected() && !_mqttConnectedFlag)
//   {
//     _event = Event::EV_DISCONNECTED_MQTT;
//     _mqttConnectedFlag = true;
//   }
//   else if (_client.isMqttConnected() && _mqttConnectedFlag)
//   {
//     _event = Event::noEvent;
//     _mqttConnectedFlag = false;
//   }
//}