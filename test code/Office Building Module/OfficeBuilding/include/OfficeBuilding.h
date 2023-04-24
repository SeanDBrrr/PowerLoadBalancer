#ifndef OFFICEBUILDING_H
#define OFFICEBUILDING_H

#include "IDisplay.h"
#include "IPLB.h"
#include "ISolarPanel.h"
#include "IStateBuilding.h"
#include "LCD.h"
#include "LDR.h"
#include "MQTTServer.h"
#include "PotentioMeter.h"
#include "Events.h"
#include "States.h"

class OfficeBuilding
{
private:
    States currentState;
    IDisplay& display;
    ISolarPanel& solarpanel;
    IStateBuilding& statebuilding;
    IPLB& plb;
    int totalSolarPower;
    int currentPower;
    bool officeState;

    int calculateSolarPower();
public:
    void HandleEvent(Events ev);

    States HandleActive(Events ev);
    States HandleInActive(Events ev);
};

#endif