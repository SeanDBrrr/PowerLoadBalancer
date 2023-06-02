#include "MQTTClientPLB.h"

MQTTClientPLB::MQTTClientPLB() : _isStartedCharging{false}, _solarPowerRequested{false} {}

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
        _solarPowerRequested = false;
        return BuildingEvents::EV_SendSolarPower;
    }

    if (_isStartedCharging)
    {
        _isStartedCharging = false;
        return BuildingEvents::EV_ChargeBuilding;
    }
    return BuildingEvents::NoEvent;
}

void 
MQTTClientPLB::onConnectionSubscribe()
{
    _client.subscribe(mqtt_topic_calculateSolarPower, [this](const String &topic, const String &payload)
    {
        _solarPowerRequested = true;
        _powerFromPLB = payload.toDouble();
    });
    _client.subscribe(mqtt_topic_charge_building, [this](const String &topic, const String &payload)
    {
        _isStartedCharging = true;
        _powerFromPLB = payload.toDouble();
    });
}

void MQTTClientPLB::supplyPowerToBuilding(double power)  
{
    send(mqtt_topic_send_power, (String)power);
}

double MQTTClientPLB::getPower() 
{
    return _powerFromPLB;
}