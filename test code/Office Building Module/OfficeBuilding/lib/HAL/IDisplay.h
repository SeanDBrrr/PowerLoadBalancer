#ifndef IDISPLAY_H
#define IDISPLAY_H
#include <Arduino.h>

class IDisplay
{
public:
    virtual ~IDisplay() = default;
    virtual void display(String msg1, String msg2 = "") = 0;
};

#endif