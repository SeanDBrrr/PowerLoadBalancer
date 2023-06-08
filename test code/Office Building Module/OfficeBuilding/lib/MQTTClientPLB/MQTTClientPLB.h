#ifndef MQTTCLIENTPLB_H
#define MQTTCLIENTPLB_H

#include "IPLB.h"
#include <EspMQTTClient.h>

class MQTTClientPLB : public IPLB
{
private:
    bool _isStartedCharging;
    bool _solarPowerRequested;
    double _powerFromPLB;
    bool _wifiConnected;
    bool _mqttConnected;
    int _wifiTrials;
    int _mqttTrials;
    unsigned long _previousTime;
    const int _INTERVAL = 1000;
    const int _TRIALS = 40;

    BuildingEvents _event;
    String mqtt_topic_calculateSolarPower = "group4/calculateSolarPower";
    String mqtt_topic_charge_building = "group4/chargeBuilding";
    String mqtt_topic_send_power = "group4/powerProduced";
    String mqtt_topic_buildingHeartbeat = "group4/buildingHeartbeat";
    String name = "Kiwy";
    String password = "aquamagic23";
    String mqtt_module = "Group4-Building";
    String broker_ip = "192.168.140.23";
    short port = 1883;
    EspMQTTClient _client = EspMQTTClient(name.c_str(), password.c_str(), broker_ip.c_str(), mqtt_module.c_str(), port);

public:
    MQTTClientPLB();
    ~MQTTClientPLB() {}
    EspMQTTClient &getClient();
    void send(String topic, String message);
    BuildingEvents receive();
    BuildingEvents connectionStatusEvent();
    void onConnectionSubscribe();
    void supplyPowerToBuilding(double power);
    double getPower();
};

#endif