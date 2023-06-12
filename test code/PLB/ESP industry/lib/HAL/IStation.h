#ifndef _IStation_HPP
#define _IStation_HPP

#include "stdint.h"
#include <Arduino.h>
#include "Enums.h"

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
    virtual StationModes getStationMode() = 0;
    virtual PLBModes getPLBMode() = 0;
};

#endif