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
    float _directorId;
    int _stationId;
    StationModes _stationMode;
    PLBModes _mode;
    unsigned long _timer;
    float _slope;

    void _setStationTopics();
    void _setTimer(float power);

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
    String name = "Kiwy";
    String password = "aquamagic23";
    String broker_ip = "192.168.131.23";
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
    float getDirectorId();
    void validateDirector(DirectorState directorState);
    void charge(float power);
    void switchMode(StationModes mode);
    void notifyDashboard(String message);
    StationModes getStationMode();
};

#endif