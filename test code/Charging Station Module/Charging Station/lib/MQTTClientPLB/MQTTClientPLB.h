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
    bool _wifiConnectedFlag;
    bool _mqttConnectedFlag;
    bool _isPLBConnected;
    bool _plbWasConnectedBefore;
    DirectorState _directorState;
    StationMode _stationMode;
    unsigned long _previousTime;
    bool _wifiConnected;
    bool _mqttConnected;
    int _wifiTrials;
    int _mqttTrials;
    const int _INTERVAL = 1000;
    const int _TRIALS = 40;


    String name = "Kiwy";
    String password = "aquamagic23";
    String mqtt_module = "Group4-Charger" + static_cast<String>(_id);
    String broker_ip = "192.168.225.23";
    String mqtt_topic_requestPower = "group4/requestPowerSupply" + static_cast<String>(_id);
    String mqtt_topic_stopSupply = "group4/stopPowerSupply" + static_cast<String>(_id);
    String mqtt_topic_receivePower = "group4/chargeStation" + static_cast<String>(_id);
    String mqtt_topic_directorId = "group4/directorId" + static_cast<String>(_id);
    String mqtt_topic_directorResponse = "group4/directorResponse" + static_cast<String>(_id);
    String mqtt_topic_heartbeat = "group4/heartbeat";
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
    void WaitingForPower();
    void checkConnection();
    void SetPowerRecievedFlag(bool powerRecievedFlag);
    void birthMessage();
    bool getPowerReceievedFlag();
    Event getConnectionStatusEvent();
    Event getPLBChargingStatusEvent();
    Event getEvent();
    void callClientLoop();
    ~MQTTClientPLB() = default;
};