#ifndef MQTTCLIENTSTATION_H
#define MQTTCLIENTSTATION_H

#include <string>
#include "IBuilding.h"
#include "IStation.h"
#include "EspMQTTClient.h"

using namespace std;

const char *mqtt_topic_mode = "group4/mode";
const char *mqtt_topic_StationId = "group4/StationId";
const char *mqtt_topic_directorId = "group4/directorId";
const char *mqtt_topic_requestSupply = "group4/requestSupply";
const char *mqtt_topic_powerProduced = "group4/powerProduced";
const char *mqtt_topic_buildingState = "group4/buildingState";

class MQTTClientStation
{
private:
    EspMQTTClient& _client;
    bool _isRequestSupplyFlag;
    bool _isDirectorDetectedFlag;
    int _directorId;
    int _stationId;

public:
    MQTTClientStation();
    ~MQTTClientStation();

    EspMQTTClient& getClient();
    void send(String topic, String message);
    void receive();
    void onConnectionSubscribe();
    inline int getDirectorId() { return _directorId; }
    inline int getStationId() { return _stationId; }
};

#endif