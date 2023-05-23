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
}

void MQTTClientPLB::onConnectionSubscribe()
{
  _client.subscribe(mqtt_topic_receivePower, [this](const String & topic, const String & payload) {
    _isPowerReceivedFlag = true;
    _powerReceived = payload.toFloat();
  });
}

void MQTTClientPLB::checkDirector(int id)
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

Event MQTTClientPLB::loop()
{
  receive();
  _event = Event::noEvent;
  if(_isPowerReceivedFlag)
  {
    _event = Event::EV_CHARGING;
    _isPowerReceivedFlag = 0;
  }
  return _event;
}



