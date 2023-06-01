#include "MQTTClientPLB.h"

MQTTClientPLB::MQTTClientPLB(int id) : _id(id)
{
    Serial.print("OBJECT CREATED: ");
    Serial.println(_id);
}

EspMQTTClient& MQTTClientPLB::getClient()
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
    _event = Event::noEvent;
    if(_isDirectorResponseFlag)
    {
      if (_directorState == DirectorState::INVALID)
      {
        _event = Event::EV_RFID_INVALID;
      }
      else if (_directorState == DirectorState::VALID)
      {
        _event = Event::EV_RFID_VALID;
      }
      else if (_directorState == DirectorState::TIMED_OUT)
      {
        _event = Event::EV_RFID_TIMED_OUT;
      }
      else if (_directorState == DirectorState::ALREADY_CHECKED_IN)
      {
       _event = Event::EV_RFID_ALREADY_CHECKED_IN;
      }
      _isDirectorResponseFlag = 0;
    }

    if(_isModeChangedFlag)
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
  _client.subscribe(mqtt_topic_receivePower, [this](const String & topic, const String & payload) {
    _isPowerReceivedFlag = true;
    _powerReceived = payload.toFloat();
  });
  _client.subscribe(mqtt_topic_directorResponse, [this](const String & topic, const String & payload) {
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
    }
  });
    _client.subscribe(mqtt_topic_mode, [this](const String & topic, const String & payload) {
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
    }
  });
}

void MQTTClientPLB::checkDirector(uint32_t id)
{
    send(mqtt_topic_directorId, String(id));
}

void MQTTClientPLB::supplyPowerToStation(int id)
{
    send(mqtt_topic_requestPower, String(id));
}

void MQTTClientPLB::stopSupplyToStation(int id)
{
    send(mqtt_topic_stopSupply, String(id));
}

float MQTTClientPLB::getPowerReceived()
{
    return _powerReceived;
}

Event MQTTClientPLB::getEvent()
{
  return _event;
}

// DirectorState MQTTClientPLB::waitingForPlb(int waitingTime)
// {
//     long lastTime = millis();
//     while(!_isDirectorResponseFlag)
//     {
//         if(millis() - lastTime >= waitingTime)
//         {
//           return DirectorState::TIMED_OUT;
//         }
//     }
//     _isDirectorResponseFlag = 0;
//     return _directorState;
// }