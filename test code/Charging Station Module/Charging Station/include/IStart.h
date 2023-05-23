#pragma once

class IStart
{
public:
    virtual ~IStart() = default;

    virtual bool isStarted() = 0;
};

IStart::~IStart()
{
}