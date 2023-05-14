#pragma once

class IStart
{
public:
    virtual ~IStart();

    virtual bool isStarted() = 0;
};

IStart::~IStart()
{
}