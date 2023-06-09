#include "MQTTClientPLB.h"

MQTTClientPLB::MQTTClientPLB() : _isStartedCharging(false),
                                 _solarPowerRequested(false),
                                 _powerFromPLB(0),
                                 _wifiConnected(false),
                                 _mqttConnected(false),
                                 _wifiTrials(0),
                                 _mqttTrials(0),
                                 _previousTime(0)
{
}

EspMQTTClient &MQTTClientPLB::getClient()
{
    return _client;
}

void MQTTClientPLB::send(String topic, String message)
{
    _client.publish(topic, message);
}

BuildingEvents MQTTClientPLB::getConnectionStatusEvent()
{
    BuildingEvents buildingEvents = BuildingEvents::NoEvent;

    if (millis() - _previousTime >= _INTERVAL)
    {
        _previousTime = millis();

        if (!_client.isWifiConnected() && _wifiTrials < _TRIALS)
        {
            _wifiTrials++;
            buildingEvents = BuildingEvents::EV_WIFI_TRIALS;
            _wifiConnected = false;
        }
        else if (!_client.isWifiConnected() && _wifiTrials == _TRIALS)
        {
            _wifiTrials++;
            buildingEvents = BuildingEvents::EV_WIFI_NOT_CONNECTED;
        }
        if (!_client.isMqttConnected() && _wifiConnected && _mqttTrials < _TRIALS)
        {
            _mqttTrials++;
            _mqttConnected = false;
            buildingEvents = BuildingEvents::EV_MQTT_TRIALS;
        }
        else if (!_client.isMqttConnected() && _wifiConnected && _mqttTrials == _TRIALS)
        {
            _mqttTrials++;
            buildingEvents = BuildingEvents::EV_MQTT_NOT_CONNECTED;
        }
    }

    if (_client.isWifiConnected() && _wifiTrials > 0 && _wifiTrials < _TRIALS)
    {
        _wifiTrials = 0;
        _mqttTrials = 0;
        buildingEvents = BuildingEvents::EV_WIFI_CONNECTED;
        _wifiConnected = true;
    }

    if (_client.isMqttConnected() && _wifiConnected && _mqttTrials > 0 && _mqttTrials < _TRIALS)
    {
        _mqttTrials = 0;
        _mqttConnected = true;
        buildingEvents = BuildingEvents::EV_MQTT_CONNECTED;
    }

    return buildingEvents;
}

BuildingEvents MQTTClientPLB::receive()
{
    BuildingEvents buildingEvents;
    _client.loop();
    buildingEvents = BuildingEvents::NoEvent;

    buildingEvents = getConnectionStatusEvent();

    if (_solarPowerRequested)
    {
        _solarPowerRequested = false;
        buildingEvents = BuildingEvents::EV_SendSolarPower;
    }

    if (_isStartedCharging)
    {
        _isStartedCharging = false;
        buildingEvents = BuildingEvents::EV_ChargeBuilding;
    }
    return buildingEvents;
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
    send(mqtt_topic_send_power, static_cast<String>(power));
}

double MQTTClientPLB::getPower()
{
    return _powerFromPLB;
}
