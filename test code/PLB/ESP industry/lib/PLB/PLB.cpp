#include "PLB.h"

PLB::PLB(
    IBuilding *building,
    IStation *station1,
    IStation *station2,
    IStation *station3,
    IStation *station4
    )
    : _state(ST_Idle),
      _mode(MO_Auto),
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
    _state = ST_Idle;
    int solarPower;
    switch (ev)
    {
    case EV_Timeout:
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    case EV_Supply1:
        if (_directorIds.size() == 0)
        {
            _state = ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(0));
        break;
    case EV_Supply2:
        if (_directorIds.size() == 0)
        {
            _state = ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(1));
        break;
    case EV_Supply3:
        if (_directorIds.size() == 0)
        {
            _state = ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(2));
        break;
    case EV_Supply4:
        if (_directorIds.size() == 0)
        {
            _state = ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(3));
        break;
    case EV_Director1:
        _state = ST_Dir1;
        checkDirector(_stations.at(0));
        break;
    case EV_Director2:
        _state = ST_Dir1;
        checkDirector(_stations.at(1));
        break;
    case EV_Director3:
        _state = ST_Dir1;
        checkDirector(_stations.at(2));
        break;
    case EV_Director4:
        _state = ST_Dir1;
        checkDirector(_stations.at(3));
        break;
    }
    return _state;
}

PLBStates PLB::handleNoDirState(PLBEvents ev)
{
    _state = ST_NoDir;
    int solarPower;
    switch (_state)
    {
    case EV_Timeout:
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    case EV_Supply1:
        if (_directorIds.size() == 0)
        {
            _state = ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(0));
        break;
    case EV_Supply2:
        if (_directorIds.size() == 0)
        {
            _state = ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(1));
        break;
    case EV_Supply3:
        if (_directorIds.size() == 0)
        {
            _state = ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(2));
        break;
    case EV_Supply4:
        if (_directorIds.size() == 0)
        {
            _state = ST_NoDir;
        }
        _supplyPowerToStation(_stations.at(3));
        break;
    case EV_Director1:
        if (checkDirector(_stations.at(0)))
            _state = ST_Dir1;
        break;
    case EV_Director2:
        if (checkDirector(_stations.at(1)))
            _state = ST_Dir1;
        break;
    case EV_Director3:
        if (checkDirector(_stations.at(2)))
            _state = ST_Dir1;
        break;
    case EV_Director4:
        if (checkDirector(_stations.at(3)))
            _state = ST_Dir1;
        break;
    case EV_Stop1:
        _stopSupply(_stations.at(0));
        _state = (_userStations.size() == 0) ? ST_Idle : ST_NoDir;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case EV_Stop2:
        _stopSupply(_stations.at(1));
        _state = (_userStations.size() == 0) ? ST_Idle : ST_NoDir;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case EV_Stop3:
        _stopSupply(_stations.at(2));
        _state = (_userStations.size() == 0) ? ST_Idle : ST_NoDir;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case EV_Stop4:
        _stopSupply(_stations.at(3));
        _state = (_userStations.size() == 0) ? ST_Idle : ST_NoDir;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    }
    return _state;
}

PLBStates PLB::handleDir1State(PLBEvents ev)
{
    _state = ST_Dir1;
    int solarPower;
    switch (_state)
    {
    case EV_Timeout:
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    case EV_Supply1:
        _state = (_directorIds.size() == 2) ? ST_Dir2 : ST_Dir1;
        _supplyPowerToStation(_stations.at(0));
        break;
    case EV_Supply2:
        _state = (_directorIds.size() == 2) ? ST_Dir2 : ST_Dir1;
        _supplyPowerToStation(_stations.at(1));
        break;
    case EV_Supply3:
        _state = (_directorIds.size() == 2) ? ST_Dir2 : ST_Dir1;
        _supplyPowerToStation(_stations.at(2));
        break;
    case EV_Supply4:
        _state = (_directorIds.size() == 2) ? ST_Dir2 : ST_Dir1;
        _supplyPowerToStation(_stations.at(3));
        break;
    case EV_Director1:
        if (checkDirector(_stations.at(0)))
            _state = ST_Dir2;
        break;
    case EV_Director2:
        if (checkDirector(_stations.at(1)))
            _state = ST_Dir2;
        break;
    case EV_Director3:
        if (checkDirector(_stations.at(2)))
            _state = ST_Dir2;
        break;
    case EV_Director4:
        if (checkDirector(_stations.at(3)))
            _state = ST_Dir2;
        break;
    case EV_Stop1:
        _state = (_stopSupply(_stations.at(0))) ? ST_NoDir : ST_Dir1;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case EV_Stop2:
        _state = (_stopSupply(_stations.at(1))) ? ST_NoDir : ST_Dir1;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case EV_Stop3:
        _state = (_stopSupply(_stations.at(2))) ? ST_NoDir : ST_Dir1;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case EV_Stop4:
        _state = (_stopSupply(_stations.at(3))) ? ST_NoDir : ST_Dir1;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    }
    return _state;
}

PLBStates PLB::handleDir2State(PLBEvents ev)
{
    _state = ST_Dir2;
    int solarPower;
    switch (_state)
    {
    case EV_Timeout:
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    case EV_Supply1:
        _state = (_directorIds.size() == 3 && _userStations.size() == 0) ? ST_Dir3Only : ST_Dir3;
        _supplyPowerToStation(_stations.at(0));
        break;
    case EV_Supply2:
        _state = (_directorIds.size() == 3 && _userStations.size() == 0) ? ST_Dir3Only : ST_Dir3;
        _supplyPowerToStation(_stations.at(1));
        break;
    case EV_Supply3:
        _state = (_directorIds.size() == 3 && _userStations.size() == 0) ? ST_Dir3Only : ST_Dir3;
        _supplyPowerToStation(_stations.at(2));
        break;
    case EV_Supply4:
        _state = (_directorIds.size() == 3 && _userStations.size() == 0) ? ST_Dir3Only : ST_Dir3;
        _supplyPowerToStation(_stations.at(3));
        break;
    case EV_Director1:
        if (checkDirector(_stations.at(0)))
            _state = ST_Dir3;
        break;
    case EV_Director2:
        if (checkDirector(_stations.at(1)))
            _state = ST_Dir3;
        break;
    case EV_Director3:
        if (checkDirector(_stations.at(2)))
            _state = ST_Dir3;
        break;
    case EV_Director4:
        if (checkDirector(_stations.at(3)))
            _state = ST_Dir3;
        break;
    case EV_Stop1:
        _state = (_stopSupply(_stations.at(0))) ? ST_Dir1 : ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case EV_Stop2:
        _state = (_stopSupply(_stations.at(1))) ? ST_Dir1 : ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case EV_Stop3:
        _state = (_stopSupply(_stations.at(2))) ? ST_Dir1 : ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case EV_Stop4:
        _state = (_stopSupply(_stations.at(3))) ? ST_Dir1 : ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    }
    return _state;
}

PLBStates PLB::handleDir3OnlyState(PLBEvents ev)
{
    _state = ST_Dir3;
    int solarPower;
    switch (_state)
    {
    case EV_Timeout:
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
        break;
    case EV_Supply1:
        _state = (_directorStations.size() == 3) ? ST_Dir3 : ST_Dir4;
        _supplyPowerToStation(_stations.at(0));
        break;
    case EV_Supply2:
        _state = (_directorStations.size() == 3) ? ST_Dir3 : ST_Dir4;
        _supplyPowerToStation(_stations.at(1));
        break;
    case EV_Supply3:
        _state = (_directorStations.size() == 3) ? ST_Dir3 : ST_Dir4;
        _supplyPowerToStation(_stations.at(2));
        break;
    case EV_Supply4:
        _state = (_directorStations.size() == 3) ? ST_Dir3 : ST_Dir4;
        _supplyPowerToStation(_stations.at(3));
        break;
    case EV_Director1:
        if (checkDirector(_stations.at(0)))
            _state = ST_Dir4;
        break;
    case EV_Director2:
        if (checkDirector(_stations.at(1)))
            _state = ST_Dir4;
        break;
    case EV_Director3:
        if (checkDirector(_stations.at(2)))
            _state = ST_Dir4;
        break;
    case EV_Director4:
        if (checkDirector(_stations.at(3)))
            _state = ST_Dir4;
        break;
    case EV_Stop1:
        _stopSupply(_stations.at(0));
        _state = ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case EV_Stop2:
        _stopSupply(_stations.at(1));
        _state = ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case EV_Stop3:
        _stopSupply(_stations.at(2));
        _state = ST_Dir2;
        solarPower = _building->calculateSolarPower();
        _distributePower(solarPower);
        _supplyPowerToBuilding(solarPower);
    case EV_Stop4:
        _stopSupply(_stations.at(3));
        _state = ST_Dir2;
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
    case ST_Idle:
        handleIdleState(ev);
        break;
    case ST_NoDir:
        handleNoDirState(ev);
        break;
    case ST_Dir1:
        handleDir1State(ev);
        break;
    case ST_Dir2:
        handleDir2State(ev);
        break;
    case ST_Dir3:
        handleDir3State(ev);
        break;
    case ST_Dir3Only:
        handleDir3OnlyState(ev);
        break;
    case ST_Dir4:
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
    case ST_Idle:
        break;
    case ST_NoDir:
        userPower = availablePower / busyStations;
        for (size_t i = 0; i < _userStations.size(); i++)
        {
            _stations.at(_userStations.at(i))->charge(userPower);
        }
        break;
    case ST_Dir1:
        directorPower = 11;
        userPower = busyStations > 1 ? (availablePower - directorPower) / (busyStations - 1) : 0;
        _stations.at(_directorStations.at(0))->charge(directorPower);
        for (size_t i = 0; i < _userStations.size(); i++)
        {
            _stations.at(_userStations.at(i))->charge(userPower);
        }
        break;
    case ST_Dir2:
        directorPower = (availablePower >= 22) ? 11 : availablePower / 2;
        userPower = (busyStations > 2 && availablePower > 22) ? (availablePower - directorPower) / (busyStations - 2) : 0;
        for (size_t i = 0; i < _directorStations.size(); i++)
        {
            int stationId = _directorStations.at(i);
            _stations.at(stationId)->switchMode(MO_Director);
            _stations.at(stationId)->charge(directorPower);
        }
        for (size_t i = 0; i < _userStations.size(); i++)
        {
            int stationId = _directorStations.at(i);
            _stations.at(stationId)->switchMode(MO_Dynamic);
            _stations.at(stationId)->charge(userPower);
        }
        break;
    case ST_Dir3:
        stationPower = availablePower / 4;
        for (size_t i = 0; i < _stations.size(); i++)
        {
            _stations.at(i)->switchMode(MO_Dynamic);
            _stations.at(i)->charge(stationPower);
        }
        break;
    case ST_Dir3Only:
        if (availablePower > 22)
        {
            for (size_t i = 0; i < _directorStations.size(); i++)
            {
                int stationId = _directorStations.at(i);
                _stations.at(stationId)->switchMode(MO_FCFS);
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
                _stations.at(stationId)->switchMode(MO_Dynamic);
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
bool PLB::checkDirector(IStation *station)
{
    int directoId = station->getDirectorId();
    for (size_t i = 0; i < _directorIds.size(); i++)
    {
        /* return RFID state: failed(-1), checkedIn(1), notCheckedIn(0) */
        if (_directorIds.at(i) == directoId)
            return false;
    }
    _directorIds.emplace_back(directoId);
    _directorStations.emplace_back(station->getId());
    return true;
}

bool PLB::isTimeout()
{

    return false;
}

void PLB::loop()
{
    // for (const auto &s : _stations)
    // {
    //     _event = s->loop();
    //     manageEvents(_event);
    // }
    if (isTimeout())
    {
        handleEvents(EV_Timeout);
    }
}
