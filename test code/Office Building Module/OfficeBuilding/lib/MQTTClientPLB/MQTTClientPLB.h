#ifndef MQTTCLIENTPLB_H
#define MQTTCLIENTPLB_H

#include "IPLB.h"
#include <EspMQTTClient.h>

class MQTTClientPLB : public IPLB
{
private:
    bool _isBeingCharged;
    bool _solarPowerRequested;

    String mqtt_topic_calculateSolarPower= "group4/calculateSolarPower";
    String mqtt_topic_charge_building = "group4/chargeBuilding";
    String name = "Kiwy";
    String password = "aquamagic23";
    String mqtt_module = "Group4-Building";
    String broker_ip = "192.168.4.23";
    short port = 1883;
    EspMQTTClient _client = EspMQTTClient(name.c_str(), password.c_str(), broker_ip.c_str(), mqtt_module.c_str(), port);

public:
    MQTTClientPLB();
    ~MQTTClientPLB() {}
    EspMQTTClient& getClient();
    void send(String topic, String message);
    BuildingEvents receive();
    void onConnectionSubscribe();
    void supplyPowerToBuilding();
};

#endif