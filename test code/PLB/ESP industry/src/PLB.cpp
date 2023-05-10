#include "PLB.h"

PLB::PLB(IBuilding *building, IStation *station) : _state{ST_Idle}, _mode{MO_Manual}, _building{building}
{
    _stations.emplace_back(station);
}

void PLB::addStation(IStation *station)
{
    _stations.emplace_back(station);
}

void PLB::manageIdleState(Events ev)
{
    _state = ST_Idle;
    switch (ev)
    {
    case EV_timeout:
        _building->getPowerProduced();
        supplyPowerToBuidling();
        break;
    case EV_supply1:
        if (_directorIds.size() == 0)
        {
            _state = ST_NoDir;
        }
        supplyPowerToStation(_stations.at(0));
        break;
    case EV_supply2:
        if (_directorIds.size() == 0)
        {
            _state = ST_NoDir;
        }
        supplyPowerToStation(_stations.at(1));
        break;
    case EV_supply3:
        if (_directorIds.size() == 0)
        {
            _state = ST_NoDir;
        }
        supplyPowerToStation(_stations.at(2));
        break;
    case EV_supply4:
        if (_directorIds.size() == 0)
        {
            _state = ST_NoDir;
        }
        supplyPowerToStation(_stations.at(3));
        break;
    case EV_director1:
        _state = ST_Dir1;
        checkDirector(_stations.at(0), 4);
        break;
    case EV_director2:
        _state = ST_Dir1;
        checkDirector(_stations.at(1), 4);
        break;
    case EV_director3:
        _state = ST_Dir1;
        checkDirector(_stations.at(2), 4);
        break;
    case EV_director4:
        _state = ST_Dir1;
        checkDirector(_stations.at(3), 4);
        break;
    }
}

void PLB::manageNoDirState(Events ev)
{
}

void PLB::manageDir1State(Events ev)
{
}

void PLB::manageDir2State(Events ev)
{
}

void PLB::manageDir3State(Events ev)
{
}

void PLB::manageDir3OnlyState(Events ev)
{
}

void PLB::manageEvents(Events ev)
{
    switch (_state)
    {
    case ST_Idle:
        manageIdleState(ev);
        break;
    case ST_NoDir:
        manageNoDirState(ev);
        break;
    case ST_Dir1:
        manageDir1State(ev);
        break;
    case ST_Dir2:
        manageDir2State(ev);
        break;
    case ST_Dir3:
        manageDir3State(ev);
        break;
    case ST_Dir3Only:
        manageDir3OnlyState(ev);
        break;
    default:
        break;
    }
}

/*
 * @brief This function is called only when a user press the start button
 * @return void
 */
void PLB::supplyPowerToStation(IStation *station)
{
    ++busyStations;
    int buidlingPower = _building->getPowerProduced();
    if (busyStations > _directorIds.size())
    {
        _userStations.emplace_back(station->getId());
    }
    _calculatePower(buidlingPower);
}

void PLB::_calculatePower(int solarPower)
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
            _stations.at(stationId)->switchMode(Director);
            _stations.at(stationId)->charge(directorPower);
        }
        for (size_t i = 0; i < _userStations.size(); i++)
        {
            int stationId = _directorStations.at(i);
            _stations.at(stationId)->switchMode(Dynamic);
            _stations.at(stationId)->charge(userPower);
        }
        break;
    case ST_Dir3:
        stationPower = availablePower / 4;
        for (size_t i = 0; i < _stations.size(); i++)
        {
            _stations.at(i)->switchMode(Dynamic);
            _stations.at(i)->charge(stationPower);
        }
        break;
    case ST_Dir3Only:
        if (availablePower > 22)
        {
            for (size_t i = 0; i < _directorStations.size(); i++)
            {
                int stationId = _directorStations.at(i);
                _stations.at(stationId)->switchMode(FCFS);
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
                _stations.at(stationId)->switchMode(Dynamic);
                _stations.at(stationId)->charge(availablePower / 3);
            }
        }
        break;
    }
    prevSolarPower = solarPower;
}

/*
 * @brief This function is called when a director swipes his RFID card
 * @return 0 (success) / 1 (failure)
 */
void PLB::supplyPowerToBuidling()
{
    _building.charge(80);
}

/*
 * @brief This function is called only when a user press the start button
 * @return void
 */
void PLB::stopSupply(IStation *station)
{
    --busyStations;
    for (size_t i = 0; i < _directorStations.size(); i++)
    {
        if (station.getId() == _directorStations.at(i))
        {
            _directorStations.erase(_directorStations.begin() + i);
            return;
        }
    }
    for (size_t i = 0; i < _userStations.size(); i++)
    {
        if (station.getId() == _userStations.at(i))
        {
            _userStations.erase(_userStations.begin() + i);
            return;
        }
    }
}

/*
 * @brief This function is called when a director swipes his RFID card
 * @return 0 (success) / 1 (failure)
 */
bool PLB::checkDirector(IStation *station, int directoId)
{
    for (size_t i = 0; i < _directorIds.size(); i++)
    {
        /* return RFID state: failed(-1), checkedIn(1), notCheckedIn(0) */
        if (_directorIds.at(i) == directoId)
            return 1;
    }
    _directorIds.emplace_back(directoId);
    _directorStations.emplace_back(station.getId());
    return 0;
}

void PLB::loop()
{
    for (const auto &s: _stations)
    {
        s->loop();
    }
    _building->loop();
    manageEvents(_event);
}
