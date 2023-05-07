#include "PLB.h"

void 
PLB::addStation(const IStation& station) {
    _stations.emplace_back(std::move(station));
}

void 
PLB::manageIdleState(Events ev) {
    switch (ev) {
    case timeout: 
        _building.getPowerProduced();
        supplyPowerToBuidling();
        break;
    case supply:
        supplyPowerToStation(_stations[1]);
        break;
    }
}

void 
PLB::manageNoDirState(Events ev) { 

}

void 
PLB::manageDir1State(Events ev) {

}

void 
PLB::manageDir2State(Events ev) {

}

void 
PLB::manageDir3State(Events ev) {

}

void 
PLB::manageEvents(Events ev) {
    switch (_state) {
    
    }
}
    
void 
PLB::_calculatePower(int solarPower) {
    static int prevSolarPower = 0;
    if (solarPower==prevSolarPower) return;    
    int availablePower = 20+solarPower;
    int directorPower, userPower;
    switch (_state) {
    case IDLE:
        break;
    case NO_DIR: 
        userPower = availablePower/busyStations;
        for (size_t i = 0; i < _userStations.size(); i++) {
            _userStations.at(i).charge(userPower);
        }
        break;
    case DIR1:
        directorPower = 11;
        userPower = busyStations>1 ? (availablePower-directorPower)/(busyStations-1): 0;
        _directorStations.at(0).charge(directorPower);
        for (size_t i = 0; i < _userStations.size(); i++) {
            _userStations.at(i).charge(userPower);
        }
        break;
    case DIR2:
        directorPower = (availablePower >= 22) ? 11 : availablePower/2;
        userPower = (busyStations>2 && availablePower > 22) ? (availablePower-directorPower)/(busyStations-2) : 0;
        _directorStations.at(0).charge(directorPower);
        _directorStations.at(1).charge(directorPower);
        for (size_t i = 0; i < _userStations.size(); i++) {
            _userStations.at(i).charge(userPower);
        }
        break;
    case DIR3:
        int stationPower = availablePower/4;
        _userStations.at(0).charge(stationPower);
        for (size_t i = 0; i < _directorStations.size(); i++) {
            _directorStations.at(i).charge(stationPower);
        }
        break;
    case DIR3_ONLY:
        if (availablePower > 22) {
            for (size_t i = 0; i < _userStations.size(); i++) {
                _stations.at(i).switchMode(FCFS);
            }
            _directorStations.at(0).charge(11);
            _directorStations.at(1).charge(11);
            _directorStations.at(2).charge(availablePower-22);
        } 
        else {
            for (size_t i = 0; i < _userStations.size(); i++) {
                _stations.at(i).switchMode(Dynamic);
                _directorStations.at(0).charge(availablePower/3);
            }
        }
        break;
    default:
        break;
    }
    prevSolarPower = solarPower;
}

/*
 * @brief This function is called only when a user press the start button
 * @return void
*/
void 
PLB::supplyPowerToStation(IStation& station) {
    ++busyStations;
    int buidlingPower = _building.getPowerProduced();
    if (busyStations > _directorIds.size()) {
        _userStations.emplace_back(std::move(station));
    }
    
    _calculatePower(buidlingPower);
}

/*
 * @brief This function is called when a director swipes his RFID card
 * @return 0 (success) / 1 (failure)
*/
void 
PLB::supplyPowerToBuidling() {
    _building.charge(80);
}

/*
 * @brief This function is called only when a user press the start button
 * @return void
*/
void 
PLB::stopSupply(IStation& station) {
    --busyStations;
    for (size_t i = 0; i < _directorStations.size(); i++) {
        if (station.getId()==_directorStations.at(i).getId()) {
            _directorStations.erase(_directorStations.cbegin()+i);
            return;
        }
    }
    for (size_t i = 0; i < _userStations.size(); i++) {
        if (station.getId()==_userStations.at(i).getId()) {
            _userStations.erase(_userStations.cbegin()+i);
            return;
        }
    }
}

/*
 * @brief This function is called when a director swipes his RFID card
 * @return 0 (success) / 1 (failure)
*/
bool 
PLB::checkDirector(IStation& station, int directoId) {
    for (size_t i = 0; i < _directorIds.size(); i++) {
        /* return RFID state: failed(-1), checkedIn(1), notCheckedIn(0) */
        if (_directorIds.at(i)==directoId) return 1;
    }
    _directorIds.emplace_back(directoId);
    _directorStations.emplace_back(std::move(station));
    return 0;
}

