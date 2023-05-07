#pragma once

#ifndef _IPlug_HPP
#define _IPlug_HPP

class IPlug
{
public:
    virtual ~IPlug();

    virtual bool isPlugged() = 0;
};

IPlug::~IPlug()
{
}

#endif