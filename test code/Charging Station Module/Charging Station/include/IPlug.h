#pragma once

class IPlug
{
public:
    virtual ~IPlug() = default;

    virtual bool isPlugged() = 0;
};