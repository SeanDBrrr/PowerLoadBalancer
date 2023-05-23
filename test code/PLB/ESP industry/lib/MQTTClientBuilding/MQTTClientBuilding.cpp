#include "MQTTClientBuilding.h"

MQTTClientBuilding::MQTTClientBuilding()
{
  Serial.println("OBJECT CREATED: BUILDING");
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
        _totalSolarPower = payload.toInt(); });
}

int MQTTClientBuilding::calculateSolarPower()
{
  send(mqtt_topic_calculateSolarPower, "Calculate Solar Power");
  long lastTime = millis();
  while(!_isSolarPowerArrivedFlag)
  {
    if(millis() - lastTime >= 5000)
    {
      return -1;
    }
  }
  _isSolarPowerArrivedFlag = 0;
  return _totalSolarPower;
}

void MQTTClientBuilding::charge(float power)
{
  send(mqtt_topic_charge_building, (String)power);
}
