#include "PLB.h"

PLB::PLB(IBuilding *building, IStation *station1, IStation *station2, IStation *station3, IStation *station4) : 
    _state{ST_Idle}, _mode{MO_Manual}, _event{NoEvent}
{
    _building = building;
    _stations.emplace_back(station1);
    _stations.emplace_back(station2);
    _stations.emplace_back(station3);
    _stations.emplace_back(station4);
}

void PLB::addStation(IStation *station)
{
    _stations.emplace_back(station);
}

/*
 * @brief 
 * This function handles event while PLB is in 
*/
PLBStates PLB::manageIdleState()
{
    _state = ST_Idle;
    switch (_event)
    {
    case EV_timeout:
        _distributePower(_building->calculateSolarPower());
        supplyPowerToBuidling(_building->calculateSolarPower());
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
    return _state;
}

PLBStates PLB::manageNoDirState()
{
    return _state;
}

PLBStates PLB::manageDir1State()
{
    return _state;
}

PLBStates PLB::manageDir2State()
{
    return _state;
}

PLBStates PLB::manageDir3State()
{
    return _state;
}

PLBStates PLB::manageDir3OnlyState()
{
    return _state;
}

void PLB::manageEvents()
{
    switch (_state)
    {
    case ST_Idle:
        manageIdleState();
        break;
    case ST_NoDir:
        manageNoDirState();
        break;
    case ST_Dir1:
        manageDir1State();
        break;
    case ST_Dir2:
        manageDir2State();
        break;
    case ST_Dir3:
        manageDir3State();
        break;
    case ST_Dir3Only:
        manageDir3OnlyState();
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
    int solarPower = _building->calculateSolarPower();
    if(solarPower == -1)
    {
        //handle error
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
    /* If the power production from the solar panels did not fluctuate, we don't need to proceed calculations */
    if (solarPower == prevSolarPower) return;

    prevSolarPower = solarPower;
    int availablePower = 20 + solarPower;
    supplyPowerToBuidling(solarPower);

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
 * @brief This function is called when a director swipes his RFID card
 * @return 0 (success) / 1 (failure)
 */
void PLB::supplyPowerToBuidling(int solarPower)
{
    _building->charge(80 - solarPower);
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
        if (station->getId() == _directorStations.at(i))
        {
            _directorStations.erase(_directorStations.begin() + i);
            return;
        }
    }
    for (size_t i = 0; i < _userStations.size(); i++)
    {
        if (station->getId() == _userStations.at(i))
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
    _directorStations.emplace_back(station->getId());
    return 0;
}

void PLB::loop()
{
    if(isTimeout())
    {
        _event = EV_timeout;
    }
    manageEvents();
}
