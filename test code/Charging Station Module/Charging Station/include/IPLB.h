#pragma once

#ifndef _IPLB_HPP
#define _IPLB_HPP

class IPLB
{
public:
    virtual ~IPLB();
    virtual int supplyPower() = 0;
    virtual int stopSupply() = 0;
    virtual void checkDirector(int id) = 0;
};

IPLB::~IPLB()
{
}

#endif