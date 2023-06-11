#ifndef _PLB_HPP
#define _PLB_HPP

#include <vector>
#include <queue>
#include <map>
#include <string>
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
    StationModes _stationsMode;
    std::queue<int> _stationIdEvents;

    IBuilding *_building;
    std::vector<IStation *> _stations;
    std::vector<int> _directorStations;
    std::vector<int> _userStations;
    std::vector<int> _occupancyStations;
    std::vector<uint32_t> _directorIds;
    std::vector<uint32_t> _validDirectorIds = {2267176004, 432104642, 1518206872, 40, 50};
    
    void _initialiseStations();
   
    /* Private functions (Auto mode) */
    void _distributePower(float solarPower);
    void _supplyPowerToStation(IStation* station);
    void _supplyPowerToBuilding(float solarPower);
    int _stopSupply(IStation* station);
    
    /* Private functions (Manual mode) */
    void _switchToAutoMode();
    void _supplyPowerDynamicMode(float availablePower);
    void _supplyPowerDirectorMode(float availablePower);
    void _supplyPowerFCFSMode(float availablePower);

public:
    /* Constructor and Destructor */
    PLB(IBuilding *building,
        IStation *station1,
        IStation *station2,
        IStation *station3,
        IStation *station4
    );

    ~PLB();

    /* PLB Public Functions */
    void addStation(IStation* station);
    int checkDirector(IStation* station);
    void setIdEvents(std::queue<int>& ids);
    bool isTimeout();
    void loop(std::vector<PLBEvents>& events);

    /* Public Functions (Auto mode) */
    void handleAutoModeEvents(PLBEvents ev);
    PLBStates handleIdleState(PLBEvents ev);
    PLBStates handleNoDirState(PLBEvents ev);
    PLBStates handleDir1State(PLBEvents ev);
    PLBStates handleDir2State(PLBEvents ev);
    PLBStates handleDir3State(PLBEvents ev);
    PLBStates handleDir3OnlyState(PLBEvents ev);
    PLBStates handleDir4State(PLBEvents ev);

    /* Public Functions (Manual mode) */
    void handleManualModeEvents(PLBEvents ev);
    void handleDynamicMode(PLBEvents ev);
    void handleDirectorMode(PLBEvents ev);
    void handleFCFSMode(PLBEvents ev);

    /* Getters & Setters */
    inline const IBuilding *
    getBuilding() const { return _building; }

    inline const IStation *
    getStation(int stationId) const { return _stations.at(stationId-1); }

    inline void
    changeMode(PLBModes mode) { _mode = mode; }

};
#endif /* _PLB_HPP */
