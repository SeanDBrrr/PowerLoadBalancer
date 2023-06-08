#include "MQTTClientBuilding.h"

MQTTClientBuilding::MQTTClientBuilding() : 
_isSolarPowerArrivedFlag(false), 
_totalSolarPower(0),
_solarPowerTimeout(5000),
_isConnected(true),
_lastConnectionState(true)
{
  _client.enableDebuggingMessages();
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
    Serial.println("OnConnectionSubscribe(): _isSolarPowerArrivedFlag == true.");
    _totalSolarPower = payload.toFloat();
    Serial.print("_totalSolarPower = "); Serial.println(_totalSolarPower);
  });
}

float MQTTClientBuilding::calculateSolarPower()
{
  _isSolarPowerArrivedFlag = false;
  #if COMMENTS
  Serial.print("Before publishing Calculate Solar Power: _isSolarPowerArrivedFlag = "); Serial.println(_isSolarPowerArrivedFlag);
  #endif
  send(mqtt_topic_calculateSolarPower, "Calculate Solar Power");
  #if COMMENTS
  Serial.print("After publishing Calculate Solar Power: _isSolarPowerArrivedFlag = "); Serial.println(_isSolarPowerArrivedFlag);
  #endif
  long lastTime = millis();
  while(!_isSolarPowerArrivedFlag)
  {
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
  #if COMMENTS
  Serial.print("Before returning _totalSolarPower: _isSolarPowerArrivedFlag = "); Serial.println(_isSolarPowerArrivedFlag);
  #endif
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
