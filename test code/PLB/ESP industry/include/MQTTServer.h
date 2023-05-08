#ifndef MQTTSERVER_H
#define MQTTSERVER_H

#include <string>
#include "IBuilding.h"
#include "IStation.h"
#include "EspMQTTClient.h"
#include <string.h>

using namespace std;

class MQTTServer : public IStation, public IBuilding
{
private:
    EspMQTTClient& _client;
    int _id;
    int _powerProduced;
    bool _buildingState;
    bool _receivedRequest;
    int _stationId;
public:
    MQTTServer();

    EspMQTTClient& getClient();
    int getId();
    void charge(int power);
    void switchMode(StationModes mode);
    int getPowerProduced();
    bool getState();
    void send(String topic, String message);
    void receive();
    void onConnectionEstablished();
    int getStationId();
    bool getSupplyRequest();
    void setSupplyRequest();
    ~MQTTServer();
};

MQTTServer::~MQTTServer()
{
}

#endif