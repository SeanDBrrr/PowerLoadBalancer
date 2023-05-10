#ifndef MQTTCLIENTBUILDING_HPP
#define MQTTCLIENTBUILDING_HPP

#include <string>
#include "IBuilding.h"
#include "EspMQTTClient.h"

using namespace std;

const char *mqtt_topic_solarPower = "group4/powerProduced";
const char *mqtt_topic_charge = "group4/charge";

class MQTTClientBuilding
{
private:
    EspMQTTClient& _client;
    bool _isSolarPowerRequestFlag;
    int _totalSolarPower;

public:
    MQTTClientBuilding();
    ~MQTTClientBuilding();

    EspMQTTClient& getClient();
    void send(String topic, String message);
    void receive();
    void onConnectionSubscribe();
    inline int getSolarPower() { return _totalSolarPower; }

    int calculateSolarPower();
    void charge(float power);
    void loop();
};

#endif