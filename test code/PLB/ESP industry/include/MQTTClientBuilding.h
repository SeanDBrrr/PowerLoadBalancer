#ifndef MQTTCLIENTBUILDING_HPP
#define MQTTCLIENTBUILDING_HPP

class MQTTClientBuilding
{
private:
    EspMQTTClient& _client;
    bool _isSolarPowerRequestFlag;
    int _totalSolarPower;

public:
    MQTTClientBuilding();
    ~MQTTClientBuilding();

    EspMQTTClient& getClient();
    void send(String topic, String message);
    void receive();
    void onConnectionSubscribe();
    inline int getSolarPower() { return _totalSolarPower; }
};

MQTTClientBuilding::MQTTClientBuilding()
{
}

MQTTClientBuilding::~MQTTClientBuilding()
{
}







#endif /* MQTTCLIENTBUILDING_HPP */