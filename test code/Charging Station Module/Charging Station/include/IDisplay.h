#pragma once

class IPlug
{
public:
    virtual ~IPlug();

    virtual bool isPlugged() = 0;
};

IPlug::~IPlug()
{
}