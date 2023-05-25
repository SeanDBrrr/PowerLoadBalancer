#pragma once
#include "IPlug.h"
#include "IPLB.h"
#include "IDirector.h"
#include "IDisplay.h"
#include "IStart.h"
#include "Event.h"
#include "State.h"
#include "StationModes.h"
#include <iostream>
#include <exception>

enum class Event; 
enum class State;

class ChargingStation
{
private:
    int _id = 0;
    bool _state = 0;//Not needed anymore?
    bool _busy = 0;

    StationModes _mode;
    Event _currentEvent;
    State _currentState;

    IPLB* _IPLB;
    IPlug* _IPlug;
    IDirector* _IDirector;
    IDisplay* _IDisplay;
    IStart* _IStart;
public:
    ChargingStation(IStart *Start, IPlug *Plug, IDirector *Director, IDisplay *Display, IPLB *PLB);
    ~ChargingStation();

    void loop();
    void HandleMainEvent(Event ev);
    State HandleMainWorkingState(Event ev);
    State HandleMainErrorState(Event ev);

    void HandleEvent(Event ev);
    State HandleIdleState(Event ev);
    State HandleIdleDirectorState(Event ev);
    State HandlePluggedState(Event ev);
    State HandlePluggedDirectorState(Event ev);
    State HandleWaitingForPowerState(Event ev);
    State HandleChargingState(Event ev);
    State HandleStoppedChargingState(Event ev);
    State HandleErrorState(Event ev);
    
    void charge(float power);
    void requestPower();
    void switchMode(StationModes _mode);
};