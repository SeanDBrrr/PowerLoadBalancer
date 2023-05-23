#include "ChargingStation.h"

State ChargingStation::HandleIdleState(Event ev)
{
    State result = State::STATE_IDLE;

    switch (ev)
    {
    case Event::EV_PLUGGED:
        result = State::STATE_PLUGGED;
        break;
    case Event::EV_RFID_DIRECTOR_DETECTED:
        result = State::STATE_IDLE_DIRECTOR;
        break;
    case Event::EV_ERROR:
        result = State::STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

State ChargingStation::HandleIdleDirectorState(Event ev)
{
    State result = State::STATE_IDLE_DIRECTOR;

    switch (ev)
    {
    case Event::EV_PLUGGED:
        result = State::STATE_PLUGGED;
        break;
    case Event::EV_UNPLUGGED || Event::EV_INVALID_RFID: // SHOULD WORK, OTHERWISE try only 1 |, OTHERWISE make 2 cases.
        result = State::STATE_IDLE;
        break;
    case Event::EV_ERROR:
        result = State::STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

State ChargingStation::HandlePluggedState(Event ev)
{
    State result = State::STATE_PLUGGED;

    switch (ev)
    {
    case Event::EV_UNPLUGGED:
        result = State::STATE_IDLE;
        break;
    case Event::EV_RFID_DIRECTOR_DETECTED:
        result = State::STATE_PLUGGED_DIRECTOR;
        break;
    case Event::EV_START:
        result = State::STATE_CHARGING;
        break;
    case Event::EV_ERROR:
        result = State::STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

State ChargingStation::HandlePluggedDirectorState(Event ev)
{
    State result = State::STATE_PLUGGED_DIRECTOR;

    switch (ev)
    {
    case Event::EV_UNPLUGGED:
        result = State::STATE_IDLE_DIRECTOR;
        break;
    case Event::EV_INVALID_RFID:
        result = State::STATE_PLUGGED;
        break;
    case Event::EV_START:
        result = State::STATE_WAITING_FOR_POWER;
        break;
    case Event::EV_ERROR:
        result = State::STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

State ChargingStation::HandleWaitingForPowerState(Event ev)
{
    State result = State::STATE_WAITING_FOR_POWER;

    switch (ev)
    {
    case Event::EV_CHARGING:
        result = State::STATE_CHARGING;
        break;
    case Event::EV_STOP:
        result = State::STATE_STOPPED_CHARGING;
        break;
    case Event::EV_ERROR:
        result = State::STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

State ChargingStation::HandleChargingState(Event ev)
{
    State result = State::STATE_CHARGING;

    switch (ev)
    {
    case Event::EV_STOP:
        result = State::STATE_STOPPED_CHARGING;
        break;
    case Event::EV_ERROR:
        result = State::STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

State ChargingStation::HandleStoppedChargingState(Event ev)
{
    State result = State::STATE_STOPPED_CHARGING;

    switch (ev)
    {
    case Event::EV_START:
        result = State::STATE_WAITING_FOR_POWER;
        break;
    case Event::EV_UNPLUGGED:
        result = State::STATE_IDLE;
        break;
    case Event::EV_ERROR:
        result = State::STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

void ChargingStation::HandleMainEvent(Event ev) // Technically might not be needed unless there are more states
{
    switch (_currentState)
    {
    case State::MAIN_STATE_WORKING:
        _currentState = HandleMainWorkingState(ev);
        break;
    case State::MAIN_STATE_ERROR:
        _currentState = HandleMainErrorState(ev);
        break;
    default:
        throw std::runtime_error("ERROR: illegal/unhandled state with number");
        break;
    }
}

void ChargingStation::HandleEvent(Event ev)
{
    switch (_currentState)
    {
    case State::STATE_IDLE:
        _currentState = HandleIdleState(ev);
        break;
    case State::STATE_IDLE_DIRECTOR:
        _currentState = HandleIdleDirectorState(ev);
        break;
    case State::STATE_PLUGGED:
        _currentState = HandlePluggedState(ev);
        break;
    case State::STATE_PLUGGED_DIRECTOR:
        _currentState = HandlePluggedDirectorState(ev);
        break;
    case State::STATE_WAITING_FOR_POWER:
        _currentState = HandleWaitingForPowerState(ev);
        break;
    case State::STATE_CHARGING:
        _currentState = HandleChargingState(ev);
        break;
    case State::STATE_STOPPED_CHARGING:
        _currentState = HandleStoppedChargingState(ev);
        break;
    case State::STATE_ERROR:
        _currentState = HandleErrorState(ev);
        break;
    default:
        throw std::runtime_error("ERROR: illegal/unhandled state with number");
        break;
    }
}

void ChargingStation::loop()
{

    if (_IPlug->isPlugged())
    {
        _currentEvent = Event::EV_PLUGGED;
    }

    if (_IStart->isStarted())
    {
        _currentEvent = Event::EV_START;
    }
    else if (!_IStart->isStarted()) // Should work in theory
    {
        _currentEvent = Event::EV_STOP;
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

ChargingStation::ChargingStation(
    IStart *Start,
    IPlug *Plug,
    IDirector *Director,
    IDisplay *Display,
    IPLB *PLB)
    : _IPLB{PLB},
      _IPlug{Plug},
      _IStart{Start},
      _IDirector{Director},
      _IDisplay{Display}
{
}

ChargingStation::~ChargingStation()
{

}