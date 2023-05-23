#pragma once
#include "IPlug.h"
#include "IPLB.h"
#include "IDirector.h"
#include "IDisplay.h"
#include "IStart.h"
#include "Events.h"
#include "States.h"
#include "StationModes.h"
#include <iostream>
#include <exception>

class ChargingStation
{
private:
    int _id = 0;
    bool _state = 0;//Not needed anymore?
    bool _busy = 0;

    StationModes _mode;
    Events _currentEvent;
    States _currentState;

    IPLB* _IPLB;
    IPlug* _IPlug;
    IDirector* _IDirector;
    IDisplay* _IDisplay;
    IStart* _IStart;
public:
    ChargingStation(IStart *Start, IPlug *Plug, IDirector *Director, IDisplay *Display, IPLB *PLB){};
    ~ChargingStation(){};

    void loop();
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
    
    void charge(float power);
    void requestPower();
    void switchMode(StationModes _mode);
};