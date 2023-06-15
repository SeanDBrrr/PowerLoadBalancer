#pragma once
#include "IPlug.h"
#include "IPLB.h"
#include "IDirector.h"
#include "IDisplay.h"
#include "IStart.h"
#include "Enums.h"
#include <iostream>
#include <exception>
#include <queue>

enum class Event; 
enum class State;

class ChargingStation
{
private:
    uint32_t _directorId;
    int _id;
    float _powerRecieved;
    bool _startButtonState = false;
    bool _plugButtonState = false;
    bool _isStartedFlag;
    bool _isPluggedFlag;
    bool _isRfidAvailable;
    std::vector<Event> _currentEvents;
    State _currentState;
    State _lastState;
    int _wifiTrials;
    int _mqttTrials;

    IPLB* _IPLB;
    IPlug* _IPlug;
    IDirector* _IDirector;
    IDisplay* _display;
    IStart* _IStart;
public:
    ChargingStation(
        int id,
        IStart *Start,
        IPlug *Plug,
        IDirector *Director,
        IDisplay *Display,
        IPLB *PLB
    );

    ~ChargingStation();

    void loop(Event ev);
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
    bool isPluggedFlag();
    void charge(float power);
    void requestPower();
};