#include "PLB.h"
#include <assert.h>

PLB::PLB(
    IBuilding *building,
    IStation *station1,
    IStation *station2,
    IStation *station3,
    IStation *station4
    )
    : busyStations(0),
      _state(PLBStates::ST_Idle),
      _mode(PLBModes::MO_Auto),
      _building(building),
      _prevSolarPower(0)
{
    _stations.emplace_back(station1);
    _stations.emplace_back(station2);
    _stations.emplace_back(station3);
    _stations.emplace_back(station4);

    _initialiseStations();
    
}

void PLB::addStation(IStation *station)
{
    _stations.emplace_back(station);
}

void PLB::setEvents(std::queue<int>& supplyEvents, std::queue<int>& stopEvents, std::queue<int>& directorEvents, std::queue<int>& connectionEvents)
{
    /* using std::move() avoids unnecessary copy by tranfersing the ownership of these containers to the PLB */
    if (supplyEvents.size()>0) _supplyRequestStationIdEvents = std::move(supplyEvents);
    if (stopEvents.size()>0) _stopStationIdEvents = std::move(stopEvents);
    if (directorEvents.size()>0) _directorEvents = std::move(directorEvents);
    if (connectionEvents.size()>0) _connectionEvents = std::move(connectionEvents);
}

PLBStates PLB::handleIdleState(PLBEvents ev)
{
    _state = PLBStates::ST_Idle;
    float solarPower = 0;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        solarPower = _building->calculateSolarPower();
        #if COMMENTS
        Serial.print("_building->calculateSolarPower() = "); Serial.println(solarPower);
        #endif
        _distributePower(solarPower);
        break;
    case PLBEvents::EV_Supply:
        #if COMMENTS
        Serial.print("handleIdleState: EV_Supply -> "); Serial.println(_supplyRequestStationIdEvents.front());
        #endif
        if (_directorIds.size() == 0) _state = PLBStates::ST_NoDir;
        _supplyPowerToStation(_stations.at(_supplyRequestStationIdEvents.front()));
        _supplyRequestStationIdEvents.pop();
        break;
    case PLBEvents::EV_Director:
        if(checkDirector(_stations.at(_directorEvents.front())) == 1) _state = PLBStates::ST_Dir1;
        _directorEvents.pop();
        break;
    case PLBEvents::EV_Connected:
        #if COMMENTS
        Serial.print("_connectionEvents.front(): "); Serial.println(_connectionEvents.front());
        #endif
        _stations.at(_connectionEvents.front())->notifyDashboard("Station " + String(_connectionEvents.front()) + " connected.");
        _connectionEvents.pop();

        break;
    case PLBEvents::EV_Disconnected:
        #if COMMENTS
        Serial.print("_connectionEvents.front(): "); Serial.println(_connectionEvents.front());
        #endif
        _stations.at(_connectionEvents.front())->notifyDashboard("Station " + String(_connectionEvents.front()) + " disconnected.");
        _stopSupply(_stations.at(_connectionEvents.front()));
        _distributePower(_building->getCurrentSolarPower());
        _connectionEvents.pop();
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
        #if COMMENTS
        Serial.println("handleNoDirState: EV_Timeout");
        Serial.print("_building->calculateSolarPower() = "); Serial.println(solarPower);
        #endif
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        break;
    case PLBEvents::EV_Supply:
        #if COMMENTS
        Serial.print("handleNoDirState: EV_Supply -> "); Serial.println(_supplyRequestStationIdEvents.front());
        #endif
        if (_directorIds.size() == 0) _state = PLBStates::ST_NoDir;
        _supplyPowerToStation(_stations.at(_supplyRequestStationIdEvents.front()));
        _supplyRequestStationIdEvents.pop();
        break;
    case PLBEvents::EV_Director:
        if(checkDirector(_stations.at(_directorEvents.front())) == 1) _state = PLBStates::ST_Dir1;
        _directorEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        #if COMMENTS
        Serial.print("handleNoDirState: EV_Stop -> "); Serial.println(_stopStationIdEvents.front());
        #endif
        _stopSupply(_stations.at(_stopStationIdEvents.front()));
        _stopStationIdEvents.pop();
        _state = (_userStations.size() == 0) ? PLBStates::ST_Idle : PLBStates::ST_NoDir;
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_connectionEvents.front())->notifyDashboard("Station " + String(_connectionEvents.front()) + " connected.");
        _connectionEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        _stations.at(_connectionEvents.front())->notifyDashboard("Station " + String(_connectionEvents.front()) + " disconnected.");
        _stopSupply(_stations.at(_connectionEvents.front()));
        _distributePower(_building->getCurrentSolarPower());
        _connectionEvents.pop();
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
        solarPower = _building->calculateSolarPower();
        #if COMMENTS
        Serial.print("_building->calculateSolarPower() = "); Serial.println(solarPower);
        #endif
        _distributePower(solarPower);
        break;
    case PLBEvents::EV_Supply:
        _state = (_directorIds.size() == 2) ? PLBStates::ST_Dir2 : PLBStates::ST_Dir1;
        _supplyPowerToStation(_stations.at(_supplyRequestStationIdEvents.front()));
        _supplyRequestStationIdEvents.pop();
        break;
        break;
    case PLBEvents::EV_Director:
        if(checkDirector(_stations.at(_directorEvents.front())) == 1) _state = PLBStates::ST_Dir2;
        _directorEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        _stopSupply(_stations.at(_stopStationIdEvents.front()));
        _stopStationIdEvents.pop();
        _state = (_userStations.size() == 0) ? PLBStates::ST_NoDir : PLBStates::ST_Dir1;
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        #if COMMENTS
        Serial.print("_connectionEvents.front(): "); Serial.println(_connectionEvents.front());
        #endif
        _stations.at(_connectionEvents.front())->notifyDashboard("Station " + String(_connectionEvents.front()) + " connected.");
        _connectionEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        #if COMMENTS
        Serial.print("_connectionEvents.front(): "); Serial.println(_connectionEvents.front());
        #endif
        _stations.at(_connectionEvents.front())->notifyDashboard("Station " + String(_connectionEvents.front()) + " disconnected.");
        _stopSupply(_stations.at(_connectionEvents.front()));
        _distributePower(_building->getCurrentSolarPower());
        _connectionEvents.pop();
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
        solarPower = _building->calculateSolarPower();
        #if COMMENTS
        Serial.print("_building->calculateSolarPower() = "); Serial.println(solarPower);
        #endif
        _distributePower(solarPower);
        break;
    case PLBEvents::EV_Supply:
        _state = (_directorIds.size() == 3 && _userStations.size() == 0) ? PLBStates::ST_Dir3Only : PLBStates::ST_Dir3;
        _supplyPowerToStation(_stations.at(_supplyRequestStationIdEvents.front()));
        _supplyRequestStationIdEvents.pop();
        break;
    case PLBEvents::EV_Director:
        if(checkDirector(_stations.at(_directorEvents.front())) == 1) _state = PLBStates::ST_Dir3;
        _directorEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        _state = (_stopSupply(_stations.at(_stopStationIdEvents.front()))) ? PLBStates::ST_Dir1 : PLBStates::ST_Dir2;
        _stopStationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        #if COMMENTS
        Serial.print("_connectionEvents.front(): "); Serial.println(_connectionEvents.front());
        #endif
        _stations.at(_connectionEvents.front())->notifyDashboard("Station " + String(_connectionEvents.front()) + " connected.");
        _connectionEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        #if COMMENTS
        Serial.print("_connectionEvents.front(): "); Serial.println(_connectionEvents.front());
        #endif
        _stations.at(_connectionEvents.front())->notifyDashboard("Station " + String(_connectionEvents.front()) + " disconnected.");
        _stopSupply(_stations.at(_connectionEvents.front()));
        _distributePower(_building->getCurrentSolarPower());
        _connectionEvents.pop();
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
        solarPower = _building->calculateSolarPower();
        #if COMMENTS
        Serial.print("_building->calculateSolarPower() = "); Serial.println(solarPower);
        #endif
        _distributePower(solarPower);
        break;
    case PLBEvents::EV_Supply:
        _state = (_directorStations.size() == 3) ? PLBStates::ST_Dir3 : PLBStates::ST_Dir4;
        _supplyPowerToStation(_stations.at(_supplyRequestStationIdEvents.front()));
        _supplyRequestStationIdEvents.pop();
        break;
    case PLBEvents::EV_Director:
        if(checkDirector(_stations.at(_directorEvents.front())) == 1) _state = PLBStates::ST_Dir4;
        _directorEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        _stopSupply(_stations.at(_stopStationIdEvents.front()));
        _state = PLBStates::ST_Dir2;
        _stopStationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        #if COMMENTS
        Serial.print("_connectionEvents.front(): "); Serial.println(_connectionEvents.front());
        #endif
        _stations.at(_connectionEvents.front())->notifyDashboard("Station " + String(_connectionEvents.front()) + " connected.");
        _connectionEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        #if COMMENTS
        Serial.print("_connectionEvents.front(): "); Serial.println(_connectionEvents.front());
        #endif
        _stations.at(_connectionEvents.front())->notifyDashboard("Station " + String(_connectionEvents.front()) + " disconnected.");
        _stopSupply(_stations.at(_connectionEvents.front()));
        _distributePower(_building->getCurrentSolarPower());
        _connectionEvents.pop();
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
        solarPower = _building->calculateSolarPower();
        #if COMMENTS
        Serial.print("_building->calculateSolarPower() = "); Serial.println(solarPower);
        #endif
        _distributePower(solarPower);
    case PLBEvents::EV_Stop:
        _state = _stopSupply(_stations.at(_stopStationIdEvents.front())) ? PLBStates::ST_Dir2 : PLBStates::ST_Dir3Only;
        _stopStationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        #if COMMENTS
        Serial.print("_connectionEvents.front(): "); Serial.println(_connectionEvents.front());
        #endif
        _stations.at(_connectionEvents.front())->notifyDashboard("Station " + String(_connectionEvents.front()) + " connected.");
        _connectionEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        #if COMMENTS
        Serial.print("_connectionEvents.front(): "); Serial.println(_connectionEvents.front());
        #endif
        _stations.at(_connectionEvents.front())->notifyDashboard("Station " + String(_connectionEvents.front()) + " disconnected.");
        _stopSupply(_stations.at(_connectionEvents.front()));
        _distributePower(_building->getCurrentSolarPower());
        _connectionEvents.pop();
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
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
    case PLBEvents::EV_Stop:
        _stopSupply(_stations.at(_stopStationIdEvents.front()));
        _stopStationIdEvents.pop();
        _state = PLBStates::ST_Dir3Only;
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        #if COMMENTS
        Serial.print("_connectionEvents.front(): "); Serial.println(_connectionEvents.front());
        #endif
        _stations.at(_connectionEvents.front())->notifyDashboard("Station " + String(_connectionEvents.front()) + " connected.");
        _connectionEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        #if COMMENTS
        Serial.print("_connectionEvents.front(): "); Serial.println(_connectionEvents.front());
        #endif
        _stations.at(_connectionEvents.front())->notifyDashboard("Station " + String(_connectionEvents.front()) + " disconnected.");
        _stopSupply(_stations.at(_connectionEvents.front()));
        _distributePower(_building->getCurrentSolarPower());
        _connectionEvents.pop();
        break;
    }
    return _state;
}

void PLB::handleEvents(PLBEvents ev)
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

/*
 * @brief This function is called only when a user press the start button
 * @return void
 */
void PLB::_supplyPowerToStation(IStation *station)
{
    ++busyStations;
    #if COMMENTS
    Serial.print("_supplyPowerToStation: busyStations = ");Serial.println(busyStations);
    #endif
    if (busyStations > _directorIds.size())
    {
        _userStations.emplace_back(station->getId());
        #if COMMENTS
        Serial.print("SupplyPowerToStation(): getStationId -> "); Serial.println(station->getId());
        #endif
    }
    _distributePower(_building->getCurrentSolarPower());
}

/*
 * @brief This function sends power to the building
 * @return 0 (success) / 1 (failure)
 */
void PLB::_supplyPowerToBuilding(float solarPower)
{
    _building->charge(80 - solarPower);
}

/*
 * @brief This function is called only when a user press the stop button
 * @return int : 1 (--director) / 0 (--user) / -1 (else)
 */
int PLB::_stopSupply(IStation *station)
{
    --busyStations;
    if (busyStations<0) busyStations = 0;
    #if COMMENTS
    Serial.print("_stopSupply: busyStations = "); Serial.println(busyStations);
    #endif
    for (size_t i = 0; i < _directorStations.size(); i++)
    {
        if (station->getId() == _directorStations.at(i))
        {
            _directorStations.erase(_directorStations.begin() + i);
            _directorIds.erase(_directorIds.begin() + i);
            station->charge(0);
            return 1;
        }
    }
    for (size_t i = 0; i < _userStations.size(); i++)
    {
        if (station->getId() == _userStations.at(i))
        {
            _userStations.erase(_userStations.begin() + i);
            station->charge(0);
            return 0;
        }
    }
    return -1;
}

/*
 * @brief This function is called when a director swipes his RFID card
 * @return RFID state: failed(-1), checkedIn(1), notCheckedIn(0)
 */
int PLB::checkDirector(IStation *station)
{
    uint32_t directorId = station->getDirectorId();
    for (size_t i = 0; i < _directorIds.size(); i++)
    {
        if (_directorIds.at(i) == directorId)
            station->validateDirector(DirectorState::ALREADY_CHECKED_IN);
            return -1;
    }
    for(size_t i = 0; i < _validDirectorIds.size(); i++)
    {
        if(_validDirectorIds.at(i) == directorId)
        {
            _directorIds.emplace_back(directorId);
            _directorStations.emplace_back(station->getId());
            station->validateDirector(DirectorState::VALID);
            return 1;
        }
    }
    station->validateDirector(DirectorState::INVALID);
    return 0;
}

void PLB::_initialiseStations()
{
    for (auto &st: _stations)
    {
        st->charge(0);
    }
}

/*
 *@brief Distribute power over the 4 chargers and the building (only the calculation) 
 *@return void
*/
void PLB::_distributePower(float solarPower)
{
    static int prevBusyStations = -1;
    #if COMMENTS
    Serial.print("_distributePower: prevBusyStations = ");Serial.println(prevBusyStations);
    Serial.print("_distributePower: busyStations = ");Serial.println(busyStations);
    #endif
    if (solarPower != _prevSolarPower || busyStations != prevBusyStations) {

        _prevSolarPower = solarPower;
        if (busyStations==0) { prevBusyStations = -1; }
        else { prevBusyStations = busyStations; }

        _supplyPowerToBuilding(solarPower);
        float availablePower = 20 + solarPower;
        float directorPower=0, userPower=0, stationPower=0;
        switch (_state)
        {
        case PLBStates::ST_Idle:
            break;
        case PLBStates::ST_NoDir:
            userPower = availablePower / busyStations;
            if (userPower > 11) userPower = 11;        
            for (size_t i = 0; i < _userStations.size(); i++)
            {
                #if COMMENTS
                Serial.print("_userStations.at(i) -> ");Serial.println(_userStations.at(i));
                #endif
                _stations.at(_userStations.at(i))->charge(userPower);
            }
            break;
        case PLBStates::ST_Dir1:
            directorPower = 11;
            userPower = busyStations > 1 ? (availablePower - directorPower) / (busyStations - 1) : 0;
            _stations.at(_directorStations.at(0))->charge(directorPower);
            for (size_t i = 0; i < _userStations.size(); i++)
            {
                _stations.at(_userStations.at(i))->charge(userPower);
            }
            break;
        case PLBStates::ST_Dir2:
            directorPower = (availablePower >= 22) ? 11 : availablePower / 2;
            userPower = (busyStations > 2 && availablePower > 22) ? (availablePower - directorPower) / (busyStations - 2) : 0;
            for (size_t i = 0; i < _directorStations.size(); i++)
            {
                int stationId = _directorStations.at(i);
                _stations.at(stationId)->switchMode(StationModes::MO_Director);
                _stations.at(stationId)->charge(directorPower);
            }
            for (size_t i = 0; i < _userStations.size(); i++)
            {
                int stationId = _directorStations.at(i);
                _stations.at(stationId)->switchMode(StationModes::MO_Dynamic);
                _stations.at(stationId)->charge(userPower);
            }
            break;
        case PLBStates::ST_Dir3:
            stationPower = availablePower / 4;
            for (size_t i = 0; i < _stations.size(); i++)
            {
                _stations.at(i)->switchMode(StationModes::MO_Dynamic);
                _stations.at(i)->charge(stationPower);
            }
            break;
        case PLBStates::ST_Dir3Only:
            if (availablePower > 22)
            {
                for (size_t i = 0; i < _directorStations.size(); i++)
                {
                    int stationId = _directorStations.at(i);
                    _stations.at(stationId)->switchMode(StationModes::MO_FCFS);
                    availablePower -= 11;
                    if (availablePower < 11)
                    {
                        _stations.at(stationId)->charge(availablePower);
                    }
                    else
                    {
                        _stations.at(stationId)->charge(11);
                    }
                }
            }
            else
            {
                for (size_t i = 0; i < _directorStations.size(); i++)
                {
                    int stationId = _directorStations.at(i);
                    _stations.at(stationId)->switchMode(StationModes::MO_Dynamic);
                    _stations.at(stationId)->charge(availablePower / 3);
                }
            }
            break;
        }
    }
}

/* ----------------- Functions (Manual Mode) */

float PLB::_supplyDirectors(float availablePower)
{
    for (size_t i = 0; i < _directorStations.size(); i++)
    {
        
    }
    
    return 0.0f;
}

float PLB::_supplyUsers(float availablePower)
{
    return 0.0f;
}

void PLB::handleDynamicMode()
{
    float availablePower = 20 + _building->getCurrentSolarPower();
    for (size_t i = 0; i < _directorStations.size(); i++)
    {
        _stations.at(_directorStations.at(i))->charge(availablePower/busyStations);
    }
    for (size_t i = 0; i < _userStations.size(); i++)
    {
        _stations.at(_userStations.at(i))->charge(availablePower/busyStations);
    }
}

void PLB::handleDirectorMode()
{
    /* supply directors first (FCFS among directors) and users afterwards (dynamic) */
    float availablePower = 20 + _building->getCurrentSolarPower();
    float powerLeftover = _supplyDirectors(availablePower);
    _supplyUsers(powerLeftover);
}

void PLB::handleFCFSMode()
{
    /* supply people regarding the time they arrived */
}

void PLB::handleManualMode(StationModes mo)
{
    switch (mo)
    {
    case StationModes::MO_Dynamic:
        handleDynamicMode();
        break;
    case StationModes::MO_Director:
        handleDirectorMode();
        break;
    case StationModes::MO_FCFS:
        handleFCFSMode();
        break;
    
    default:
        break;
    }
}

bool PLB::isTimeout()
{
    static unsigned long _prev_millis = 0;
    if (millis() - _prev_millis > 10000) {
        _prev_millis = millis();
        return true;
    }
    return false;
}

void PLB::loop(std::vector<PLBEvents>& events)
{
    /* Iterate through the list of PLBEvents */
    for (const auto &ev: events)
    {
        #if COMMENTS
        if (ev==PLBEvents::EV_Supply) { Serial.println("PLB loop: EV_Supply"); }
        else if (ev==PLBEvents::EV_Stop) { Serial.println("PLB loop: EV_Stop"); }
        else if (ev==PLBEvents::EV_Director) { Serial.println("PLB loop: EV_Director"); }
        else if (ev==PLBEvents::EV_Connected) { Serial.println("PLB loop: EV_Connected"); }
        else if (ev==PLBEvents::EV_Disconnected) { Serial.println("PLB loop: EV_Disconnected"); }
        #endif
        handleEvents(ev);
    }
    /* Get rid of events once handled */
    events.clear();
    if (isTimeout())
    {
        handleEvents(PLBEvents::EV_Timeout);
    }
}
