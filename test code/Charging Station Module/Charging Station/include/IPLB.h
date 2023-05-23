#pragma once

class IPLB
{
public:
    virtual ~IPLB();
    virtual void supplyPowerToStation(int id) = 0;
    virtual void stopSupplyToStation(int id) = 0;
    virtual void checkDirector(int id) = 0;
};

IPLB::~IPLB()
{
}