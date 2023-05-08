#ifndef _PLB_HPP
#define _PLB_HPP

#include "vector"
#include "utility"
#include "IBuilding.h"
#include "IStation.h"

enum States {IDLE=1, NO_DIR, DIR1, DIR2, DIR3, DIR3_ONLY};
enum Modes {Auto=1, Manual};
enum Events {noEvent = 0, timeout=1, supply1, supply2, supply3, supply4, stop, director}variable;

class PLB
{
    static int busyStations;

private:
    States _state;
    Modes _mode;
    IBuilding& _building;
    std::vector<IStation&> _stations;
    std::vector<IStation&> _directorStations;
    std::vector<IStation&> _userStations;
    std::vector<int> _directorIds;

    /* PLB Private Functions */
    void _calculatePower(int power);

public:
    PLB(IBuilding& building, IStation& station): _building{building} {
        _stations.emplace_back(std::move(station));
    }

    ~PLB() {}

    /* PLB Public Functions */
    void addStation(const IStation& station);
    void supplyPowerToStation(IStation& station);
    void supplyPowerToBuidling();
    void stopSupply(IStation& station);
    bool checkDirector(IStation& station, int directoId);

    void manageEvents(Events ev);
    void manageIdleState(Events ev);
    void manageNoDirState(Events ev);
    void manageDir1State(Events ev);
    void manageDir2State(Events ev);
    void manageDir3State(Events ev);

    /* Getters & Setters */
    inline
    const IBuilding&
    getBuilding() const { return _building; }
    
    inline
    const IStation&
    getStation(int stationId) const { return _stations.at(stationId-1); }

    inline
    void 
    changeMode(Modes mode) { _mode = mode; }
};





#endif /* _PLB_HPP */
