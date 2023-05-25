#pragma once
#include <Arduino.h>

class IDisplay
{
public:
    virtual ~IDisplay() = default;

    virtual void display(String) = 0;
};