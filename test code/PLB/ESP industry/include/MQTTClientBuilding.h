#ifndef MQTTCLIENTBUILDING_HPP
#define MQTTCLIENTBUILDING_HPP

#include <string>
#include "IBuilding.h"
#include "EspMQTTClient.h"

using namespace std;



class MQTTClientBuilding : public IBuilding
{
private:
    EspMQTTClient _client = EspMQTTClient(name, password, broker_ip, mqtt_module, port);
    bool _isSolarPowerArrivedFlag;
    int _totalSolarPower;

    const char *mqtt_topic_solarPower = "group4/powerProduced";
    const char *mqtt_topic_calculateSolarPower= "group4/calculateSolarPower";
    const char *mqtt_topic_charge_building = "group4/chargeBuilding";

public:
    const char *name = "Kiwy";
    const char *password = "aquamagic23";
    const char *mqtt_module = "Group4-PLB-Building";
    const char *broker_ip = "192.168.61.23";
    short port = 1883;

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