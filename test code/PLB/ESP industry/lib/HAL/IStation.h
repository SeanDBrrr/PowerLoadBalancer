#ifndef _IStation_HPP
#define _IStation_HPP

#include "stdint.h"
#include <Arduino.h>

enum class StationModes {
    MO_Dynamic = 1,
    MO_FCFS,
    MO_Director,
};

enum class DirectorState
{
    VALID = 1,
    INVALID = 0,
    ALREADY_CHECKED_IN = -1,
};

class IStation
{
public:
    virtual ~IStation() = default;
    virtual int getId() = 0;
    virtual uint32_t getDirectorId() = 0;
    virtual void validateDirector(DirectorState directorState) = 0;
    virtual void charge(float power) = 0;
    virtual void switchMode(StationModes mode) = 0;
    virtual void notifyDashboard(String message) = 0;
};

#endif