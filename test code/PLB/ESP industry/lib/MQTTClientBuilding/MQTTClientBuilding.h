#ifndef MQTTCLIENTBUILDING_HPP
#define MQTTCLIENTBUILDING_HPP

#include <string>
#include "IBuilding.h"
#include "EspMQTTClient.h"

using namespace std;

class MQTTClientBuilding : public IBuilding
{
private:
    bool _isSolarPowerArrivedFlag;
    float _totalSolarPower;
    bool _isConnected;
    bool _lastConnectionState;
    int _solarPowerTimeout;

    String mqtt_topic_solarPower = "group4/powerProduced";
    String mqtt_topic_calculateSolarPower= "group4/calculateSolarPower";
    String mqtt_topic_charge_building = "group4/chargeBuilding";
    String mqtt_topic_notifyDashboard = "group4/notifyDashboard";
    String name = "S21 FE J";
    String password = "yo koaster";
    String mqtt_module = "Group4-PLB-Building";
    String broker_ip = "192.168.17.132";
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
    MQTTClientBuilding();
    ~MQTTClientBuilding();

    /* MQTT related */
    EspMQTTClient& getClient();
    void send(String topic, String message);
    void receive();
    void onConnectionSubscribe();

    /* Interface functions */
    float calculateSolarPower();
    void charge(float power);
    float getCurrentSolarPower();
    void notifyDashboard(String message);
};

#endif