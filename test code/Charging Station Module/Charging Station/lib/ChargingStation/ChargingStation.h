#pragma once
#include "IPlug.h"
#include "IPLB.h"
#include "IDirector.h"
#include "IDisplay.h"
#include "IStart.h"
#include "Enums.h"
#include <iostream>
#include <exception>

enum class Event; 
enum class State;

class ChargingStation
{
private:
    uint32_t _directorId;
    int _id;
    bool _isStartedFlag;
    bool _isPluggedFlag;
    bool _busy;
    Event _currentEvent;
    State _currentState;

    IPLB* _IPLB;
    IPlug* _IPlug;
    IDirector* _IDirector;
    IDisplay* _IDisplay;
    IStart* _IStart;
public:
    ChargingStation(
        IStart *Start,
        IPlug *Plug,
        IDirector *Director,
        IDisplay *Display,
        IPLB *PLB
    );

    ~ChargingStation();

    void loop();
    void HandleMainEvent(Event ev);
    State HandleMainWorkingState(Event ev);
    State HandleMainErrorState(Event ev);

    void HandleEvent(Event ev);
    State HandleIdleState(Event ev);
    State HandleVerifyingDirectorState(Event ev);
    State HandleIdleDirectorState(Event ev);
    State HandlePluggedState(Event ev);
    State HandlePluggedDirectorState(Event ev);
    State HandleWaitingForPowerState(Event ev);
    State HandleChargingState(Event ev);
    State HandleStoppedChargingState(Event ev);
    State HandleErrorState(Event ev);
    
    void charge(float power);
    void requestPower();
};