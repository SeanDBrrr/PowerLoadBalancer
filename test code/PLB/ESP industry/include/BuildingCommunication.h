#ifndef BUILDING_COM_HPP
#define BUILDING_COM_HPP

#include "MQTTClientBuilding.h"
#include "IStation.h"
#include <vector>

class BuildingCommunication : public IBuilding
{
private:
    MQTTClientBuilding *_mqtt;
    // std::vector<const char *> topics;
    int _powerProduced;
    bool _buildingState;

public:
    BuildingCommunication(MQTTServer *mqtt);
    ~BuildingCommunication();

    int calculateSolarPower();
    void charge(float power);
    void loop();
};

BuildingCommunication::BuildingCommunication(MQTTServer *mqtt)
{
    topics.emplace_back("group4/powerProduced");
    topics.emplace_back("group4/buildingState");
}

BuildingCommunication::~BuildingCommunication()
{
}

int BuildingCommunication::calculateSolarPower()
{
  return mqtt->getSolarPower();
}

void BuildingCommunication::charge(float power)
{
  _mqtt->send("group4/chargeBuilding", (String)power);
}









#endif /* BUILDING_COM_HPP */