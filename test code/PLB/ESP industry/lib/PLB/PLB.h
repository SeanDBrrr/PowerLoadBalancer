#ifndef _PLB_HPP
#define _PLB_HPP

#include <vector>
#include <utility>
#include <functional>
#include <Arduino.h>
#include "Enums.h"
#include "IBuilding.h"
#include "IStation.h"

class PLB
{
    int busyStations;

private:
    PLBStates _state;
    PLBModes _mode;
    PLBEvents _event;
    IBuilding *_building;
    std::vector<IStation *> _stations;
    std::vector<int> _directorStations;
    std::vector<int> _userStations;
    std::vector<int> _directorIds;

    /* PLB Private Functions */
    void _distributePower(int power);
    void _supplyPowerToStation(IStation* station);
    void _supplyPowerToBuilding(int solarPower);
    int _stopSupply(IStation* station);

public:
    PLB(IBuilding *building,
        IStation *station1,
        IStation *station2,
        IStation *station3,
        IStation *station4
    );

    ~PLB() {}

    /* PLB Public Functions */
    void addStation(IStation* station);
    
    bool checkDirector(IStation* station);
    bool isTimeout();
    void loop();

    void handleEvents(PLBEvents ev);
    PLBStates handleIdleState(PLBEvents ev);
    PLBStates handleNoDirState(PLBEvents ev);
    PLBStates handleDir1State(PLBEvents ev);
    PLBStates handleDir2State(PLBEvents ev);
    PLBStates handleDir3State(PLBEvents ev);
    PLBStates handleDir3OnlyState(PLBEvents ev);
    PLBStates handleDir4State(PLBEvents ev);

    /* Getters & Setters */
    inline const IBuilding *
    getBuilding() const { return _building; }

    inline const IStation *
    getStation(int stationId) const { return _stations.at(stationId-1); }

    inline void
    changeMode(PLBModes mode) { _mode = mode; }

};


#endif /* _PLB_HPP */
