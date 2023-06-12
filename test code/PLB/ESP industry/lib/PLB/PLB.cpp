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
      _building(building)
{
    _stations.emplace_back(station0);
    _stations.emplace_back(station1);
    _stations.emplace_back(station2);
    _stations.emplace_back(station3);
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
    if (busyStations > _directorIds.size())
    {
        _userStations.emplace_back(station->getId());
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
 * @return int : 1 (--director) / 0 (--user) / -1 (station was not busy)
 */
int PLB::_stopSupply(IStation *station)
{
    --busyStations;
    if (busyStations<0) busyStations = 0;
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
        float availablePower = 20 + solarPower;
        float directorPower=0, userPower=0, stationPower=0;
        switch (_state)
        {
        case PLBStates::ST_Idle:
            break;
        case PLBStates::ST_NoDir:
            _changeStationsMode(StationModes::MO_Dynamic);
            userPower = availablePower/busyStations;
            if (userPower > 11) userPower = 11;

            for (size_t i = 0; i < _userStations.size(); i++)
            {
                _stations.at(_userStations.at(i))->charge(userPower);
            }
            break;
        case PLBStates::ST_Dir1:
            _changeStationsMode(StationModes::MO_Director);
            directorPower = 11;
            userPower = _userStations.size() > 0 ? (availablePower-directorPower)/_userStations.size() : 0;

            /* Supply director first */
            _stations.at(_directorStations.at(0))->charge(directorPower);

            /* Then supply users */
            for (size_t i = 0; i < _userStations.size(); i++)
            {
                _stations.at(_userStations.at(i))->charge(userPower);
            }
            break;
        case PLBStates::ST_Dir2:
            _changeStationsMode(StationModes::MO_Director);
            directorPower = (availablePower >= 22) ? 11 : availablePower/2;
            userPower = (_userStations.size() > 0 && availablePower > 22) ? (availablePower-directorPower)/_userStations.size() : 0;

            /* Supply directors first */
            for (size_t i = 0; i < _directorStations.size(); i++)
            {
                _stations.at(_directorStations.at(i))->charge(directorPower);
            }

            /* Then supply users */
            for (size_t i = 0; i < _userStations.size(); i++)
            {
                _stations.at(_userStations.at(i))->charge(userPower);
            }
            break;
        case PLBStates::ST_Dir3:
            _changeStationsMode(StationModes::MO_Dynamic);
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
                    _stations.at(_directorStations.at(i))->switchMode(_stationsMode);
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
            _changeStationsMode(StationModes::MO_Director);
            for (size_t i = 0; i < _directorStations.size(); i++)
            {
                _stations.at(_directorStations.at(i))->switchMode(_stationsMode);
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
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        break;
    case PLBEvents::EV_Supply:
        if (_directorIds.size() == 0) _state = PLBStates::ST_NoDir;
        _supplyPowerToStation(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Director:
        if(checkDirector(_stations.at(_stationIdEvents.front())) == 1) _state = PLBStates::ST_Dir1;
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected: /* Might be redundant because none of the stations are busy */
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _distributePower(_building->getCurrentSolarPower());
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_SwitchMode: _mode = _stations.at(0)->getPLBMode();
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
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        break;
    case PLBEvents::EV_Supply:
        if (_directorIds.size() == 1) { _state = PLBStates::ST_Dir1; }
        else { _state = PLBStates::ST_NoDir; }
        _supplyPowerToStation(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Director:
        checkDirector(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        _state = (_userStations.size() == 0) ? PLBStates::ST_Idle : PLBStates::ST_NoDir;
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _state = (_userStations.size() == 0) ? PLBStates::ST_Idle : PLBStates::ST_NoDir;
        _distributePower(_building->getCurrentSolarPower());
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_SwitchMode: _mode = _stations.at(0)->getPLBMode();
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
        _distributePower(solarPower);
        break;
    case PLBEvents::EV_Supply:
        _state = (_directorIds.size() == 2) ? PLBStates::ST_Dir2 : PLBStates::ST_Dir1;
        _supplyPowerToStation(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
        break;
    case PLBEvents::EV_Director:
        checkDirector(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        _state = (_userStations.size() == 0) ? PLBStates::ST_NoDir : PLBStates::ST_Dir1;
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        switch (_stopSupply(_stations.at(_stationIdEvents.front())))
        {
        case 1: _state = PLBStates::ST_NoDir;
            break;
        case 0: _state = PLBStates::ST_Dir1;
            break;
        default:
            break;
        }
        _distributePower(_building->getCurrentSolarPower());
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_SwitchMode: _mode = _stations.at(0)->getPLBMode();
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
        _distributePower(solarPower);
        break;
    case PLBEvents::EV_Supply:
        _state = (_directorIds.size() == 3 && _userStations.size() == 0) ? PLBStates::ST_Dir3Only : PLBStates::ST_Dir3;
        _supplyPowerToStation(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Director:
        checkDirector(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        _state = (_stopSupply(_stations.at(_stationIdEvents.front()))) ? PLBStates::ST_Dir1 : PLBStates::ST_Dir2;
        _stationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        switch (_stopSupply(_stations.at(_stationIdEvents.front())))
        {
        case 1: _state = PLBStates::ST_Dir1;
            break;
        case 0: _state = PLBStates::ST_Dir2;
            break;
        default:
            break;
        }
        _distributePower(_building->getCurrentSolarPower());
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_SwitchMode: _mode = _stations.at(0)->getPLBMode();
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
        _distributePower(solarPower);
        break;
    case PLBEvents::EV_Supply:
        _state = (_directorStations.size() == 3) ? PLBStates::ST_Dir3 : PLBStates::ST_Dir4;
        _supplyPowerToStation(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Director:
        checkDirector(_stations.at(_stationIdEvents.front()));
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Stop:
        _stopSupply(_stations.at(_stationIdEvents.front()));
        _state = PLBStates::ST_Dir2;
        _stationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        if (_stopSupply(_stations.at(_stationIdEvents.front())) == 1) _state = PLBStates::ST_Dir2;
        _distributePower(_building->getCurrentSolarPower());
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_SwitchMode: _mode = _stations.at(0)->getPLBMode();
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
        _distributePower(solarPower);
    case PLBEvents::EV_Stop:
        _state = _stopSupply(_stations.at(_stationIdEvents.front())) ? PLBStates::ST_Dir2 : PLBStates::ST_Dir3Only;
        _stationIdEvents.pop();
        _distributePower(_building->getCurrentSolarPower());
        break;
    case PLBEvents::EV_Connected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " connected.");
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_Disconnected:
        _stations.at(_stationIdEvents.front())->notifyDashboard("Station " + String(_stationIdEvents.front()) + " disconnected.");
        switch (_stopSupply(_stations.at(_stationIdEvents.front())))
        {
        case 1: _state = PLBStates::ST_Dir2;
            break;
        case 0: _state = PLBStates::ST_Dir3Only;
            break;
        default:
            break;
        }
        _distributePower(_building->getCurrentSolarPower());
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_SwitchMode: _mode = _stations.at(0)->getPLBMode();
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
        if (_stopSupply(_stations.at(_stationIdEvents.front())) == 1) _state = PLBStates::ST_Dir3Only;
        _distributePower(_building->getCurrentSolarPower());
        _stationIdEvents.pop();
        break;
    case PLBEvents::EV_SwitchMode: _mode = _stations.at(0)->getPLBMode();
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
    for (size_t i = 0; i < _directorStations.size(); i++)
    {
        _stations.at(_directorStations.at(i))->charge(availablePower/busyStations);
    }
    for (size_t i = 0; i < _userStations.size(); i++)
    {
        _stations.at(_userStations.at(i))->charge(availablePower/busyStations);
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

    /* Directors are not priviliged in this mode, one idea is to create a vector to store the order of people arrival */
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
        _mode = _stations.at(0)->getPLBMode();
        if(_mode == PLBModes::MO_Auto)
        {
            _switchToAutoMode();
        }
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
        _mode = _stations.at(0)->getPLBMode();
        if(_mode == PLBModes::MO_Auto)
        {
            _switchToAutoMode();
        }
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
        _mode = _stations.at(0)->getPLBMode();
        if(_mode == PLBModes::MO_Auto)
        {
            _switchToAutoMode();
        }
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
            Serial.print("VALID");
            station->validateDirector(DirectorState::VALID);
            return 1;
        }
    }
    station->validateDirector(DirectorState::INVALID);
    return 0;
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

void PLB::loop(std::vector<PLBEvents>& events)
{
    if (isTimeout()) handleAutoModeEvents(PLBEvents::EV_Timeout);

    if (_mode == PLBModes::MO_Auto) 
    {
        /* Iterate through the list of PLBEvents */
        for (const auto &ev: events)
        {
            if (ev==PLBEvents::EV_Supply) { Serial.println("PLB loop: EV_Supply"); }
            else if (ev==PLBEvents::EV_Stop) { Serial.println("PLB loop: EV_Stop"); }
            else if (ev==PLBEvents::EV_Director) { Serial.println("PLB loop: EV_Director"); }
            else if (ev==PLBEvents::EV_Connected) { Serial.println("PLB loop: EV_Connected"); }
            else if (ev==PLBEvents::EV_Disconnected) { Serial.println("PLB loop: EV_Disconnected"); }
            handleAutoModeEvents(ev);
        }
    }
    else if (_mode == PLBModes::MO_Manual) 
    {
        for (const auto &ev: events)
        {
            handleManualModeEvents(ev);
        }
    }
    /* Get rid of events once handled */
    events.clear();
}
