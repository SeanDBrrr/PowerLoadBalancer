#pragma once
#include "plug.h"
#include "Events.h"
#include "States.h"

class ChargingStation
{
private:
    States currentState;
public:
    ChargingStation(IStart, Iplug, IDirector, IDisplay, IPLB);
    ~ChargingStation();

    void HandleMainEvent(Events ev);
    States HandleMainWorkingState(Events ev);
    States HandleMainErrorState(Events ev);

    void HandleEvent(Events ev);
    States HandleIdleState(Events ev);
    States HandleIdleDirectorState(Events ev);
    States HandlePluggedState(Events ev);
    States HandlePluggedDirectorState(Events ev);
    States HandleWaitingForPowerState(Events ev);
    States HandleChargingState(Events ev);
    States HandleDonePluggedState(Events ev);
    States HandleErrorState(Events ev);
};

ChargingStation::ChargingStation(/* args */)
{
}

ChargingStation::~ChargingStation()
{
}
