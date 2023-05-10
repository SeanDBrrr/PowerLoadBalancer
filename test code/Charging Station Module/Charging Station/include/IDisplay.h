#pragma once

class IDisplay
{
public:
    virtual ~IDisplay();

    virtual int display(String) = 0;
};

IDisplay::~IDisplay()
{
}