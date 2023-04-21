#ifndef MQTTSERVER_H
#define MQTTSERVER_H

#include "IPLB.h"
#include "EspMQTTClient.h"
#include <string.h>

class MQTTServer : public IPLB
{
private:
    EspMQTTClient& _client;
    IPLB& _iplb;
    const char* _name;
    const char* _password;
    const char* _mqttModule;
    const char* _mqttTopic;
    const char* _mqttTopicReceiver;
    const char* _brokerIp;
    short _brokerPort;
public:
    MQTTServer(String name, String password, String brokerIp, String mqttModule, short brokerPort) :
    _client(name, password, brokerIp, mqttModule, brokerPort), _iplb {}
    
    ~MQTTServer();
};

MQTTServer::MQTTServer(/* args */)
{
}

MQTTServer::~MQTTServer()
{
}


#endif