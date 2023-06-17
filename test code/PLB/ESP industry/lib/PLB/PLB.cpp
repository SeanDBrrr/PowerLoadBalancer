#include "PLB.h"
#include <assert.h>

/* ----------------- Constructor and Destructor */
PLB::PLB(
    IBuilding *building,
    IStation *station0,
    IStation *station1,
    IStation *station2,
    IStation *station3
    )
    : busyStations(0),
      _state(PLBStates::ST_Idle),
      _mode(PLBModes::MO_Auto),
      _stationsMode(StationModes::MO_Dynamic),
      _buildingState(BuildingState::ST_Open),
      _building(building)
{
    _stations.emplace_back(station0);
    _stations.emplace_back(station1);
    _stations.emplace_back(station2);
    _stations.emplace_back(station3);

    _validDirectorIds.emplace_back(RFID1);
    _validDirectorIds.emplace_back(RFID2);
    _validDirectorIds.emplace_back(RFID3);
    _validDirectorIds.emplace_back(RFID4);
}

PLB::~PLB() {}

/* ----------------- Private functions (Auto mode) */
void PLB::_initialiseStations()
{
    for (auto &st: _stations)
    {
        st->charge(0);
    }
}

void PLB::_changeStationsMode(StationModes stMode)
{
    _stationsMode = stMode;
    _stations.at(0)->switchMode(stMode);
}
/*
 * @brief This function is called only when a user press the start button
 * @return void
 */
void PLB::_supplyPowerToStation(IStation *station)
{
    ++busyStations;
    // _occupiedStations.emplace_back(station->getId());
    // Serial.print("_userStations.size(): "); Serial.println(_userStations.size());
    // Serial.print("busyStations: "); Serial.println(busyStations);
    // Serial.print("_directorIds.size(): "); Serial.println(_directorIds.size());
    if (busyStations > (_directorIds.size()+_userStations.size()))
    {
        _userStations.emplace_back(station->getId());
    }
}

/*
 * @brief This function sends power to the building
 * @return 0 (success) / 1 (failure)
 */
void PLB::_supplyPowerToBuilding(float solarPower)
{
    if (_buildingState == BuildingState::ST_Open) { _building->charge(80 - solarPower); }
    else if (_buildingState == BuildingState::ST_Close) { _building->charge(30 - solarPower); }
}

/*
 * @brief This function is called only when a user press the stop button
 * @return int : 1 (--director) / 0 (--user) / -1 (station was not busy)
 */
StopStatus PLB::_stopSupply(IStation *station)
{
    // for (size_t i = 0; i < _occupiedStations.size(); i++)
    // {
    //     if (station->getId() == _occupiedStations.at(i))
    //     {
    //         --busyStations;
    //         _occupiedStations.erase(_occupiedStations.cbegin() + i);
    //         break;
    //     }
    // }
    for (size_t i = 0; i < _directorStations.size(); i++)
    {
        if (station->getId() == _directorStations.at(i))
        {
            if ((_userStations.size() + _directorStations.size()) == busyStations) --busyStations;
            _directorStations.erase(_directorStations.begin() + i);
            _directorIds.erase(_directorIds.begin() + i);
            station->charge(0);
            return StopStatus::DirectorLeft;
        }
    }
    for (size_t i = 0; i < _userStations.size(); i++)
    {
        if (station->getId() == _userStations.at(i))
        {
            --busyStations;
            _userStations.erase(_userStations.begin() + i);
            Serial.print("_stopSupply USER: "); Serial.println(station->getId());
            station->charge(0);
            return StopStatus::UserLeft;
        }
    }
    if (busyStations<0) busyStations = 0;
    return StopStatus::NotOccupied;
}

/*
 * @brief This function is called when a director swipes his RFID card
 * @return DirectorState (enum class)
 */
DirectorState PLB::checkDirector(IStation *station)
{
    float directorId = station->getDirectorId();
    Serial.print("directorId: "); Serial.println(directorId);
    for (size_t i = 0; i < _directorIds.size(); i++)
    {
        if (_directorIds.at(i) == directorId)
        {
            station->validateDirector(DirectorState::ALREADY_CHECKED_IN);
            return DirectorState::ALREADY_CHECKED_IN;
        }
    }
    for(size_t i = 0; i < _validDirectorIds.size(); i++)
    {
        Serial.print("RFID: "); Serial.println(_validDirectorIds.at(i));
        if(_validDirectorIds.at(i) == directorId)
        {
            _directorIds.emplace_back(directorId);
            _directorStations.emplace_back(station->getId());
            station->validateDirector(DirectorState::VALID);
            return DirectorState::VALID;
        }
    }
    station->validateDirector(DirectorState::INVALID);
    return DirectorState::INVALID;
}

/*
 *@brief Distribute power over the 4 chargers and the building (only the calculation) 
 *@return void
*/
void PLB::_distributePower(float solarPower)
{
    static int prevBusyStations = 0;
    static float prevSolarPower = -1;
    if (solarPower != prevSolarPower || busyStations != prevBusyStations) {

        prevSolarPower = solarPower;
        prevBusyStations = busyStations;

        _supplyPowerToBuilding(solarPower);
        float availablePower;
        if (_buildingState == BuildingState::ST_Open) { availablePower = 20 + solarPower; }
        else if (_buildingState == BuildingState::ST_Close) 
        { 
            for (size_t i = 0; i < _directorStations.size(); i++)
            {
                _stations.at(_directorStations.at(i))->charge(11);
            }
            for (size_t i = 0; i < _userStations.size(); i++)
            {
                _stations.at(_userStations.at(i))->charge(11);
            }
        }
        float directorPower=0, userPower=0, stationPower=0;

        switch (_state)
        {
        case PLBStates::ST_Idle:
            break;
        case PLBStates::ST_NoDir:
            userPower = availablePower/busyStations;
            if (userPower > 11) userPower = 11;

            for (size_t i = 0; i < _userStations.size(); i++)
            {
                _stations.at(_userStations.at(i))->charge(userPower);
            }
            break;
        case PLBStates::ST_Dir1:
            userPower = (availablePower-11);
            if (_userStations.size() == 1 && userPower > 11) userPower = 11;

            /* Supply director first */
            _stations.at(_directorStations.at(0))->charge(11);

            /* Then supply users */
            for (size_t i = 0; i < _userStations.size(); i++)
            {
                _stations.at(_userStations.at(i))->charge(userPower/_userStations.size());
            }
            break;
        case PLBStates::ST_Dir2:
            directorPower = (availablePower >= 22) ? 11 : availablePower/2;
            if (_userStations.size() > 0 && availablePower > 22)
            {
                userPower = (availablePower-directorPower*_directorStations.size());
            }
            else 
            {
                userPower = 0;
            } 

            /* Supply directors first */
            for (size_t i = 0; i < _directorStations.size(); i++)
            {
                _stations.at(_directorStations.at(i))->charge(directorPower);
            }

            /* Then supply users */
            for (size_t i = 0; i < _userStations.size(); i++)
            {
                _stations.at(_userStations.at(i))->charge(userPower/_userStations.size());
            }
            break;
        case PLBStates::ST_Dir3:
            for (size_t i = 0; i < _stations.size(); i++)
            {
                _stations.at(i)->charge(availablePower/4);
            }
            break;
        case PLBStates::ST_Dir3Only:
            if (availablePower > 22)
            {
                _changeStationsMode(StationModes::MO_FCFS);
                for (size_t i = 0; i < _directorStations.size(); i++)
                {
                    if (availablePower < 11)
                    {
                        _stations.at(_directorStations.at(i))->charge(availablePower);
                    }
                    else
                    {
                        _stations.at(_directorStations.at(i))->charge(11);
                    }
                    availablePower -= 11;
                }
            }
            else
            {
                _changeStationsMode(StationModes::MO_Director);
                for (size_t i = 0; i < _directorStations.size(); i++)
                {
                    _stations.at(_directorStations.at(i))->charge(availablePower/3);
                }
            }
            break;
        case PLBStates::ST_Dir4:
            Serial.print("_distributePower() ST_Dir4: power = "); Serial.println(availablePower/4);
            for (size_t i = 0; i < _directorStations.size(); i++)
            {
                _stations.at(_directorStations.at(i))->charge(availablePower/4);
            }
            break;
        }
    }
}

/* ----------------- Public functions (Auto mode) */
void PLB::handleAutoModeEvents(PLBEvents ev)
{
    switch (_state)
    {
    case PLBStates::ST_Idle:
        handleIdleState(ev);
        break;
    case PLBStates::ST_NoDir:
        handleNoDirState(ev);
        break;
    case PLBStates::ST_Dir1:
        handleDir1State(ev);
        break;
    case PLBStates::ST_Dir2:
        handleDir2State(ev);
        break;
    case PLBStates::ST_Dir3:
        handleDir3State(ev);
        break;
    case PLBStates::ST_Dir3Only:
        handleDir3OnlyState(ev);
        break;
    case PLBStates::ST_Dir4:
        handleDir4State(ev);
        break;
    default:
        break;
    }
}

PLBStates PLB::handleIdleState(PLBEvents ev)
{
    _state = PLBStates::ST_Idle;
    float solarPower = 0;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        _distributePower(_building->calculateSolarPower());
        break;
    case PLBEvents::EV_Supply:
       if (_directorStations.size() == 0) 
       { 
            _state = PLBStates::ST_NoDir; 
            _changeStationsMode(StationModes::MO_Dynamic); 
        }
        else 
        {
            Serial.print("handleIdleState (EV_Supply): "); Serial.println(_stationIdEvents.front());
            _state = PLBStates::ST_Dir1; 
            _changeStationsMode(StationModes::MO_Director); 
        }
        _supplyPowerToStation(_stations.at(_stationIdEvents.front()));
        _distributePower(_building->getCurrentSolarPower());
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Director:
        checkDirector(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected: /* Might be redundant because none of the stations are busy */
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_SwitchMode: 
        _mode = _building->getPLBMode();
        if (_mode == PLBModes::MO_Manual) _building->notifyDashboard("PLB switched to manual mode.");
        break;
    case PLBEvents::EV_SwitchBuildingState: 
        _buildingState = _building->getState();
        _distributePower(_building->getCurrentSolarPower());
        break;
    }
    return _state;
}

PLBStates PLB::handleNoDirState(PLBEvents ev)
{
    _state = PLBStates::ST_NoDir;
    float solarPower = 0;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        _distributePower(_building->calculateSolarPower());
        break;
    case PLBEvents::EV_Supply:
        if (_directorIds.size() == 1)
        {
            _state = PLBStates::ST_Dir1; 
            _changeStationsMode(StationModes::MO_Director); 
        }
        _supplyPowerToStation(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Director:
        checkDirector(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        Serial.print("handleNoDirState EV_Stop: "); Serial.println(_stationIdEvents.front());
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        if (_userStations.size() == 0) _state = PLBStates::ST_Idle;
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        if (_userStations.size() == 0) _state = PLBStates::ST_Idle;
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_SwitchMode:
        _mode = _building->getPLBMode();
        if (_mode == PLBModes::MO_Manual) _building->notifyDashboard("PLB switched to manual mode.");
        break;
    case PLBEvents::EV_SwitchBuildingState: 
        _buildingState = _building->getState();
        _distributePower(_building->getCurrentSolarPower());
        break;
    }
    return _state;
}

PLBStates PLB::handleDir1State(PLBEvents ev)
{
    _state = PLBStates::ST_Dir1;
    float solarPower = 0;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        _distributePower(_building->calculateSolarPower());
        break;
    case PLBEvents::EV_Supply:
        Serial.print("handleDir1State (EV_Supply): "); Serial.println(_stationIdEvents.front());
        _state = (_directorIds.size() == 2) ? PLBStates::ST_Dir2 : PLBStates::ST_Dir1;
        _supplyPowerToStation(_stations.at(_stationIdEvents.front()));
        _distributePower(_building->getCurrentSolarPower());
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Director:
        Serial.print("handleDir1State EV_Director: "); Serial.println(_stationIdEvents.front());
        checkDirector(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        if (_stopSupply(_stations.at(_stationIdEvents.front())) == StopStatus::DirectorLeft)
        {
            _state = PLBStates::ST_NoDir;
            _changeStationsMode(StationModes::MO_Dynamic);
        }
        _stationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        if (_stopSupply(_stations.at(_stationIdEvents.front())) == StopStatus::DirectorLeft)
        {
            _state = PLBStates::ST_NoDir;
            _changeStationsMode(StationModes::MO_Dynamic);
        }
        _stationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_SwitchMode:
        _mode = _building->getPLBMode();
        if (_mode == PLBModes::MO_Manual) _building->notifyDashboard("PLB switched to manual mode.");
        break;
    case PLBEvents::EV_SwitchBuildingState: 
        _buildingState = _building->getState();
        _distributePower(_building->getCurrentSolarPower());
        break;
    }
    return _state;
}

PLBStates PLB::handleDir2State(PLBEvents ev)
{
    _state = PLBStates::ST_Dir2;
    float solarPower = 0;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        _distributePower(_building->calculateSolarPower());
        break;
    case PLBEvents::EV_Supply:
        if (_directorIds.size() == 3 && _userStations.size() == 0) 
        { 
            _state = PLBStates::ST_Dir3Only; 
            _changeStationsMode(StationModes::MO_Director); 
        }
        else if (_directorIds.size() == 3 && _userStations.size() == 1)
        {
            _state = PLBStates::ST_Dir3; 
            _changeStationsMode(StationModes::MO_Dynamic);
        }
        _supplyPowerToStation(_stations.at(_stationIdEvents.front()));
        _distributePower(_building->getCurrentSolarPower());
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Director:
        checkDirector(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        if (_stopSupply(_stations.at(_stationIdEvents.front())) == StopStatus::DirectorLeft) _state = PLBStates::ST_Dir1;
        _stationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        if (_stopSupply(_stations.at(_stationIdEvents.front())) == StopStatus::DirectorLeft) _state = PLBStates::ST_Dir1;
        _stationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_SwitchMode:
        _mode = _building->getPLBMode();
        if (_mode == PLBModes::MO_Manual) _building->notifyDashboard("PLB switched to manual mode.");
        break;
    case PLBEvents::EV_SwitchBuildingState: 
        _buildingState = _building->getState();
        _distributePower(_building->getCurrentSolarPower());
        break;
    }
    return _state;
}

PLBStates PLB::handleDir3OnlyState(PLBEvents ev)
{
    _state = PLBStates::ST_Dir3Only;
    float solarPower = 0;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        _distributePower(_building->calculateSolarPower());
        break;
    case PLBEvents::EV_Supply:
        if (_directorIds.size() == 3) 
        {
            Serial.print("handleDir3OnlyState EV_Supply -> ST_Dir3: "); Serial.println(_stationIdEvents.front());
            _state = PLBStates::ST_Dir3; 
            _changeStationsMode(StationModes::MO_Dynamic); 
        }
        else if (_directorIds.size() == 4)
        {
            Serial.print("handleDir3OnlyState EV_Supply -> ST_Dir4: "); Serial.println(_stationIdEvents.front());
            _state = PLBStates::ST_Dir4;
        }
        _supplyPowerToStation(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Director:
        Serial.print("handleDir3OnlyState EV_Director: "); Serial.println(_stationIdEvents.front());
        checkDirector(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        _state = PLBStates::ST_Dir2;
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        if (_stopSupply(_stations.at(_stationIdEvents.front())) == StopStatus::DirectorLeft) _state = PLBStates::ST_Dir2;
        _stationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_SwitchMode:
        _mode = _building->getPLBMode();
        if (_mode == PLBModes::MO_Manual) _building->notifyDashboard("PLB switched to manual mode.");
        break;
    case PLBEvents::EV_SwitchBuildingState: 
        _buildingState = _building->getState();
        _distributePower(_building->getCurrentSolarPower());
        break;
    }
    return _state;
}

PLBStates PLB::handleDir3State(PLBEvents ev)
{
    _state = PLBStates::ST_Dir3;
    float solarPower = 0;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        _distributePower(_building->calculateSolarPower());
        break;
    case PLBEvents::EV_Stop:
        if (_stopSupply(_stations.at(_stationIdEvents.front())) == StopStatus::DirectorLeft)
        {
            _state = PLBStates::ST_Dir2;
            _changeStationsMode(StationModes::MO_Director);
        }
        else
        {
            _state = PLBStates::ST_Dir3Only;
            _changeStationsMode(StationModes::MO_Director);
        }
        _stationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Director:
        Serial.print("handleDir3State EV_Director: "); Serial.println(_stationIdEvents.front());
        // checkDirector(_stations.at(_stationIdEvents.front()));
        // _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        if (_stopSupply(_stations.at(_stationIdEvents.front())) == StopStatus::DirectorLeft)
        {
            _state = PLBStates::ST_Dir2;
            _changeStationsMode(StationModes::MO_Director);
        }
        else
        {
            _state = PLBStates::ST_Dir3Only;
            _changeStationsMode(StationModes::MO_Director);
        }
        _stationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_SwitchMode:
        _mode = _building->getPLBMode();
        if (_mode == PLBModes::MO_Manual) _building->notifyDashboard("PLB switched to manual mode.");
        break;
    case PLBEvents::EV_SwitchBuildingState: 
        _buildingState = _building->getState();
        _distributePower(_building->getCurrentSolarPower());
        break;
    }
    return _state;
}

PLBStates PLB::handleDir4State(PLBEvents ev)
{
    _state = PLBStates::ST_Dir4;
    float solarPower = 0;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        _distributePower(_building->calculateSolarPower());
        break;
    case PLBEvents::EV_Stop:
        Serial.print("handleDir4State EV_Stop: "); Serial.println(_stationIdEvents.front());
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        _state = PLBStates::ST_Dir3Only;
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        if (_stopSupply(_stations.at(_stationIdEvents.front())) == StopStatus::DirectorLeft) _state = PLBStates::ST_Dir3Only;
        _stationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_SwitchMode:
        _mode = _building->getPLBMode();
        if (_mode == PLBModes::MO_Manual) _building->notifyDashboard("PLB switched to manual mode.");
        break;
    case PLBEvents::EV_SwitchBuildingState: 
        _buildingState = _building->getState();
        _distributePower(_building->getCurrentSolarPower());
        break;
    }
    return _state;
}

/* ----------------- Private Functions (Manual mode) */
void PLB::_switchToAutoMode()
{
    size_t directors = _directorStations.size();
    size_t users = _userStations.size();
    if (directors == 0) { _state = PLBStates::ST_NoDir; }
    else if (directors == 1) { _state = PLBStates::ST_Dir1; }
    else if (directors == 2) { _state = PLBStates::ST_Dir2; }
    else if (directors == 3 && users == 1) { _state = PLBStates::ST_Dir3; }
    else if (directors == 3 && users == 0) { _state = PLBStates::ST_Dir3Only; }
    else if (directors == 4) { _state = PLBStates::ST_Dir4; }
    else { _state = PLBStates::ST_Idle; }
}

void PLB::_supplyPowerDynamicMode(float solarPower)
{
    float availablePower = 20 + solarPower;
    float power = (availablePower/busyStations > 11) ? 11 : availablePower/busyStations;
    for (size_t i = 0; i < _directorStations.size(); i++)
    {
        _stations.at(_directorStations.at(i))->charge(power);
    }
    for (size_t i = 0; i < _userStations.size(); i++)
    {
        _stations.at(_userStations.at(i))->charge(power);
    }
}

void PLB::_supplyPowerDirectorMode(float solarPower)
{
    float availablePower = 20 + solarPower;
    float directorPower = 11*_directorStations.size();
    float userPower = availablePower-directorPower;

    /* Supply only directors dynamically */
    if (userPower <= 0)
    {
        for (size_t i = 0; i < _directorStations.size(); i++)
        {
            _stations.at(_directorStations.at(i))->charge(availablePower/_directorStations.size());
        }
    }
    /* Then supply users, if they have power left */
    else if (userPower > 0)
    {
        for (size_t i = 0; i < _directorStations.size(); i++)
        {
            _stations.at(_directorStations.at(i))->charge(availablePower/_directorStations.size());
        }
        for (size_t i = 0; i < _userStations.size(); i++)
        {
            _stations.at(_userStations.at(i))->charge(userPower/_userStations.size());
        }
    }
    
}
    
void PLB::_supplyPowerFCFSMode(float solarPower)
{
    float availablePower = 20 + solarPower;
    for (size_t i = 0; i < _occupiedStations.size(); i++)
    {
        if (availablePower > 11) 
        { 
            _stations.at(_occupiedStations.at(i))->charge(11);
            availablePower-=11;
        }
        else 
        { 
            _stations.at(_occupiedStations.at(i))->charge(availablePower);
        }   
    }
}

/* ----------------- Public Functions (Manual mode) */
void PLB::handleManualModeEvents(PLBEvents ev)
{
    switch (_stationsMode)
    {
    case StationModes::MO_Dynamic:
        handleDynamicMode(ev);
        break;
    case StationModes::MO_Director:
        handleDirectorMode(ev);
        break;
    case StationModes::MO_FCFS:
        handleFCFSMode(ev);
        break;
    default:
        break;
    }
}

void PLB::handleDynamicMode(PLBEvents ev)
{
    _stationsMode = StationModes::MO_Dynamic;
    float solarPower = 0;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        _supplyPowerDynamicMode(_building->calculateSolarPower());
        break;
    case PLBEvents::EV_Supply:
        Serial.print("handleDynamicMode EV_Supply: "); Serial.println();
        _supplyPowerToStation(_stations.at(_stationIdEvents.front()));
        _supplyPowerDynamicMode(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Director:
        checkDirector(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _supplyPowerDynamicMode(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected: /* Might be redundant because none of the stations are busy */
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _supplyPowerDynamicMode(_building->getCurrentSolarPower());
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_SwitchMode:
        _mode = _building->getPLBMode();
        if(_mode == PLBModes::MO_Auto) _switchToAutoMode();
        break;
    case PLBEvents::EV_SwitchStationMode: 
        _stationsMode = _stations.at(0)->getStationMode();
        break;
    default:
        break;
    }
}

void PLB::handleDirectorMode(PLBEvents ev)
{
    _stationsMode = StationModes::MO_Director;
    float solarPower = 0;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        _supplyPowerDirectorMode(_building->calculateSolarPower());
        break;
    case PLBEvents::EV_Supply:
        _supplyPowerToStation(_stations.at(_stationIdEvents.front()));
        _supplyPowerDirectorMode(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Director:
        checkDirector(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _supplyPowerDirectorMode(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected: /* Might be redundant because none of the stations are busy */
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _supplyPowerDirectorMode(_building->getCurrentSolarPower());
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_SwitchMode:
        _mode = _building->getPLBMode();
        if(_mode == PLBModes::MO_Auto) _switchToAutoMode();
        break;
    case PLBEvents::EV_SwitchStationMode: 
        _stationsMode = _stations.at(0)->getStationMode();
        break;
    default:
        break;
    }
}

void PLB::handleFCFSMode(PLBEvents ev)
{
    _stationsMode = StationModes::MO_FCFS;
    float solarPower = 0;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        _supplyPowerFCFSMode(_building->calculateSolarPower());
        break;
    case PLBEvents::EV_Supply:
        _supplyPowerToStation(_stations.at(_stationIdEvents.front()));
        _supplyPowerFCFSMode(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Director:
        checkDirector(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _supplyPowerFCFSMode(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected: /* Might be redundant because none of the stations are busy */
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _supplyPowerFCFSMode(_building->getCurrentSolarPower());
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_SwitchMode:
        _mode = _building->getPLBMode();
        if(_mode == PLBModes::MO_Auto) _switchToAutoMode();
        break;
    case PLBEvents::EV_SwitchStationMode:
        _stationsMode = _stations.at(0)->getStationMode();
        break;
    default:
        break;
    }
}

/* ----------------- Shared Functions (Auto & Manual mode) */
void PLB::addStation(IStation *station)
{
    _stations.emplace_back(station);
}

void PLB::setIdEvents(std::queue<int>& ids)
{
    /* using std::move() avoids unnecessary copy by tranfersing the ownership of these containers to the PLB */
    if (ids.size()>0) _stationIdEvents = std::move(ids);
}

bool PLB::isTimeout()
{
    static unsigned long _prev_millis = 0;
    if (millis() - _prev_millis > 5000) {
        _prev_millis = millis();
        return true;
    }
    return false;
}

void PLB::loop(std::vector<PLBEvents>& events, PLBEvents& PLBModeEvent)
{
    if (isTimeout()) handleAutoModeEvents(PLBEvents::EV_Timeout);

    if (_mode == PLBModes::MO_Auto) 
    {
        /* Maintainer that can switch PLB mode from the dashboard */
        if (PLBModeEvent != PLBEvents::EV_NoEvent) handleAutoModeEvents(std::move(PLBModeEvent));

        /* Iterate through the list of PLBEvents */
        for (const auto &ev: events)
        {
            if (ev==PLBEvents::EV_Supply) { Serial.println("PLB (auto) loop: EV_Supply"); }
            else if (ev==PLBEvents::EV_Stop) { Serial.println("PLB (auto) loop: EV_Stop"); }
            else if (ev==PLBEvents::EV_Director) { Serial.println("PLB (auto) loop: EV_Director"); }
            else if (ev==PLBEvents::EV_Connected) { Serial.println("PLB (auto) loop: EV_Connected"); }
            else if (ev==PLBEvents::EV_Disconnected) { Serial.println("PLB (auto) loop: EV_Disconnected"); }
            else if (ev==PLBEvents::EV_SwitchBuildingState) { Serial.println("PLB (auto) loop: EV_SwitchBuildingState"); }
            handleAutoModeEvents(ev);
        }
    }
    else if (_mode == PLBModes::MO_Manual)
    {        
        /* Maintainer that can switch PLB mode from the dashboard */
        if (PLBModeEvent != PLBEvents::EV_NoEvent) handleManualModeEvents(std::move(PLBModeEvent));

        for (const auto &ev: events)
        {
            if (ev==PLBEvents::EV_Supply) { Serial.println("PLB (manual) loop: EV_Supply"); }
            else if (ev==PLBEvents::EV_Stop) { Serial.println("PLB (manual) loop: EV_Stop"); }
            else if (ev==PLBEvents::EV_Director) { Serial.println("PLB (manual) loop: EV_Director"); }
            else if (ev==PLBEvents::EV_Connected) { Serial.println("PLB (manual) loop: EV_Connected"); }
            else if (ev==PLBEvents::EV_Disconnected) { Serial.println("PLB (manual) loop: EV_Disconnected"); }
            handleManualModeEvents(ev);
        }
    }
    /* Get rid of events once handled */
    events.clear();
}
