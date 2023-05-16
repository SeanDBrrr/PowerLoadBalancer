#ifndef _PLB_HPP
#define _PLB_HPP

#include <array>
#include <vector>
#include <list>
#include <utility>
#include <functional>
#include "IBuilding.h"
#include "IStation.h"
#include "PLBEnums.h"

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

public:
    PLB(IBuilding *building, IStation *station1, IStation *station2, IStation *station3, IStation *station4);

    ~PLB() {}

    /* PLB Public Functions */
    void addStation(IStation* station);
    void supplyPowerToStation(IStation* station);
    void supplyPowerToBuidling(int solarPower);
    void stopSupply(IStation* station);
    bool checkDirector(IStation* station, int directoId);
    bool isTimeout();
    void setEvent(PLBEvents ev) { _event = ev; }
    void loop();

    void manageEvents();
    PLBStates manageIdleState();
    PLBStates manageNoDirState();
    PLBStates manageDir1State();
    PLBStates manageDir2State();
    PLBStates manageDir3State();
    PLBStates manageDir3OnlyState();

    /* Getters & Setters */
    inline const IBuilding *
    getBuilding() const { return _building; }

    inline IStation *
    getStation(int stationId) const { return _stations.at(stationId); }

    inline void
    changeMode(PLBModes mode) { _mode = mode; }

};


#endif /* _PLB_HPP */
