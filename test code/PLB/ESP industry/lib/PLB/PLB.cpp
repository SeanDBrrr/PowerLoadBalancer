#include "PLB.h"

PLB::PLB(
    IBuilding *building,
    IStation *station1,
    IStation *station2,
    IStation *station3,
    IStation *station4
    )
    : _state(PLBStates::ST_Idle),
      _mode(PLBModes::MO_Auto),
      _building(building)
{
    _stations.emplace_back(station1);
    _stations.emplace_back(station2);
    _stations.emplace_back(station3);
    _stations.emplace_back(station4);
}

void PLB::addStation(IStation *station)
{
    _stations.emplace_back(station);
}

PLBStates PLB::handleIdleState(PLBEvents ev)
{
    _state = PLBStates::ST_Idle;
    int solarPower;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    case PLBEvents::EV_Supply1:
        if (_directorIds.size() == 0)
        {
            _state = PLBStates::ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(0));
        break;
    case PLBEvents::EV_Supply2:
        if (_directorIds.size() == 0)
        {
            _state = PLBStates::ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(1));
        break;
    case PLBEvents::EV_Supply3:
        if (_directorIds.size() == 0)
        {
            _state = PLBStates::ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(2));
        break;
    case PLBEvents::EV_Supply4:
        if (_directorIds.size() == 0)
        {
            _state = PLBStates::ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(3));
        break;
    case PLBEvents::EV_Director1:
        _state = PLBStates::ST_Dir1;
        checkDirector(_stations.at(0));
        break;
    case PLBEvents::EV_Director2:
        _state = PLBStates::ST_Dir1;
        checkDirector(_stations.at(1));
        break;
    case PLBEvents::EV_Director3:
        _state = PLBStates::ST_Dir1;
        checkDirector(_stations.at(2));
        break;
    case PLBEvents::EV_Director4:
        _state = PLBStates::ST_Dir1;
        checkDirector(_stations.at(3));
        break;
    }
    return _state;
}

PLBStates PLB::handleNoDirState(PLBEvents ev)
{
    _state = PLBStates::ST_NoDir;
    int solarPower;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    case PLBEvents::EV_Supply1:
        if (_directorIds.size() == 0)
        {
            _state = PLBStates::ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(0));
        break;
    case PLBEvents::EV_Supply2:
        if (_directorIds.size() == 0)
        {
            _state = PLBStates::ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(1));
        break;
    case PLBEvents::EV_Supply3:
        if (_directorIds.size() == 0)
        {
            _state = PLBStates::ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(2));
        break;
    case PLBEvents::EV_Supply4:
        if (_directorIds.size() == 0)
        {
            _state = PLBStates::ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(3));
        break;
    case PLBEvents::EV_Director1:
        if (checkDirector(_stations.at(0)))
            _state = PLBStates::ST_Dir1;
        break;
    case PLBEvents::EV_Director2:
        if (checkDirector(_stations.at(1)))
            _state = PLBStates::ST_Dir1;
        break;
    case PLBEvents::EV_Director3:
        if (checkDirector(_stations.at(2)))
            _state = PLBStates::ST_Dir1;
        break;
    case PLBEvents::EV_Director4:
        if (checkDirector(_stations.at(3)))
            _state = PLBStates::ST_Dir1;
        break;
    case PLBEvents::EV_Stop1:
        _stopSupply(_stations.at(0));
        _state = (_userStations.size() == 0) ? PLBStates::ST_Idle : PLBStates::ST_NoDir;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case PLBEvents::EV_Stop2:
        _stopSupply(_stations.at(1));
        _state = (_userStations.size() == 0) ? PLBStates::ST_Idle : PLBStates::ST_NoDir;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case PLBEvents::EV_Stop3:
        _stopSupply(_stations.at(2));
        _state = (_userStations.size() == 0) ? PLBStates::ST_Idle : PLBStates::ST_NoDir;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case PLBEvents::EV_Stop4:
        _stopSupply(_stations.at(3));
        _state = (_userStations.size() == 0) ? PLBStates::ST_Idle : PLBStates::ST_NoDir;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    }
    return _state;
}

PLBStates PLB::handleDir1State(PLBEvents ev)
{
    _state = PLBStates::ST_Dir1;
    int solarPower;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    case PLBEvents::EV_Supply1:
        _state = (_directorIds.size() == 2) ? PLBStates::ST_Dir2 : PLBStates::ST_Dir1;
        _supplyPowerToStation(_stations.at(0));
        break;
    case PLBEvents::EV_Supply2:
        _state = (_directorIds.size() == 2) ? PLBStates::ST_Dir2 : PLBStates::ST_Dir1;
        _supplyPowerToStation(_stations.at(1));
        break;
    case PLBEvents::EV_Supply3:
        _state = (_directorIds.size() == 2) ? PLBStates::ST_Dir2 : PLBStates::ST_Dir1;
        _supplyPowerToStation(_stations.at(2));
        break;
    case PLBEvents::EV_Supply4:
        _state = (_directorIds.size() == 2) ? PLBStates::ST_Dir2 : PLBStates::ST_Dir1;
        _supplyPowerToStation(_stations.at(3));
        break;
    case PLBEvents::EV_Director1:
        if (checkDirector(_stations.at(0)))
            _state = PLBStates::ST_Dir2;
        break;
    case PLBEvents::EV_Director2:
        if (checkDirector(_stations.at(1)))
            _state = PLBStates::ST_Dir2;
        break;
    case PLBEvents::EV_Director3:
        if (checkDirector(_stations.at(2)))
            _state = PLBStates::ST_Dir2;
        break;
    case PLBEvents::EV_Director4:
        if (checkDirector(_stations.at(3)))
            _state = PLBStates::ST_Dir2;
        break;
    case PLBEvents::EV_Stop1:
        _state = (_stopSupply(_stations.at(0))) ? PLBStates::ST_NoDir : PLBStates::ST_Dir1;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case PLBEvents::EV_Stop2:
        _state = (_stopSupply(_stations.at(1))) ? PLBStates::ST_NoDir : PLBStates::ST_Dir1;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case PLBEvents::EV_Stop3:
        _state = (_stopSupply(_stations.at(2))) ? PLBStates::ST_NoDir : PLBStates::ST_Dir1;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case PLBEvents::EV_Stop4:
        _state = (_stopSupply(_stations.at(3))) ? PLBStates::ST_NoDir : PLBStates::ST_Dir1;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    }
    return _state;
}

PLBStates PLB::handleDir2State(PLBEvents ev)
{
    _state = PLBStates::ST_Dir2;
    int solarPower;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    case PLBEvents::EV_Supply1:
        _state = (_directorIds.size() == 3 && _userStations.size() == 0) ? PLBStates::ST_Dir3Only : PLBStates::ST_Dir3;
        _supplyPowerToStation(_stations.at(0));
        break;
    case PLBEvents::EV_Supply2:
        _state = (_directorIds.size() == 3 && _userStations.size() == 0) ? PLBStates::ST_Dir3Only : PLBStates::ST_Dir3;
        _supplyPowerToStation(_stations.at(1));
        break;
    case PLBEvents::EV_Supply3:
        _state = (_directorIds.size() == 3 && _userStations.size() == 0) ? PLBStates::ST_Dir3Only : PLBStates::ST_Dir3;
        _supplyPowerToStation(_stations.at(2));
        break;
    case PLBEvents::EV_Supply4:
        _state = (_directorIds.size() == 3 && _userStations.size() == 0) ? PLBStates::ST_Dir3Only : PLBStates::ST_Dir3;
        _supplyPowerToStation(_stations.at(3));
        break;
    case PLBEvents::EV_Director1:
        if (checkDirector(_stations.at(0)))
            _state = PLBStates::ST_Dir3;
        break;
    case PLBEvents::EV_Director2:
        if (checkDirector(_stations.at(1)))
            _state = PLBStates::ST_Dir3;
        break;
    case PLBEvents::EV_Director3:
        if (checkDirector(_stations.at(2)))
            _state = PLBStates::ST_Dir3;
        break;
    case PLBEvents::EV_Director4:
        if (checkDirector(_stations.at(3)))
            _state = PLBStates::ST_Dir3;
        break;
    case PLBEvents::EV_Stop1:
        _state = (_stopSupply(_stations.at(0))) ? PLBStates::ST_Dir1 : PLBStates::ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case PLBEvents::EV_Stop2:
        _state = (_stopSupply(_stations.at(1))) ? PLBStates::ST_Dir1 : PLBStates::ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case PLBEvents::EV_Stop3:
        _state = (_stopSupply(_stations.at(2))) ? PLBStates::ST_Dir1 : PLBStates::ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case PLBEvents::EV_Stop4:
        _state = (_stopSupply(_stations.at(3))) ? PLBStates::ST_Dir1 : PLBStates::ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    }
    return _state;
}

PLBStates PLB::handleDir3OnlyState(PLBEvents ev)
{
    _state = PLBStates::ST_Dir3;
    int solarPower;
    switch (ev)
    {
    case PLBEvents::EV_Timeout:
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    case PLBEvents::EV_Supply1:
        _state = (_directorStations.size() == 3) ? PLBStates::ST_Dir3 : PLBStates::ST_Dir4;
        _supplyPowerToStation(_stations.at(0));
        break;
    case PLBEvents::EV_Supply2:
        _state = (_directorStations.size() == 3) ? PLBStates::ST_Dir3 : PLBStates::ST_Dir4;
        _supplyPowerToStation(_stations.at(1));
        break;
    case PLBEvents::EV_Supply3:
        _state = (_directorStations.size() == 3) ? PLBStates::ST_Dir3 : PLBStates::ST_Dir4;
        _supplyPowerToStation(_stations.at(2));
        break;
    case PLBEvents::EV_Supply4:
        _state = (_directorStations.size() == 3) ? PLBStates::ST_Dir3 : PLBStates::ST_Dir4;
        _supplyPowerToStation(_stations.at(3));
        break;
    case PLBEvents::EV_Director1:
        if (checkDirector(_stations.at(0)) == 1)
            _state = PLBStates::ST_Dir4;
        break;
    case PLBEvents::EV_Director2:
        if (checkDirector(_stations.at(1)))
            _state = PLBStates::ST_Dir4;
        break;
    case PLBEvents::EV_Director3:
        if (checkDirector(_stations.at(2)))
            _state = PLBStates::ST_Dir4;
        break;
    case PLBEvents::EV_Director4:
        if (checkDirector(_stations.at(3)))
            _state = PLBStates::ST_Dir4;
        break;
    case PLBEvents::EV_Stop1:
        _stopSupply(_stations.at(0));
        _state = PLBStates::ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case PLBEvents::EV_Stop2:
        _stopSupply(_stations.at(1));
        _state = PLBStates::ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case PLBEvents::EV_Stop3:
        _stopSupply(_stations.at(2));
        _state = PLBStates::ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case PLBEvents::EV_Stop4:
        _stopSupply(_stations.at(3));
        _state = PLBStates::ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    }
    return _state;
}

PLBStates PLB::handleDir3State(PLBEvents ev)
{
    return _state;
}

PLBStates PLB::handleDir4State(PLBEvents ev)
{
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
    int solarPower = _building->calculateSolarPower();
    if (solarPower == -1)
    {
        // handle timeout error
    }
    if (busyStations > _directorIds.size())
    {
        _userStations.emplace_back(station->getId());
    }
    _distributePower(solarPower);
}

void PLB::_distributePower(int solarPower)
{
    static int prevSolarPower = 0;
    if (solarPower == prevSolarPower)
        return;
    int availablePower = 20 + solarPower;
    float directorPower, userPower, stationPower;
    switch (_state)
    {
    case PLBStates::ST_Idle:
        break;
    case PLBStates::ST_NoDir:
        userPower = availablePower / busyStations;
        for (size_t i = 0; i < _userStations.size(); i++)
        {
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
    prevSolarPower = solarPower;
}

/*
 * @brief This function sends power to the building
 * @return 0 (success) / 1 (failure)
 */
void PLB::_supplyPowerToBuilding(int solarPower)
{
    _building->charge(80 - solarPower);
}

/*
 * @brief This function is called only when a user press the stop button
 * @return int : 1 (--director) / 2 (--user)
 */
int PLB::_stopSupply(IStation *station)
{
    --busyStations;
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
            return 2;
        }
    }
    return 0;
}

/*
 * @brief This function is called when a director swipes his RFID card
 * @return true (success) / false (failure)
 */
int PLB::checkDirector(IStation *station)
{
    int directorId = station->getDirectorId();
    for (size_t i = 0; i < _directorIds.size(); i++)
    {
        /* return RFID state: failed(-1), checkedIn(1), notCheckedIn(0) */
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

bool PLB::isTimeout()
{
    static unsigned long prev_millis = 0;
    if (millis() - prev_millis > 10000) {
        return true;
        prev_millis = millis();
    }
    return false;
}

void PLB::loop()
{
    if (isTimeout())
    {
        int solarPower = _building->calculateSolarPower();
        if (solarPower == -1)
        {
            // handle solar request timeout error
            // maybe display sth on the Building screen
        }
        _distributePower(solarPower);
    }
}
