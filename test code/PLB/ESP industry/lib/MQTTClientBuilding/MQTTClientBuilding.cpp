#include "MQTTClientBuilding.h"

MQTTClientBuilding::MQTTClientBuilding() : 
_isSolarPowerArrivedFlag(false), 
_totalSolarPower(0),
_solarPowerTimeout(5000),
_isConnected(true),
_lastConnectionState(true)
{
  calculateSolarPower();
}

MQTTClientBuilding::~MQTTClientBuilding() {}

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
}

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
