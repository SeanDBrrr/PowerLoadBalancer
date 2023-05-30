#include "ChargingStation.h"

State ChargingStation::HandleIdleState(Event ev)
{

    State result = State::STATE_IDLE;

    switch (ev)
    {
    case Event::EV_PLUGGED:
        _IDisplay->display("PLUGGED");
        result = State::STATE_PLUGGED;
        break;
    case Event::EV_RFID_DIRECTOR_DETECTED:
        _IDisplay->display("idl VERIFYING DIR"); // should display director status
        result = State::STATE_VERIFYING_DIRECTOR;
        break;
    case Event::EV_ERROR:
        _IDisplay->display("ERROR");
        result = State::STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

State ChargingStation::HandleVerifyingDirectorState(Event ev)
{
    State result = State::STATE_VERIFYING_DIRECTOR;

    switch (ev)
    {
    case Event::EV_RFID_VALID:
        if (_isPluggedFlag)
        {
            _IDisplay->display("valid PLUGGED DIR");
            result = State::STATE_PLUGGED_DIRECTOR;
        }
        else if (!_isPluggedFlag)
        {
            _IDisplay->display("valid IDLE_DIR");
            result = State::STATE_IDLE_DIRECTOR;
        }
        break;
    case Event::EV_RFID_INVALID:
        if (_isPluggedFlag)
        {
            _IDisplay->display("inval PLUGGED");
            result = State::STATE_PLUGGED;
        }
        else if (!_isPluggedFlag)
        {
            _IDisplay->display("inval IDLE");
            result = State::STATE_IDLE;
        }
        break;
    case Event::EV_RFID_TIMED_OUT:
        if (_isPluggedFlag)
        {
            _IDisplay->display("timed out PLUGGED");
            result = State::STATE_PLUGGED;
        }
        else if (!_isPluggedFlag)
        {
            _IDisplay->display("timed out IDLE");
            result = State::STATE_IDLE;
        }
        break;
    case Event::EV_ERROR:
        _IDisplay->display("ERROR");
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
        _IDisplay->display("PLUGGED DIR");
        result = State::STATE_PLUGGED_DIRECTOR;
        break;
    case Event::EV_ERROR:
        _IDisplay->display("ERROR");
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
        _IDisplay->display("unplug IDLE");
        result = State::STATE_IDLE;
        break;
    case Event::EV_RFID_DIRECTOR_DETECTED:
        _IDisplay->display("plg VERIFYING DIR");
        result = State::STATE_VERIFYING_DIRECTOR;
        break;
    case Event::EV_START:
        _IDisplay->display("WAITING FOR POWER");
        result = State::STATE_WAITING_FOR_POWER;
        break;
    case Event::EV_ERROR:
        _IDisplay->display("ERROR");
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
    //_IDisplay->display("Plugged Director");
    State result = State::STATE_PLUGGED_DIRECTOR;

    switch (ev)
    {
    case Event::EV_UNPLUGGED:
        _IDisplay->display("unplug IDLE DIR");
        result = State::STATE_IDLE_DIRECTOR;
        break;
    case Event::EV_RFID_INVALID:
        _IDisplay->display("rfid inval PLUGGED");
        result = State::STATE_PLUGGED;
        break;
    case Event::EV_START:
        _IDisplay->display("WAITING FOR POWER");
        result = State::STATE_WAITING_FOR_POWER;
        break;
    case Event::EV_ERROR:
        _IDisplay->display("ERROR");
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
        _IDisplay->display("CHARGING");
        result = State::STATE_CHARGING;
        break;
    case Event::EV_STOP:
        _IDisplay->display("STOPED CHARGING");
        result = State::STATE_STOPPED_CHARGING;
        break;
    case Event::EV_ERROR:
        _IDisplay->display("ERROR");
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
        _IDisplay->display("STOPED CHARGING");
        result = State::STATE_STOPPED_CHARGING;
        break;
    case Event::EV_ERROR:
        _IDisplay->display("ERROR");
        result = State::STATE_ERROR;
        break;
    case Event::EV_MODE_CHANGED_DIRECTOR:
        _IDisplay->display("Mode: Director");
        break;
    case Event::EV_MODE_CHANGED_FCFS:
        _IDisplay->display("Mode: FCFS");
        break;
    case Event::EV_MODE_CHANGED_DYNAMIC:
        _IDisplay->display("Mode: Dynamic");
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
        _IDisplay->display("WAITING FOR POWER");
        result = State::STATE_WAITING_FOR_POWER;
        break;
    case Event::EV_UNPLUGGED:
        _IDisplay->display("unplugged IDLE");
        result = State::STATE_IDLE;
        break;
    case Event::EV_ERROR:
        _IDisplay->display("ERROR");
        result = State::STATE_ERROR;
        break;
    default:
        // ignored event, nothing to do here
        break;
    }

    return result;
}

State ChargingStation::HandleErrorState(Event ev)
{
    return State::STATE_STOPPED_CHARGING;
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
    case State::STATE_VERIFYING_DIRECTOR:
        _currentState = HandleVerifyingDirectorState(ev);
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
    if (!_isStartedFlag)
    {
        if (_IStart->isStarted())
        {
            _isStartedFlag = true;
            _currentEvent = Event::EV_START;
        }
    }
    else if (_isStartedFlag)
    {
        if (_IStart->isStarted())
        {
            _isStartedFlag = false;
            _currentEvent = Event::EV_STOP;
        }
    }

    if (!_isPluggedFlag)
    {
        if (_IPlug->isPlugged())
        {
            _isPluggedFlag = true;
            _currentEvent = Event::EV_PLUGGED;
        }
    }
    else if (_isPluggedFlag)
    {
        if (_IPlug->isPlugged())
        {
            _isPluggedFlag = false;
            _currentEvent = Event::EV_UNPLUGGED;
        }
    }

    _directorId = _IDirector->getID();
    if (_directorId != 0)
    {
        _IPLB->checkDirector(_directorId);
        _currentEvent = Event::EV_RFID_DIRECTOR_DETECTED;
    }

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
    : _directorId(0),
      _id(0),
      _isStartedFlag(0),
      _busy(0),
      _currentEvent(Event::noEvent),
      _currentState(State::STATE_IDLE),
      _IPLB(PLB),
      _IPlug(Plug),
      _IStart(Start),
      _IDirector(Director),
      _IDisplay(Display)
{
    _IDisplay->display("IDLE");
}