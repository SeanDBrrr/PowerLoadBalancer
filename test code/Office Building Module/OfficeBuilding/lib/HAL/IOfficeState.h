#pragma once

class IOfficeState
{
public:
    virtual ~IOfficeState() = default;

    virtual bool isOpen() = 0;
};