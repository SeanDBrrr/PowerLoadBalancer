#pragma once
#include "IPlug.h"
#include "IPLB.h"
#include "IDirector.h"
#include "IDisplay.h"
#include "IStart.h"
#include "Events.h"
#include "States.h"

class ChargingStation
{
private:
    States currentState;
    IPLB& _IPLB;
    IPlug& _IPlug;
    IDirector& _IDirector;
    IDisplay& _IDisplay;
    IStart& _IStart;
public:
    ChargingStation(IStart& IStart, IPlug& IPlug, IDirector& IDirector, IDisplay& IDisplay, IPLB& IPLB);
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
    States HandleStoppedChargingState(Events ev);
    States HandleErrorState(Events ev);
};

ChargingStation::ChargingStation(IStart& IStart, IPlug& IPlug, IDirector& IDirector, IDisplay& IDisplay, IPLB& IPLB): _IPLB{IPLB}, _IPlug{IPlug}, _IStart{IStart}, _IDirector{IDirector}, _IDisplay{IDisplay}
{
}

ChargingStation::~ChargingStation()
{
}
