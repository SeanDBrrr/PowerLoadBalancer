#ifndef MQTTCLIENTSTATION_H
#define MQTTCLIENTSTATION_H

#include "IStation.h"
#include "EspMQTTClient.h"
#include "Enums.h"
#include <vector>


using namespace std;

class MQTTClientStation : public IStation
{

private:
    PLBEvents _event;
    bool _isRequestSupplyFlag;
    bool _isStopSupplyFlag;
    bool _isDirectorDetectedFlag;
    uint32_t _directorId;
    int _stationId;
    void _setStationId();

    String mqtt_topic_mode = "group4/stationMode";
    String mqtt_topic_StationId = "group4/StationId";
    String mqtt_topic_directorId = "group4/directorId";
    String mqtt_topic_requestSupply = "group4/requestPowerSupply";
    String mqtt_topic_stopSupply = "group4/stopPowerSupply";
    String mqtt_topic_charge_station = "group4/chargeStation";
    String mqtt_topic_directorValidate = "group4/directorResponse";
    String name = "S21 FE J";
    String password = "yo koaster";
    String mqtt_module = "Group4-PLB-Station";
    String broker_ip = "192.168.137.132";
    short port = 1883;
    EspMQTTClient _client = EspMQTTClient(
        name.c_str(),
        password.c_str(),
        broker_ip.c_str(),
        mqtt_module.c_str(),
        port
    );

public:
    MQTTClientStation(int Id);
    ~MQTTClientStation();

    EspMQTTClient& getClient();
    void send(String topic, String message);
    void receive();
    PLBEvents getEvent();
    void onConnectionSubscribe();

    void validateDirector(DirectorState directorState);
    int getId();
    uint32_t getDirectorId();
    void charge(float power);
    void switchMode(StationModes mode);
};

#endif