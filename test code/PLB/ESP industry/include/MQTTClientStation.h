#ifndef MQTTCLIENTSTATION_H
#define MQTTCLIENTSTATION_H

#include "IStation.h"
#include "EspMQTTClient.h"
#include "Topics.h"

using namespace std;

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