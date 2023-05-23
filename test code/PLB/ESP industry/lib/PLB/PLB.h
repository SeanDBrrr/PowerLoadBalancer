#ifndef _PLB_HPP
#define _PLB_HPP

#include <array>
#include <vector>
#include <list>
#include <utility>
#include <functional>
#include "IBuilding.h"
#include "IStation.h"

enum PLBStates {ST_Idle=1, 
                ST_NoDir, 
                ST_Dir1, 
                ST_Dir2, 
                ST_Dir3, 
                ST_Dir3Only,
                ST_Dir4};

enum PLBModes {MO_Auto=1, 
                MO_Manual};

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
    PLB(IBuilding *building, IStation *station1, IStation *station2, IStation *station3, IStation *station4);

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