#ifndef MOCKSTATION_HPP
#define MOCKSTATION_HPP

#include <IStation.h>
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockStation : public IStation
{
public:
    MOCK_METHOD(int, getId, ());
    MOCK_METHOD(void, charge, (float));
    MOCK_METHOD(void, switchMode, (StationModes));
};

#endif