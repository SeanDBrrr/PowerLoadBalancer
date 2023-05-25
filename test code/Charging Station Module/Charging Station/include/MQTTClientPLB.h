#pragma once

#include <string>
#include "IPLB.h"
#include "EspMQTTClient.h"
#include <string.h>
#include "Event.h"

using namespace std;

class MQTTClientPLB : public IPLB
{
private:
    Event _event;
    int _id;
    float _powerReceived;
    bool _isPowerReceivedFlag;

    String name = "Kiwy";
    String password = "aquamagic23";
    String mqtt_module = "Group4-Charger" + String(_id);
    String broker_ip = "192.168.4.23";
    String mqtt_topic_requestPower = "group4/requestPowerSupply" + String(_id);
    String mqtt_topic_stopSupply = "group4/stopPowerSupply" + String(_id);
    String mqtt_topic_receivePower = "group4/chargeStation" + String(_id);
    String mqtt_topic_directorId = "group4/directorId";
    short port = 1883;
    EspMQTTClient _client = EspMQTTClient(
        name.c_str(),
        password.c_str(),
        broker_ip.c_str(),
        mqtt_module.c_str(),
        port
    );
public:
    MQTTClientPLB(int id);

    EspMQTTClient& getClient();
    float getPowerReceived();
    void supplyPowerToStation(int id);
    void stopSupplyToStation(int id);
    void checkDirector(int id);
    void send(String topic, String message);
    void receive();
    void onConnectionSubscribe();
    Event loop();
    ~MQTTClientPLB() = default;
};