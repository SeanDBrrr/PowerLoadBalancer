#include "MQTTClientBuilding.h"

/* ----------------- Constructor and Destructor */
MQTTClientBuilding::MQTTClientBuilding() :
_event(PLBEvents::EV_NoEvent),
_PLBMode(PLBModes::MO_Auto),
_isSolarPowerArrivedFlag(false), 
_totalSolarPower(0),
_solarPowerTimeout(5000),
_isConnected(true),
_lastConnectionState(true)
{
  _client.enableDebuggingMessages();
  _client.enableLastWillMessage(mqtt_topic_buildingHeartbeat.c_str(), "PLB");
}

MQTTClientBuilding::~MQTTClientBuilding() {}

PLBEvents& MQTTClientBuilding::getEvent()
{
  return _event;
}

PLBModes MQTTClientBuilding::getPLBMode()
{
  return _PLBMode;
}

/* ----------------- MQTT related functions */
EspMQTTClient &MQTTClientBuilding::getClient()
{
  return _client;
}

void MQTTClientBuilding::send(String topic, String message)
{
  _client.publish(topic, message, 0);
}

void MQTTClientBuilding::receive()
{
  _client.loop();
}

void MQTTClientBuilding::onConnectionSubscribe()
{
  _client.subscribe(mqtt_topic_solarPower, [this](const String &topic, const String &payload)
  { 
    _isSolarPowerArrivedFlag = true;
    _totalSolarPower = payload.toFloat();
  });
  _client.subscribe(mqtt_topic_PLBmode, [this](const String &topic, const String &payload)
  {
    _event = PLBEvents::EV_SwitchMode;
    if (payload == "PLBAutoMode") {
      _PLBMode = PLBModes::MO_Auto;
    }
    else if (payload == "PLBManualMode") {
      _PLBMode = PLBModes::MO_Manual;
    }
  });
}

/* ----------------- Interface's functions */
float MQTTClientBuilding::calculateSolarPower()
{
  send(mqtt_topic_calculateSolarPower, "Calculate Solar Power");
  long lastTime = millis();
  while(!_isSolarPowerArrivedFlag)
  {
    if (_client.isConnected()) _client.loop();
    if(millis() - lastTime >= _solarPowerTimeout)
    {
      _isConnected = false;
      if ((_isConnected != _lastConnectionState) && !_isConnected) notifyDashboard("Building failed to respond."); //This works but it pushes it before the mqtt is able to subscribe or connect. Try serial print to see.
      _lastConnectionState = _isConnected;
      return 0;
    }
  }
  _isConnected = true;
  if ((_isConnected != _lastConnectionState) && _isConnected)
  {
    _lastConnectionState = _isConnected;
    notifyDashboard("Building is online again.");
  }
  _isSolarPowerArrivedFlag = false;
  return _totalSolarPower;
}

void MQTTClientBuilding::charge(float power)
{
  send(mqtt_topic_charge_building, (String)power);
}

float MQTTClientBuilding::getCurrentSolarPower()
{
  return _totalSolarPower;
}

void MQTTClientBuilding::notifyDashboard(String message)
{
  send(mqtt_topic_notifyDashboard, message);
}
