#ifndef MQTTCLIENTSTATION_H
#define MQTTCLIENTSTATION_H

#include <string>
#include "IStation.h"
#include "EspMQTTClient.h"

using namespace std;

const char *mqtt_topic_mode = "group4/mode";
const char *mqtt_topic_StationId = "group4/StationId";
const char *mqtt_topic_directorId = "group4/directorId";
const char *mqtt_topic_requestSupply = "group4/requestSupply";
const char *mqtt_topic_charge = "group4/charge";

class MQTTClientStation : public IStation
{
private:
    EspMQTTClient& _client;
    PLBEvents _event;
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

    int getId();
    void charge(float power);
    void switchMode(StationModes mode);

    bool isSupplyRequest();
    bool isDirectorChecked();
    void clearSupplyRequest();
    void clearDirectorCheck();
    void loop();
};

#endif