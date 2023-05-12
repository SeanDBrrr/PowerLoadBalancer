#ifndef MQTTCLIENTBUILDING_HPP
#define MQTTCLIENTBUILDING_HPP

#include <string>
#include "IBuilding.h"
#include "EspMQTTClient.h"

using namespace std;

const char *mqtt_topic_solarPower = "group4/powerProduced";
const char *mqtt_topic_calculateSolarPower= "group4/calculateSolarPower";
const char *mqtt_topic_charge_building = "group4/chargeBuilding";

class MQTTClientBuilding
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