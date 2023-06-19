#include "ChargingStation.h"

#include <iostream>
ChargingStation::ChargingStation(
    int id,
    IStart *Start,
    IPlug *Plug,
    IDirector *Director,
    IDisplay *Display,
    IPLB *PLB)
    : _directorId(0),
      _id(id),
      _powerRecieved(0),
      _isDirPluggedFlag(false),
      _currentState(State::STATE_IDLE),
      _wifiTrials(0),
      _mqttTrials(0),
      _IPLB(PLB),
      _IPlug(Plug),
      _IStart(Start),
      _IDirector(Director),
      _display(Display)
{
    _display->display("IDLE");
}

State ChargingStation::HandleIdleState(Event ev)
{

    State state = State::STATE_IDLE;

    switch (ev)
    {
    case Event::EV_PLUGGED:
        _display->display("EV plugged");
        _isDirPluggedFlag = true;
        state = State::STATE_PLUGGED;
        break;
    case Event::EV_RFID_DIRECTOR_DETECTED:
        state = State::STATE_VERIFYING_DIRECTOR;
        break;
    case Event::EV_WIFI_TRIALS:
        _wifiTrials++;
        _display->display("Connecting WiFi:", "Attempts: " + static_cast<String>(_wifiTrials));
        break;
    case Event::EV_MQTT_TRIALS:
        _mqttTrials++;
        _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
        break;
    case Event::EV_WIFI_NOT_CONNECTED:
        _display->display("ERROR:", "WiFi Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_MQTT_NOT_CONNECTED:
        _display->display("ERROR:", "MQTT Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_WIFI_CONNECTED:
        _wifiTrials = 0;
        _mqttTrials = 0;
        _display->display("WIFI:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_MQTT_CONNECTED:
        _mqttTrials = 0;
        _display->display("MQTT:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_CONNECTED:
        _display->display("plb connected", "IDLE");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_DISCONNECTED:
        _display->display("plb disconnected", "STOPPED CHARGING");
        state = State::STATE_STOPPED_CHARGING;
        break;
    case Event::noEvent:
        // do nothing
        break;
    }

    return state;
}

State ChargingStation::HandleVerifyingDirectorState(Event ev)
{
    State state = State::STATE_VERIFYING_DIRECTOR;

    switch (ev)
    {
    case Event::EV_RFID_VALID:
        if (_isDirPluggedFlag)
        {
            _display->display("RFID valid", "PLUGGED DIRECTOR");
            state = State::STATE_PLUGGED_DIRECTOR;
        }
        else if (!_isDirPluggedFlag)
        {
            _display->display("RFID valid", "IDLE DIRECTOR");
            state = State::STATE_IDLE_DIRECTOR;
        }
        break;
    case Event::EV_RFID_INVALID:
        if (_isDirPluggedFlag)
        {
            _display->display("RFID invalid", "PLUGGED");
            state = State::STATE_PLUGGED;
        }
        else if (!_isDirPluggedFlag)
        {
            _display->display("RFID invalid", "IDLE");
            state = State::STATE_IDLE;
        }
        break;
    case Event::EV_RFID_TIMED_OUT:
        if (_isDirPluggedFlag)
        {
            _display->display("RFID timed out", "PLUGGED");
            state = State::STATE_PLUGGED;
        }
        else if (!_isDirPluggedFlag)
        {
            _display->display("RFID timed out", "IDLE");
            state = State::STATE_IDLE;
        }
        break;
    case Event::EV_RFID_ALREADY_CHECKED_IN:
        if (_isDirPluggedFlag)
        {
            _display->display("alrdy chekd in", "PLUGGED");
            state = State::STATE_PLUGGED;
        }
        else if (!_isDirPluggedFlag)
        {
            _display->display("alrdy chekd in", "IDLE");
            state = State::STATE_IDLE;
        }
        break;
        _display->display("Connecting WiFi:", "Attempts: " + static_cast<String>(_wifiTrials));
        break;
    case Event::EV_MQTT_TRIALS:
        _mqttTrials++;
        _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
        break;
    case Event::EV_WIFI_NOT_CONNECTED:
        _display->display("ERROR:", "WiFi Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_MQTT_NOT_CONNECTED:
        _display->display("ERROR:", "MQTT Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_WIFI_CONNECTED:
        _wifiTrials = 0;
        _mqttTrials = 0;
        _display->display("WIFI:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_MQTT_CONNECTED:
        _mqttTrials = 0;
        _display->display("MQTT:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_CONNECTED:
        _display->display("plb connected", "IDLE");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_DISCONNECTED:
        _display->display("plb disconnected", "STOPPED CHARGING");
        state = State::STATE_STOPPED_CHARGING;
        break;
    case Event::noEvent:
        // do nothing
        break;
    }

    return state;
}

State ChargingStation::HandleIdleDirectorState(Event ev)
{
    State state = State::STATE_IDLE_DIRECTOR;

    switch (ev)
    {
    case Event::EV_PLUGGED:
        _display->display("ev plugged", "PLUGGED DIRECTOR");
        state = State::STATE_PLUGGED_DIRECTOR;
        break;
        _display->display("Connecting WiFi:", "Attempts: " + static_cast<String>(_wifiTrials));
        break;
    case Event::EV_MQTT_TRIALS:
        _mqttTrials++;
        _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
        break;
    case Event::EV_WIFI_NOT_CONNECTED:
        _display->display("ERROR:", "WiFi Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_MQTT_NOT_CONNECTED:
        _display->display("ERROR:", "MQTT Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_WIFI_CONNECTED:
        _wifiTrials = 0;
        _mqttTrials = 0;
        _display->display("WIFI:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_MQTT_CONNECTED:
        _mqttTrials = 0;
        _display->display("MQTT:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_CONNECTED:
        _display->display("plb connected", "IDLE");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_DISCONNECTED:
        _display->display("plb disconnected", "STOPPED CHARGING");
        state = State::STATE_STOPPED_CHARGING;
        break;
    case Event::noEvent:
        // do nothing
        break;
    }

    return state;
}

State ChargingStation::HandlePluggedState(Event ev)
{

    State state = State::STATE_PLUGGED;

    switch (ev)
    {
    case Event::EV_UNPLUGGED:
        _display->display("ev unplugged", "IDLE");
        _isDirPluggedFlag = false;
        state = State::STATE_IDLE;
        break;
    case Event::EV_RFID_DIRECTOR_DETECTED:
        state = State::STATE_VERIFYING_DIRECTOR;
        break;
    case Event::EV_START:

        _display->display("ev start", "WAITING ON POWER");
        state = State::STATE_WAITING_FOR_POWER;
        _IPLB->supplyPowerToStation(_id);
        //requestPower();
        break;
    case Event::EV_MQTT_TRIALS:
        _mqttTrials++;
        _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
        break;
    case Event::EV_WIFI_NOT_CONNECTED:
        _display->display("ERROR:", "WiFi Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_MQTT_NOT_CONNECTED:
        _display->display("ERROR:", "MQTT Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_WIFI_CONNECTED:
        _wifiTrials = 0;
        _mqttTrials = 0;
        _display->display("WIFI:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_MQTT_CONNECTED:
        _mqttTrials = 0;
        _display->display("MQTT:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_CONNECTED:
        _display->display("plb connected", "IDLE");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_DISCONNECTED:
        _display->display("plb disconnected", "STOPPED CHARGING");
        state = State::STATE_STOPPED_CHARGING;
        break;
    case Event::noEvent:
        // do nothing
        break;
    }

    return state;
}

State ChargingStation::HandlePluggedDirectorState(Event ev)
{
    State state = State::STATE_PLUGGED_DIRECTOR;

    switch (ev)
    {
    case Event::EV_UNPLUGGED:
        _display->display("ev unplugged", "IDLE DIRECTOR");
        state = State::STATE_IDLE_DIRECTOR;
        break;
    case Event::EV_RFID_INVALID:
        _display->display("RFID invalid", "PLUGGED");
        _isDirPluggedFlag = true;
        state = State::STATE_PLUGGED;
        break;
    case Event::EV_START:

        _display->display("ev start", "WAITING ON POWER");
        state = State::STATE_WAITING_FOR_POWER;
        _IPLB->supplyPowerToStation(_id);
        //requestPower();
        break;
    case Event::EV_MQTT_TRIALS:
        _mqttTrials++;
        _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
        break;
    case Event::EV_WIFI_NOT_CONNECTED:
        _display->display("ERROR:", "WiFi Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_MQTT_NOT_CONNECTED:
        state = State::STATE_ERROR;
        _display->display("ERROR:", "MQTT Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_WIFI_CONNECTED:
        _wifiTrials = 0;
        _mqttTrials = 0;
        _display->display("WIFI:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_MQTT_CONNECTED:
        _mqttTrials = 0;
        _display->display("MQTT:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_CONNECTED:
        _display->display("plb connected", "IDLE");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_DISCONNECTED:
        _display->display("plb disconnected", "STOPPED CHARGING");
        state = State::STATE_STOPPED_CHARGING;
        break;
    case Event::noEvent:
        // do nothing
        break;
    }

    return state;
}

State ChargingStation::HandleWaitingForPowerState(Event ev)
{
    State state = State::STATE_WAITING_FOR_POWER;

    switch (ev)
    {
    case Event::EV_CHARGING:
        state = State::STATE_CHARGING;
        break;
    case Event::EV_STOP:
        _IPLB->stopSupplyToStation(_id);
        _display->display("ev stop", "STOPPED CHARGING");
        state = State::STATE_STOPPED_CHARGING;
        break;
    case Event::EV_MQTT_TRIALS:
        _mqttTrials++;
        _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
        break;
    case Event::EV_WIFI_NOT_CONNECTED:
        _display->display("ERROR:", "WiFi Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_MQTT_NOT_CONNECTED:
        _display->display("ERROR:", "MQTT Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_WIFI_CONNECTED:
        _wifiTrials = 0;
        _mqttTrials = 0;
        _display->display("WIFI:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_MQTT_CONNECTED:
        _mqttTrials = 0;
        _display->display("MQTT:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_CONNECTED:
        _display->display("plb connected", "IDLE");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_DISCONNECTED:
        _display->display("plb disconnected", "STOPPED CHARGING");
        state = State::STATE_STOPPED_CHARGING;
        break;
    case Event::noEvent:
        // do nothing
        break;
    }

    return state;
}

State ChargingStation::HandleChargingState(Event ev)
{
    State state = State::STATE_CHARGING;

    switch (ev)
    {
    case Event::EV_STOP:
        _IPLB->stopSupplyToStation(_id);
        _display->display("ev stop", "STOPPED CHARGING");
        state = State::STATE_STOPPED_CHARGING;
        break;
    case Event::EV_ERROR:
        _display->display("ERROR");
        state = State::STATE_ERROR;
        break;
    case Event::EV_MODE_CHANGED_DIRECTOR:
        _display->display("Mode: Director");
        break;
    case Event::EV_MODE_CHANGED_FCFS:
        _display->display("Mode: FCFS");
        break;
    case Event::EV_MODE_CHANGED_DYNAMIC:
        _display->display("Mode: Dynamic");
        break;
    case Event::EV_MQTT_TRIALS:
        _mqttTrials++;
        _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
        break;
    case Event::EV_WIFI_NOT_CONNECTED:
        _display->display("ERROR:", "WiFi Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_MQTT_NOT_CONNECTED:
        _display->display("ERROR:", "MQTT Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_WIFI_CONNECTED:
        _wifiTrials = 0;
        _mqttTrials = 0;
        _display->display("WIFI:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_MQTT_CONNECTED:
        _mqttTrials = 0;
        _display->display("MQTT:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_CONNECTED:
        _display->display("plb connected", "IDLE");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_DISCONNECTED:
        _display->display("plb disconnected", "STOPPED CHARGING");
        state = State::STATE_STOPPED_CHARGING;
        break;
    case Event::noEvent:
        // do nothing
        break;
    }

    return state;
}

State ChargingStation::HandleStoppedChargingState(Event ev)
{
    State state = State::STATE_STOPPED_CHARGING;

    switch (ev)
    {
    case Event::EV_START:

        _display->display("ev start", "WAITING POWER");
        state = State::STATE_WAITING_FOR_POWER;
        _IPLB->supplyPowerToStation(_id);
        //requestPower();
        break;
    case Event::EV_UNPLUGGED:
        _display->display("unplugged IDLE");
        _isDirPluggedFlag = false;
        state = State::STATE_IDLE;
        break;
    case Event::EV_MQTT_TRIALS:
        _mqttTrials++;
        _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
        break;
    case Event::EV_WIFI_NOT_CONNECTED:
        _display->display("ERROR:", "WiFi Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_MQTT_NOT_CONNECTED:
        _display->display("ERROR:", "MQTT Connection");
        state = State::STATE_ERROR;
        break;
    case Event::EV_WIFI_CONNECTED:
        _wifiTrials = 0;
        _mqttTrials = 0;
        _display->display("WIFI:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_MQTT_CONNECTED:
        _mqttTrials = 0;
        _display->display("MQTT:", "Connected!");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_CONNECTED:
        _display->display("plb connected", "IDLE");
        state = State::STATE_IDLE;
        break;
    case Event::EV_PLB_DISCONNECTED:
        _display->display("plb disconnected", "STOPPED CHARGING");
        state = State::STATE_STOPPED_CHARGING;
        break;
    case Event::noEvent:
        // do nothing
        break;
    }

    return state;
}

State ChargingStation::HandleErrorState(Event ev)
{
    return State::STATE_ERROR;
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

void ChargingStation::HandleEvent(Event ev) // can technically just call private variable _currentEvent, avoided for now in fear of issues.
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

void ChargingStation::requestPower() // DOES NOT WORK COMMENTED OUT FOR NOW, WOULD NEED EXTRA TESTING!!!!(PERHAPS EVENT)
{
    Serial.println("IN request power");
    _IPLB->supplyPowerToStation(_id);
    unsigned long lastTime = millis();
    bool breakLoop = false;
    while (!_IPLB->getPowerReceievedFlag()) // PLB
    {
        _IPLB->callClientLoop(); // PLB

        if (_stopCharge) // STARTBUTTON has been pressed
        {
            Serial.println("stop charge");
            if (_IStart->isStarted())
            {
                Serial.println("STOPPED CHARGING2313123");
                _display->display("ev stop", "STOPPED CHARGING");
                _currentState = State::STATE_STOPPED_CHARGING;
                _startButtonState = 0;
                _stopCharge = false;
                return;
            }
        }

        if (millis() - lastTime >= 5000)
        {
            Serial.println("Requesting power 11313");
            _IPLB->supplyPowerToStation(_id); //_IPLB
            lastTime = millis();
        }
    }
    _IPLB->SetPowerRecievedFlag(false); // PLB
    _currentEvents.emplace_back(Event::EV_CHARGING);
    return;
}

void ChargingStation::loop(Event ev)
{

    //_startButtonState = _IStart->isStarted();//Guaranteed works
    //_plugButtonState = _IPlug->isPlugged();//Guranateed works

    if (_currentState == State::STATE_STOPPED_CHARGING || _currentState == State::STATE_PLUGGED_DIRECTOR || _currentState == State::STATE_PLUGGED)
    {
        _startButtonState = _IStart->isStarted(); // test
        if (_startButtonState)
        {
            _currentEvents.emplace_back(Event::EV_START);
            _startButtonState = 0;
        }
    }

    if (_currentState == State::STATE_WAITING_FOR_POWER || _currentState == State::STATE_CHARGING)
    {
        _startButtonState = _IStart->isStarted(); // test
        if (_startButtonState)
        {

            _currentEvents.emplace_back(Event::EV_STOP);
            _startButtonState = 0;
        }
    }

    if (_currentState == State::STATE_IDLE || _currentState == State::STATE_IDLE_DIRECTOR)
    {
        _plugButtonState = _IPlug->isPlugged(); // test
        if (_plugButtonState)
        {
            _currentEvents.emplace_back(Event::EV_PLUGGED);
            _plugButtonState = 0;
        }
    }

    if ((_currentState == State::STATE_STOPPED_CHARGING || _currentState == State::STATE_PLUGGED || _currentState == State::STATE_PLUGGED_DIRECTOR))
    {
        _plugButtonState = _IPlug->isPlugged(); // test
        if (_plugButtonState)
        {
            _currentEvents.emplace_back(Event::EV_UNPLUGGED);
            _plugButtonState = 0;
        }
    }

    if (_currentState == State::STATE_IDLE || _currentState == State::STATE_PLUGGED)
    {

        _directorId = _IDirector->getID();
    }
    else
    {
        _directorId = 0;
    }

    if (_directorId != 0)
    {
        Serial.println("Read!");
        Serial.println("Read ID");
        _IPLB->checkDirector(_directorId);
        _display->display("VERIFYING DIR");
        _IPLB->directorTimeout(3000);

        _currentEvents.emplace_back(Event::EV_RFID_DIRECTOR_DETECTED);
    }

    static float lastPower = 0;
    _powerRecieved = _IPLB->getPowerReceived();
    if (_powerRecieved != lastPower)
    {
        if (_powerRecieved == 0) // test
        {
            _display->display("Charging:" + static_cast<String>(_powerRecieved)+ "kW", "STOPPED CHARGING");
            _currentEvents.emplace_back(Event::EV_STOP);
        }
        else
        {
            _display->display("Charging:" + static_cast<String>(_powerRecieved) + "kW");
            _currentEvents.emplace_back(Event::EV_CHARGING); // technically shouldnt always call this event
            // because if it receives 0 power when it stops charging, it will call EV_CHARGING for no reason.
        }
        lastPower = _powerRecieved;
    }

    _currentEvents.emplace_back(ev); // needed for the events recieved from the PLB

    try
    {
        for (const auto &currentEvent : _currentEvents)
        {
            HandleEvent(currentEvent);
        }
        _currentEvents.clear();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
    }
}