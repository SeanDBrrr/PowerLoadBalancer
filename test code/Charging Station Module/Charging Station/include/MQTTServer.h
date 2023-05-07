#ifndef MQTTSERVER_H
#define MQTTSERVER_H

#include <string>
#include "IPLB.h"
#include "EspMQTTClient.h"
#include <string.h>

using namespace std;

class MQTTServer : public IPLB
{
private:
    EspMQTTClient& _client;
    int _id;
public:
    MQTTServer();

    EspMQTTClient& getClient();
    void checkDirector(int id);
    int supplyPower();
    int stopSupply();
    void send(String topic, String message);
    void receive();
    void onConnectionEstablished();
    ~MQTTServer();
};

MQTTServer::~MQTTServer()
{
}

#endif