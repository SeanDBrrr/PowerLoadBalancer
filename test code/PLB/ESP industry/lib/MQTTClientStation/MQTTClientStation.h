#ifndef MQTTCLIENTSTATION_H
#define MQTTCLIENTSTATION_H

#include <vector>
#include <queue>
#include <string>

#include "IStation.h"
#include "EspMQTTClient.h"
#include "Enums.h"

using namespace std;

class MQTTClientStation : public IStation
{
private:
    uint32_t _directorId;
    int _stationId;
    StationModes _stationMode;
    PLBModes _mode;

    void _setStationTopics();

    String mqtt_topic_stationMode = "group4/stationMode";
    String mqtt_topic_StationId = "group4/StationId";
    String mqtt_topic_directorId = "group4/directorId";
    String mqtt_topic_requestSupply = "group4/requestPowerSupply";
    String mqtt_topic_stopSupply = "group4/stopPowerSupply";
    String mqtt_topic_charge_station = "group4/chargeStation";
    String mqtt_topic_directorValidate = "group4/directorResponse";
    String mqtt_topic_stationHeartbeat = "group4/heartbeat";
    String mqtt_topic_notifyDashboard = "group4/notifyDashboard";
    String mqtt_module = "Group4-PLB-Station";
    String name = "S21 FE J";
    String password = "yo koaster";
    String broker_ip = "192.168.190.132";
    short port = 1883;
    EspMQTTClient _client = EspMQTTClient(
        name.c_str(),
        password.c_str(),
        broker_ip.c_str(),
        mqtt_module.c_str(),
        port
    );
public:
/* Constructor and Destructor */
    MQTTClientStation(int Id);
    ~MQTTClientStation();
    
    /* These are containers that all the MQTTClientStation object instances share, 
        every time a event is triggered, they are updated */

    /* Stores stations ID that requested power */
    static std::queue<int> idEvents;
    /* Stores all PLBEvents that occured over loop */
    static std::vector<PLBEvents> events;

    /* MQTT related */
    EspMQTTClient& getClient();
    void send(String topic, String message);
    void receive();
    void onConnectionSubscribe();

    /* Events Getters */
    std::vector<PLBEvents>& getEvents();
    std::queue<int>& getIdEvents();

    /* Interface functions */
    int getId();
    uint32_t getDirectorId();
    void validateDirector(DirectorState directorState);
    void charge(float power);
    void switchMode(StationModes mode);
    void notifyDashboard(String message);
    StationModes getStationMode();
};

#endif