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
                ST_Dir3Only};

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
    void _calculatePower(int power);

public:
    PLB(IBuilding *building, IStation *station);

    ~PLB() {}

    /* PLB Public Functions */
    void addStation(IStation* station);
    void supplyPowerToStation(IStation* station);
    void supplyPowerToBuidling();
    void stopSupply(IStation* station);
    bool checkDirector(IStation* station, int directoId);
    void isTimeout();
    void loop();

    void manageEvents(Events ev);
    void manageIdleState(Events ev);
    void manageNoDirState(Events ev);
    void manageDir1State(Events ev);
    void manageDir2State(Events ev);
    void manageDir3State(Events ev);
    void manageDir3OnlyState(Events ev);

    /* Getters & Setters */
    inline const IBuilding *
    getBuilding() const { return _building; }

    inline const IStation *
    getStation(int stationId) const { return _stations.at(stationId-1); }

    inline void
    changeMode(Modes mode) { _mode = mode; }

};


#endif /* _PLB_HPP */
