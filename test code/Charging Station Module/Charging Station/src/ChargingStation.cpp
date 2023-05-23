#include "ChargingStation.h"

States ChargingStation::HandleIdleState(Events ev)
{
    States result = STATE_IDLE;

    switch (ev)
    {
    case EV_PLUGGED:
        result = STATE_PLUGGED;
        break;
    case EV_RFID_DIRECTOR_DETECTED:
        result = STATE_IDLE_DIRECTOR;
        break;
    case EV_ERROR:
        result = STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

States ChargingStation::HandleIdleDirectorState(Events ev)
{
    States result = STATE_IDLE_DIRECTOR;

    switch (ev)
    {
    case EV_PLUGGED:
        result = STATE_PLUGGED;
        break;
    case EV_UNPLUGGED || EV_INVALID_RFID: // SHOULD WORK, OTHERWISE try only 1 |, OTHERWISE make 2 cases.
        result = STATE_IDLE;
        break;
    case EV_ERROR:
        result = STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

States ChargingStation::HandlePluggedState(Events ev)
{
    States result = STATE_PLUGGED;

    switch (ev)
    {
    case EV_UNPLUGGED:
        result = STATE_IDLE;
        break;
    case EV_RFID_DIRECTOR_DETECTED:
        result = STATE_PLUGGED_DIRECTOR;
        break;
    case EV_START:
        result = STATE_CHARGING;
        break;
    case EV_ERROR:
        result = STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

States ChargingStation::HandlePluggedDirectorState(Events ev)
{
    States result = STATE_PLUGGED_DIRECTOR;

    switch (ev)
    {
    case EV_UNPLUGGED:
        result = STATE_IDLE_DIRECTOR;
        break;
    case EV_INVALID_RFID:
        result = STATE_PLUGGED;
        break;
    case EV_START:
        result = STATE_WAITING_FOR_POWER;
        break;
    case EV_ERROR:
        result = STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

States ChargingStation::HandleWaitingForPowerState(Events ev)
{
    States result = STATE_WAITING_FOR_POWER;

    switch (ev)
    {
    case EV_CHARGING:
        result = STATE_CHARGING;
        break;
    case EV_STOP:
        result = STATE_STOPPED_CHARGING;
        break;
    case EV_ERROR:
        result = STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

States ChargingStation::HandleChargingState(Events ev)
{
    States result = STATE_CHARGING;

    switch (ev)
    {
    case EV_STOP:
        result = STATE_STOPPED_CHARGING;
        break;
    case EV_ERROR:
        result = STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

States ChargingStation::HandleStoppedChargingState(Events ev)
{
    States result = STATE_STOPPED_CHARGING;

    switch (ev)
    {
    case EV_START:
        result = STATE_WAITING_FOR_POWER;
        break;
    case EV_UNPLUGGED:
        result = STATE_IDLE;
        break;
    case EV_ERROR:
        result = STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

void ChargingStation::HandleMainEvent(Events ev) // Technically might not be needed unless there are more states
{
    switch (_currentState)
    {
    case MAIN_STATE_WORKING:
        _currentState = HandleMainWorkingState(ev);
        break;
    case MAIN_STATE_ERROR:
        _currentState = HandleMainErrorState(ev);
        break;
    default:
        throw std::runtime_error("ERROR: illegal/unhandled state with number");
        break;
    };
}

void ChargingStation::HandleEvent(Events ev)
{
    switch (_currentState)
    {
    case STATE_IDLE:
        _currentState = HandleIdleState(ev);
        break;
    case STATE_IDLE_DIRECTOR:
        _currentState = HandleIdleDirectorState(ev);
        break;
    case STATE_PLUGGED:
        _currentState = HandlePluggedState(ev);
        break;
    case STATE_PLUGGED_DIRECTOR:
        _currentState = HandlePluggedDirectorState(ev);
        break;
    case STATE_WAITING_FOR_POWER:
        _currentState = HandleWaitingForPowerState(ev);
        break;
    case STATE_CHARGING:
        _currentState = HandleChargingState(ev);
        break;
    case STATE_STOPPED_CHARGING:
        _currentState = HandleStoppedChargingState(ev);
        break;
    case STATE_ERROR:
        _currentState = HandleErrorState(ev);
        break;
    default:
        throw std::runtime_error("ERROR: illegal/unhandled state with number");
        break;
    };
}

void ChargingStation::loop()
{
    if (_IPlug->isPlugged())
    {
        _currentEvent = EV_PLUGGED;
    }

    if (_IStart->isStarted())
    {
        _currentEvent = EV_START;
    }
    else if (!_IStart->isStarted()) // Should work in theory
    {
        _currentEvent = EV_STOP;
    }

    _IDisplay->display("test");

    try
    {
        HandleEvent(_currentEvent);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
    }
}

ChargingStation::ChargingStation(IStart *Start, IPlug *Plug, IDirector *Director, IDisplay *Display, IPLB *PLB)
    : _IPLB{PLB}, _IPlug{Plug}, _IStart{Start}, _IDirector{Director}, _IDisplay{Display}
{
}

ChargingStation::~ChargingStation()
{
}