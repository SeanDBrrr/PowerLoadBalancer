#ifndef LDR_H
#define LDR_H

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