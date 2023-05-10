#ifndef STATION_COM_HPP
#define STATION_COM_HPP

#include "MQTTServer.h"
#include "IStation.h"
#include <vector>

class StationCommunication : public IStation
{
private:
    MQTTServer *_mqtt;
    // std::vector<const char *> _topics;
    int _directorId;
    bool _receivedRequestSupply;
    bool _directorChecked;
    int _stationId;

public:
    StationCommunication(MQTTServer *mqtt);
    ~StationCommunication();

    int getDirectorId();
    int getId();
    void charge(float power);
    void switchMode(StationModes mode);
    bool isSupplyRequest();
    bool isDirectorChecked();
    void clearSupplyRequest();
    void clearDirectorCheck();
    void loop();
};

StationCommunication::StationCommunication(MQTTServer *mqtt)
{
    _topics.emplace_back("group4/StationId");
    _topics.emplace_back("group4/DirectorId");
    _topics.emplace_back("group4/RequestSupply");
    _topics.emplace_back("group4/ChangeStationMode");
}

StationCommunication::~StationCommunication()
{
}

int StationCommunication::getId() override
{
  send(mqtt_topic_StationId, )
}

int StationCommunication::getDirectorId()
{
  return _directorId;
}

bool StationCommunication::isSupplyRequest()
{
  return _receivedRequest;
}

bool StationCommunication::isDirectorChecked()
{
  return mqtt.getDirectorCheck();
}

void StationCommunication::clearSupplyRequest()
{
  _receivedRequest = false;
}

void StationCommunication::clearDirectorCheck()
{
  _directorChecked = false;
}

void StationCommunication::charge(float power)
{
  _mqtt->send("group4/chargeStation", (String)power);
}

void StationCommunication::switchMode(StationModes mode)
{
  send(mqtt_topic_mode, (String)mode);
}

int StationCommunication::loop()
{
  mqtt->receive();
  if(isDirectorChecked())
  {
    switch (mqtt.getDirectorId()) 
    {
        case 1:
        event = EV_director1;
        break;
        case 2:
        event = EV_director2;
        break;
        case 3:
        event = EV_director3;
        break;
        case 4:
        event = EV_director4;
        break;
    }
    mqtt.clearDirectorCheck();
  }
}






#endif /* STATION_COM_HPP */