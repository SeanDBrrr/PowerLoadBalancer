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
public:
    MQTTServer();

    EspMQTTClient& getClient();
    int getId();
    void charge(int power) = 0;
    void switchMode(StationModes mode) = 0;
    int getPowerProduced() = 0;
    bool getState() = 0;
    void send(String topic, String message);
    void receive();
    void onConnectionEstablished();
    ~MQTTServer();
};

MQTTServer::~MQTTServer()
{
}

#endif