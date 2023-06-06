#include "MQTTClientBuilding.h"

MQTTClientBuilding::MQTTClientBuilding() : 
_isSolarPowerArrivedFlag(false), 
_totalSolarPower(0),
_solarPowerTimeout(5000)
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

  static bool lastConnectionState = _isConnected;
  long lastTime = millis();
  while(!_isSolarPowerArrivedFlag)
  {
    if(millis() - lastTime >= _solarPowerTimeout)
    {
      if ((_isConnected != lastConnectionState) && !_isConnected) notifyDashboard("Building failed to respond.");
      _isConnected = false;
      lastConnectionState = _isConnected;
      return 0;
    }
  }
  _isConnected = true;
  if ((_isConnected != lastConnectionState) && _isConnected)
  {
    lastConnectionState = _isConnected;
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
