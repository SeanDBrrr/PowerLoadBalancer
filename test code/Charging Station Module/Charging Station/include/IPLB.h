#pragma once

class IPLB
{
public:
    virtual ~IPLB() = default;
    virtual void supplyPowerToStation(int id) = 0;
    virtual void stopSupplyToStation(int id) = 0;
    virtual void checkDirector(int id) = 0;
};