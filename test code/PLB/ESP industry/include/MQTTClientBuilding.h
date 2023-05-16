#ifndef MQTTCLIENTBUILDING_HPP
#define MQTTCLIENTBUILDING_HPP

#include <string>
#include "IBuilding.h"
#include "EspMQTTClient.h"
#include "Topics.h"

using namespace std;

class MQTTClientBuilding : public IBuilding
{
private:
    EspMQTTClient& _client;
    bool _isSolarPowerArrivedFlag;
    int _totalSolarPower;

public:
    MQTTClientBuilding();
    ~MQTTClientBuilding();

    EspMQTTClient& getClient();
    void send(String topic, String message);
    void receive();
    void onConnectionSubscribe();

    int calculateSolarPower();
    void charge(float power);
};

#endif