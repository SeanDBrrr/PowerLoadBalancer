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

Event MQTTClientPLB::getConnectionStatusEvent()
{
    _event = Event::NoEvent;

    if (millis() - _previousTime >= _INTERVAL)
    {
        _previousTime = millis();

        if (!_client.isWifiConnected() && _wifiTrials < _TRIALS)
        {
            _wifiTrials++;
            _event = Event::EV_WIFI_TRIALS;
            _wifiConnected = false;
        }
        else if (!_client.isWifiConnected() && _wifiTrials == _TRIALS)
        {
            _wifiTrials++;
            _event = Event::EV_WIFI_NOT_CONNECTED;
        }
        if (!_client.isMqttConnected() && _wifiConnected && _mqttTrials < _TRIALS)
        {
            _mqttTrials++;
            _mqttConnected = false;
            _event = Event::EV_MQTT_TRIALS;
        }
        else if (!_client.isMqttConnected() && _wifiConnected && _mqttTrials == _TRIALS)
        {
            _mqttTrials++;
            _event = Event::EV_MQTT_NOT_CONNECTED;
        }
    }

    if (_client.isWifiConnected() && _wifiTrials > 0 && _wifiTrials < _TRIALS)
    {
        _wifiTrials = 0;
        _mqttTrials = 0;
        _event = Event::EV_WIFI_CONNECTED;
        _wifiConnected = true;
    }

    if (_client.isMqttConnected() && _wifiConnected && _mqttTrials > 0 && _mqttTrials < _TRIALS)
    {
        _mqttTrials = 0;
        _mqttConnected = true;
        _event = Event::EV_MQTT_CONNECTED;
    }

    return _event;
}

Event& MQTTClientPLB::receive()
{
    _client.loop();
    _event = Event::NoEvent;

    _event = getConnectionStatusEvent();

    if (_solarPowerRequested)
    {
        _solarPowerRequested = false;
        _event = Event::EV_SEND_SOLAR_POWER;
    }

    if (_isStartedCharging)
    {
        _isStartedCharging = false;
        _event = Event::EV_CHARGE_BUILDING;
    }
    return _event;
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

void MQTTClientPLB::sendStateToPLB(State buildingState)
{
    if (buildingState == State::STATE_OPEN)
    {
        send(mqtt_topic_buildingState, "Open");
    }
    else if(buildingState == State::STATE_CLOSED)
    {
        send(mqtt_topic_buildingState, "Close");
    }
}

double MQTTClientPLB::getPower()
{
    return _powerFromPLB;
}
