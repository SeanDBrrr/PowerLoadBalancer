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
const char *mqtt_topic_charge_station = "group4/chargeStation";

class MQTTClientStation : public IStation
{
private:
    EspMQTTClient& _client;
    PLBEvents _event;
    bool _isRequestSupplyFlag;
    bool _isDirectorDetectedFlag;
    int _directorId;
    int _stationId;
    void _setStationId();

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
};

#endif