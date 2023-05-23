#ifndef IDISPLAY_H
#define IDISPLAY_H
#include <Arduino.h>

class IDisplay
{
public:
    virtual ~IDisplay() = default;
    virtual void display(String message) = 0;
};

#endif