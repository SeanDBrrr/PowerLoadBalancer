#ifndef MOCKBUILDING_HPP
#define MOCKBUILDING_HPP

#include <IBuilding.h>
#include <string.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockBuilding : public IBuilding
{
public:
    MOCK_METHOD(int, calculateSolarPower, ());
    MOCK_METHOD(void, charge, (float));
};

#endif