#ifndef SOLAR_LDR_H
#define SOLAR_LDR_H

class LDR
{
private:
    int _pin;

public:
    LDR(int pin);
    ~LDR() {}
    int readPin();
};

#endif