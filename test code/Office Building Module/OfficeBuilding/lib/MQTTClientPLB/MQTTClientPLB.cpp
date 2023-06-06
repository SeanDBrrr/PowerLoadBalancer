#include "MQTTClientPLB.h"

MQTTClientPLB::MQTTClientPLB() : _isStartedCharging{false}, _solarPowerRequested{false} {}

EspMQTTClient &MQTTClientPLB::getClient()
{
    return _client;
}

void MQTTClientPLB::send(String topic, String message)
{
    _client.publish(topic, message);
}

BuildingEvents MQTTClientPLB::receive()
{
    _client.loop();

    static unsigned long lastTime = 0;
    if (millis() - lastTime >= 5000)
    {
        send(mqtt_topic_buildingHeartbeat, "Building_heartbeat");
        lastTime = millis();
    }

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

void MQTTClientPLB::onConnectionSubscribe()
{
    _client.subscribe(mqtt_topic_calculateSolarPower, [this](const String &topic, const String &payload)
                      {
        _solarPowerRequested = true;
        _powerFromPLB = payload.toDouble(); });
    _client.subscribe(mqtt_topic_charge_building, [this](const String &topic, const String &payload)
                      {
        _isStartedCharging = true;
        _powerFromPLB = payload.toDouble(); });
}

void MQTTClientPLB::supplyPowerToBuilding(double power)
{
    send(mqtt_topic_send_power, (String)power);
}

String MQTTClientPLB::checkConnection()
{
    String message;
    int connectionCase = 0;
    if (!_client.isWifiConnected() && connectionCase == 0)
    {
        for (size_t i = 0; i < 10; i++)
        {
            message = "Connecting WiFi attempt: " + static_cast<String>(i);
            delay(1000);
        }
        message = "WiFi connection Failed";
        connectionCase = 1;
    }

    if (!_client.isMqttConnected() && connectionCase == -1)
    {
        for (size_t i = 0; i < 10; i++)
        {
            message = "Connecting Mqtt attempt: " + static_cast<String>(i);
            delay(1000);
        }
        message = "Mqtt connection Failed";
        connectionCase = 1;
    }

    return message;
}

double MQTTClientPLB::getPower()
{
    return _powerFromPLB;
}