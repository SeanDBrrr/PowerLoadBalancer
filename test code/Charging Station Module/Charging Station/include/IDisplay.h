#pragma once
#include <string>

class IDisplay
{
public:
    virtual ~IDisplay() = default;

    virtual int display(std::string) = 0;
};