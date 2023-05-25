#pragma once
#include <stdint.h>

class IDirector
{
public:
    virtual ~IDirector() = default;

    virtual uint32_t getID() = 0;
};