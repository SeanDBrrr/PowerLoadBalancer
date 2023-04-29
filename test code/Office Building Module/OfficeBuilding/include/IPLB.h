#ifndef IPLB_H
#define IPLB_H
#include <Arduino.h>
#include <string.h>

class IPLB
{ 
    public:
    IPLB(){}
    ~IPLB(){}

    virtual int supplyPower() = 0;
    virtual void Send(String topic, String attr);
};

#endif