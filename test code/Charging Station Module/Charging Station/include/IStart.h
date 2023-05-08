#pragma once

#ifndef _IStart_HPP
#define _IStart_HPP

class IStart
{
public:
    virtual ~IStart();

    virtual bool isStarted() = 0;
};

IStart::~IStart()
{
}

#endif