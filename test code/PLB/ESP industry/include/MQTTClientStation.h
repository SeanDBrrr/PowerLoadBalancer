#ifndef MQTTCLIENTSTATION_H
#define MQTTCLIENTSTATION_H
#pragma once

#include "IStation.h"
#include "EspMQTTClient.h"
#include "Topics.h"

using namespace std;

class MQTTClientStation : public IStation
{
private:
    EspMQTTClient _client = EspMQTTClient(name, password, broker_ip, mqtt_module, port);
    PLBEvents _event;
    bool _isRequestSupplyFlag;
    bool _isDirectorDetectedFlag;
    int _directorId;
    int _stationId;
    void _setStationId();

    const char *mqtt_topic_mode = "group4/mode";
    const char *mqtt_topic_StationId = "group4/StationId";
    const char *mqtt_topic_directorId = "group4/directorId";
    const char *mqtt_topic_requestSupply = "group4/requestSupply";
    const char *mqtt_topic_charge_station = "group4/chargeStation";

public:
    MQTTClientStation(int Id);
    ~MQTTClientStation();

    EspMQTTClient& getClient();
    void send(String topic, String message);
    void receive();
    void onConnectionSubscribe();

    int getId();
    void charge(float power);
    void switchMode(StationModes mode);
    PLBEvents loop();

    const char *name = "Kiwy";
    const char *password = "aquamagic23";
    const char *mqtt_module = "Group4-PLB-Station";
    const char *broker_ip = "192.168.61.23";
    short port = 1883;
};

#endif