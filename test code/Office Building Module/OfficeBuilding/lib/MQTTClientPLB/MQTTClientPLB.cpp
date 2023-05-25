#include "MQTTClientPLB.h"

MQTTClientPLB::MQTTClientPLB() : _isBeingCharged{false}, _solarPowerRequested{false} {}

EspMQTTClient& 
MQTTClientPLB::getClient() 
{ 
    return _client; 
}

void 
MQTTClientPLB::send(String topic, String message)
{
    _client.publish(topic, message);
}
    
BuildingEvents 
MQTTClientPLB::receive()
{
    _client.loop();
    if (_solarPowerRequested)
    {
        return BuildingEvents::EV_RequestSolarPower;
    }
    return BuildingEvents::NoEvent;
}


void 
MQTTClientPLB::onConnectionSubscribe()
{
    _client.subscribe(mqtt_topic_calculateSolarPower, [this](const String &topic, const String &payload)
    {
        _solarPowerRequested = true;
    });
}

void 
MQTTClientPLB::supplyPowerToBuilding()
{
    send(mqtt_topic_charge_building, "Charge Building");
}