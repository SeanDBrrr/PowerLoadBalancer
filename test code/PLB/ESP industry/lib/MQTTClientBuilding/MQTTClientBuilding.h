#ifndef MQTTCLIENTBUILDING_HPP
#define MQTTCLIENTBUILDING_HPP

#include <string>
#include "IBuilding.h"
#include "EspMQTTClient.h"

using namespace std;

class MQTTClientBuilding : public IBuilding
{
private:
    bool _isSolarPowerArrivedFlag;
    float _totalSolarPower;

    String mqtt_topic_solarPower = "group4/powerProduced";
    String mqtt_topic_calculateSolarPower= "group4/calculateSolarPower";
    String mqtt_topic_charge_building = "group4/chargeBuilding";
    String name = "S21 FE J";
    String password = "yo koaster";
    String mqtt_module = "Group4-PLB-Building";
    String broker_ip = "192.168.206.132";
    short port = 1883;
    EspMQTTClient _client = EspMQTTClient(
        name.c_str(),
        password.c_str(),
        broker_ip.c_str(),
        mqtt_module.c_str(),
        port
    );

public:
    MQTTClientBuilding();
    ~MQTTClientBuilding();

    EspMQTTClient& getClient();
    void send(String topic, String message);
    void receive();
    void onConnectionSubscribe();

    float calculateSolarPower();
    void charge(float power);
    float getCurrentSolarPower();
};

#endif