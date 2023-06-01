#ifndef MQTTCLIENTPLB_H
#define MQTTCLIENTPLB_H

#include "IPLB.h"
#include <EspMQTTClient.h>

class MQTTClientPLB : public IPLB
{
private:
    bool _isBeingCharged;
    bool _solarPowerRequested;
    double _powerFromPLB;
    String mqtt_topic_calculateSolarPower= "group4/calculateSolarPower";
    String mqtt_topic_charge_building = "group4/chargeBuilding";
    String mqtt_topic_send_power = "group4/powerProduced";
    String name = "S21 FE J";
    String password = "yo koaster";
    String mqtt_module = "Group4-Building";
    String broker_ip = "192.168.137.132";
    short port = 1883;
    EspMQTTClient _client = EspMQTTClient(name.c_str(), password.c_str(), broker_ip.c_str(), mqtt_module.c_str(), port);

public:
    MQTTClientPLB();
    ~MQTTClientPLB() {}
    EspMQTTClient& getClient();
    void send(String topic, String message);
    BuildingEvents receive();
    void onConnectionSubscribe();
    void supplyPowerToBuilding(double power);
    double getPower();
};

#endif