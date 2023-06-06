#pragma once

#include <string>
#include "IPLB.h"
#include "EspMQTTClient.h"
#include <string.h>
#include "Enums.h"

using namespace std;

class MQTTClientPLB : public IPLB
{
private:
    Event _event;
    int _id;
    float _powerReceived;
    bool _isPowerReceivedFlag;
    bool _isDirectorResponseFlag;
    bool _isModeChangedFlag;
    DirectorState _directorState;
    StationMode _stationMode;

    String name = "Kiwy";
    String password = "aquamagic23";
    String mqtt_module = "Group4-Charger" + static_cast<String>(_id);
    String broker_ip = "192.168.140.23";
    String mqtt_topic_requestPower = "group4/requestPowerSupply" + static_cast<String>(_id);
    String mqtt_topic_stopSupply = "group4/stopPowerSupply" + static_cast<String>(_id);
    String mqtt_topic_receivePower = "group4/chargeStation" + static_cast<String>(_id);
    String mqtt_topic_directorId = "group4/directorId" + static_cast<String>(_id);
    String mqtt_topic_directorResponse = "group4/directorResponse" + static_cast<String>(_id);
    String mqtt_topic_stationHeartbeat = "group4/stationHeartbeat" + static_cast<String>(_id);
    String mqtt_topic_mode = "group4/stationMode";
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
    void checkDirector(uint32_t id);
    void send(String topic, String message);
    void receive();
    void onConnectionSubscribe();
    void directorTimeout (int waitingTime);
    
    Event getEvent();
    ~MQTTClientPLB() = default;
};