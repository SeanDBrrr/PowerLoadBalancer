#ifndef MQTTCLIENTSTATION_H
#define MQTTCLIENTSTATION_H

#include "IStation.h"
#include "EspMQTTClient.h"
#include "Enums.h"
#include <vector>
#include <queue>
#include <string>
#include <set>


using namespace std;

class MQTTClientStation : public IStation
{
public:
    MQTTClientStation(int Id);
    ~MQTTClientStation();
    
    /* These are containers that all the MQTTClientStation object instances share, 
        every time a event is triggered, they are updated */

    /* Stores stations ID that requested power */
    static std::queue<int> supplyRequestEvents;
    /* Stores stations ID that requested to stop the supply */
    static std::queue<int> stopSupplyEvents;
    /* Stores stations ID where a director swiped his card (the corresponding ID is already stored in _directorId) */
    static std::queue<int> directorEvents;
    /* Stores all PLBEvents that occured over loop */
    static std::vector<PLBEvents> events;

    /* MQTT related */
    EspMQTTClient& getClient();
    void send(String topic, String message);
    void receive();
    void onConnectionSubscribe();

    /* Events Getters */
    std::vector<PLBEvents>& getEvents();
    std::queue<int>& getSupplyEvents();
    std::queue<int>& getStopEvents();
    std::queue<int>& getDirectorEvents();

    /* Interface functions */
    void validateDirector(DirectorState directorState);
    int getId();
    uint32_t getDirectorId();
    void charge(float power);
    void switchMode(StationModes mode);

private:
    uint32_t _directorId;
    int _stationId;
    void _setStationTopics();

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
    String broker_ip = "192.168.206.132";
    short port = 1883;
    EspMQTTClient _client = EspMQTTClient(
        name.c_str(),
        password.c_str(),
        broker_ip.c_str(),
        mqtt_module.c_str(),
        port
    );
};

#endif