#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

#include "PLB.h"
#include "MockBuilding.h"
#include "MockStation.h"
#include "Topics.h"

using namespace testing;

class testIdleState : public ::testing::Test
{
protected:
    testIdleState(IBuilding building, IStation station1, IStation station2, IStation station3, IStation station4) : 
        PLB(&building, &station1, &station2, &station3, &station4) {
            _building = building;
            _stations.emplace_back(station1);
            _stations.emplace_back(station2);
            _stations.emplace_back(station3);
            _stations.emplace_back(station4);
        }
    virtual ~testIdleState();

    MockBuilding mockBuilding;
    std::vector<MockStation> mockStations;
    PLB plb;
};


TEST_F(testIdleState, test_timeout_event) {
    /*1. Variables declaration */
    int power = 80;

    /*2. Method test */
    PLBStates state = plb.manageIdleState(EV_timeout);

    /*3. Calls expectations */
    EXPECT_CALL(plb, _distributePower()).Times(1);
    EXPECT_CALL(mockBuilding, calculateSolarPower()).Times(1);
    EXPECT_CALL(mockBuilding, send(mqtt_topic_calculateSolarPower, "Calculate Solar Power")).Times(1);
    EXPECT_CALL(mockBuilding, receive()).Times(1);
    
    for(const auto &mock_station: mockStations)
    {
        EXPECT_CALL(mock_station, charge(0)).Times(0);
    }
    EXPECT_CALL(plb, supplyPowerToBuidling()).Times(1);
    EXPECT_CALL(mockBuilding, charge(power)).Times(1);
    EXPECT_CALL(mockBuilding, send(mqtt_topic_charge_building, power)).Times(1);

    /*4. Return value expectation */
    EXPECT_EQ(ST_Idle, state);
}

TEST_F(testIdleState, test_supply1_event) {
    /*1. Variables declaration */
    int power = 80;

    /*2. Method test */
    PLBStates state = plb.manageIdleState(EV_supply1);

    /*3. Calls expectations */
    EXPECT_CALL(plb, _supplyPowerToStation(&mockStations.at(0))).Times(1);
    EXPECT_CALL(mockBuilding, calculateSolarPower()).Times(1);
    EXPECT_CALL(mockBuilding, send(mqtt_topic_calculateSolarPower, "Calculate Solar Power")).Times(1);
    EXPECT_CALL(plb, _distributePower()).Times(1);
    EXPECT_CALL(mockBuilding, receive()).Times(1);
    
    for(int i=0, i<mockStations.size(), i++)
    {
        if(i==0) { EXPECT_CALL(mock_station, charge(11)).Times(1); }
        else { EXPECT_CALL(mock_station, charge(0)).Times(0); }
    }
    EXPECT_CALL(plb, supplyPowerToBuidling()).Times(1);
    EXPECT_CALL(mockBuilding, charge(power)).Times(1);
    EXPECT_CALL(mockBuilding, send(mqtt_topic_charge_building, power)).Times(1);

    /*4. Return values expectations*/
    EXPECT_EQ(ST_NoDir, state);
}

TEST_F(testIdleState, test_supply2_event) {
    /*1. Variables declaration */
    int power = 80;

    /*2. Method test */
    PLBStates state =  plb.manageIdleState(EV_supply2);

    /*3. Calls expectations */
    EXPECT_CALL(plb, _supplyPowerToStation(&mockStations.at(1))).Times(1);
    EXPECT_CALL(mockBuilding, calculateSolarPower()).Times(1);
    EXPECT_CALL(plb, _distributePower()).Times(1);
    EXPECT_CALL(mockBuilding, send(mqtt_topic_calculateSolarPower, "Calculate Solar Power")).Times(1);
    EXPECT_CALL(mockBuilding, receive()).Times(1);
    
    for(int i=0, i<mockStations.size(), i++)
    {
        if(i==1) { EXPECT_CALL(mock_station, charge(11)).Times(1); }
        else { EXPECT_CALL(mock_station, charge(0)).Times(0); }
    }
    EXPECT_CALL(plb, supplyPowerToBuidling()).Times(1);
    EXPECT_CALL(mockBuilding, charge(power)).Times(1);
    EXPECT_CALL(mockBuilding, send(mqtt_topic_charge_building, power)).Times(1);

    /*4. Return values expectations*/
    EXPECT_EQ(ST_NoDir, state);
}

TEST_F(testIdleState, test_supply3_event) {
    /*1. Variables declaration */
    int power = 80;

    /*2. Method test */
    PLBStates state =  plb.manageIdleState(EV_supply3);

    /*3. Calls expectations */
    EXPECT_CALL(plb, _supplyPowerToStation(&mockStations.at(2))).Times(1);
    EXPECT_CALL(mockBuilding, calculateSolarPower()).Times(1);
    EXPECT_CALL(plb, _distributePower()).Times(1);
    EXPECT_CALL(mockBuilding, send(mqtt_topic_calculateSolarPower, "Calculate Solar Power")).Times(1);
    EXPECT_CALL(mockBuilding, receive()).Times(1);
    
    for(int i=0, i<mockStations.size(), i++)
    {
        if(i==2) { EXPECT_CALL(mock_station, charge(11)).Times(1); }
        else { EXPECT_CALL(mock_station, charge(0)).Times(0); }
    }
    EXPECT_CALL(plb, supplyPowerToBuidling()).Times(1);
    EXPECT_CALL(mockBuilding, charge(power)).Times(1);
    EXPECT_CALL(mockBuilding, send(mqtt_topic_charge_building, power)).Times(1);

    /*4. Return values expectations*/
    EXPECT_EQ(ST_NoDir, state);
}

TEST_F(testIdleState, test_supply4_event) {
    /*1. Variables declaration */
    int power = 80;

    /*2. Method test */
    PLBStates state =  plb.manageIdleState(EV_supply4);

    /*3. Calls expectations */
    EXPECT_CALL(plb, _supplyPowerToStation(&mockStations.at(3))).Times(1);
    EXPECT_CALL(mockBuilding, calculateSolarPower()).Times(1);
    EXPECT_CALL(plb, _distributePower()).Times(1);
    EXPECT_CALL(mockBuilding, send(mqtt_topic_calculateSolarPower, "Calculate Solar Power")).Times(1);
    EXPECT_CALL(mockBuilding, receive()).Times(1);
    
    for(int i=0, i<mockStations.size(), i++)
    {
        if(i==3) { EXPECT_CALL(mock_station, charge(11)).Times(1); }
        else { EXPECT_CALL(mock_station, charge(0)).Times(0); }
    }
    EXPECT_CALL(plb, supplyPowerToBuidling()).Times(1);
    EXPECT_CALL(mockBuilding, charge(power)).Times(1);
    EXPECT_CALL(mockBuilding, send(mqtt_topic_charge_building, power)).Times(1);

    /*4. Return values expectations*/
    EXPECT_EQ(ST_NoDir, state);
}

TEST_F(testIdleState, test_director1_event) {
    /*1. Variables declaration */

    int directorId = 1;

    /*2. Method test */
    PLBStates state =  plb.manageIdleState(EV_director1);

    /*3. Calls expectations */
    EXPECT_CALL(plb, checkDirector(directorId)).Times(1);

    /*4. Return values expectations*/
    EXPECT_EQ(ST_Dir1, state);
}

TEST_F(testIdleState, test_director2_event) {
    /*1. Variables declaration */
    int directorId = 1;

    /*2. Method test */
    PLBStates state =  plb.manageIdleState(EV_director2);

    /*3. Calls expectations */
    EXPECT_CALL(plb, checkDirector(directorId)).Times(1);

    /*4. Return values expectations*/
    EXPECT_EQ(ST_Dir1, state);
}

TEST_F(testIdleState, test_director3_event) {
    /*1. Variables declaration */
    int directorId = 1;

    /*2. Method test */
    PLBStates state =  plb.manageIdleState(EV_director3);

    /*3. Calls expectations */
    EXPECT_CALL(plb, checkDirector(directorId)).Times(1);

    /*4. Return values expectations*/
    EXPECT_EQ(ST_Dir1, state);
}

TEST_F(testIdleState, test_director4_event) {
    /*1. Variables declaration */
    int directorId = 1;

    /*2. Method test */
    PLBStates state =  plb.manageIdleState(EV_director4);

    /*3. Calls expectations */
    EXPECT_CALL(plb, checkDirector(directorId)).Times(1);

    /*4. Return values expectations*/
    EXPECT_EQ(ST_Dir1, state);
}
