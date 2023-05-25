#ifndef _IStation_HPP
#define _IStation_HPP

enum class StationModes {
    MO_Dynamic = 1,
    MO_FCFS,
    MO_Director,
};

class IStation
{
public:
    virtual ~IStation() = default;
    virtual int getId() = 0;
    virtual int getDirectorId() = 0;
    void validateDirector(bool directorValidated);
    virtual void charge(float power) = 0;
    virtual void switchMode(StationModes mode) = 0;
};

#endif