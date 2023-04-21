#ifndef IPLB_H
#define IPLB_H

class IPLB
{
public:
    virtual ~IPLB();
    virtual int SupplyPower() = 0;
    virtual int StopSupply() = 0;
    virtual void CheckDirector(int Id) = 0;
};

#endif