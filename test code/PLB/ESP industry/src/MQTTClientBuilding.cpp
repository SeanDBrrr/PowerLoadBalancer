#include "MQTTClientBuilding.h"

const char *name = "Kiwy";
const char *password = "aquamagic23";
const char *mqtt_module = "Group4-PLB-Building";
const char *broker_ip = "192.168.61.23";
short port = 1883;

EspMQTTClient client(name, password, broker_ip, mqtt_module, port);

MQTTClientBuilding::MQTTClientBuilding() : _client(client) {}

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
