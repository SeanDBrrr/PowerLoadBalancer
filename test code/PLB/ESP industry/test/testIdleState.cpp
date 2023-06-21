#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "PLB.h"
#include "MockBuilding.h"
#include "MockStation.h"

using namespace testing;

class testIdleState : public ::testing::Test
{
protected:
    testIdleState() : plb(&mockBuilding, &mockStation1, &mockStation2, &mockStation3, &mockStation4) { }

    MockBuilding mockBuilding;
    MockStation mockStation1;
    MockStation mockStation2;
    MockStation mockStation3;
    MockStation mockStation4;
    PLB plb;
};


TEST_F(testIdleState, test_timeout_event) {
    /*1. Variables declaration */
    float solarPower = 10;

    /*2. Method test */
    PLBStates state = plb.handleIdleState(PLBEvents::EV_Timeout);

    /*3. Calls expectations */
    EXPECT_CALL(mockBuilding, calculateSolarPower()).Times(1);

    // plb._distributePower(solarPower);

    EXPECT_CALL(mockBuilding, charge(80)).Times(1);
    // EXPECT_CALL(mockStation1, charge(0)).Times(0);
    // EXPECT_CALL(mockStation2, charge(0)).Times(0);
    // EXPECT_CALL(mockStation3, charge(0)).Times(0);
    // EXPECT_CALL(mockStation4, charge(0)).Times(0);

    /*4. Return value expectation */
    EXPECT_EQ(PLBStates::ST_Idle, state);
}

// TEST_F(testIdleState, test_supply1_event) {
//     /*1. Variables declaration */
//     float solarPower = 10;

//     /*2. Method test */
//     PLBStates state = plb.handleIdleState(PLBEvents::EV_Supply);

//     /*3. Calls expectations */
//     EXPECT_CALL(mockBuilding, calculateSolarPower()).Times(1);
//     EXPECT_CALL(mockBuilding, charge(solarPower)).Times(1);
//     EXPECT_CALL(mockStation1, charge(11)).Times(1);
//     EXPECT_CALL(mockStation2, charge(0)).Times(0);
//     EXPECT_CALL(mockStation3, charge(0)).Times(0);
//     EXPECT_CALL(mockStation4, charge(0)).Times(0);

//     /*4. Return value expectation */
//     EXPECT_EQ(PLBStates::ST_NoDir, state);
// }

// TEST_F(testIdleState, test_supply2_event) {
//     /*1. Variables declaration */
//     int power = 80;

//     /*2. Method test */
//     PLBStates state =  plb.manageIdleState(EV_supply2);

//     /*3. Calls expectations */
//     EXPECT_CALL(plb, _supplyPowerToStation(&mockStations.at(1))).Times(1);
//     EXPECT_CALL(mockBuilding, calculateSolarPower()).Times(1);
//     EXPECT_CALL(plb, _distributePower()).Times(1);
//     EXPECT_CALL(mockBuilding, send(mqtt_topic_calculateSolarPower, "Calculate Solar Power")).Times(1);
//     EXPECT_CALL(mockBuilding, receive()).Times(1);
    
//     for(int i=0, i<mockStations.size(), i++)
//     {
//         if(i==1) { EXPECT_CALL(mock_station, charge(11)).Times(1); }
//         else { EXPECT_CALL(mock_station, charge(0)).Times(0); }
//     }
//     EXPECT_CALL(plb, supplyPowerToBuidling()).Times(1);
//     EXPECT_CALL(mockBuilding, charge(power)).Times(1);
//     EXPECT_CALL(mockBuilding, send(mqtt_topic_charge_building, power)).Times(1);

//     /*4. Return values expectations*/
//     EXPECT_EQ(ST_NoDir, state);
// }

// TEST_F(testIdleState, test_supply3_event) {
//     /*1. Variables declaration */
//     int power = 80;

//     /*2. Method test */
//     PLBStates state =  plb.manageIdleState(EV_supply3);

//     /*3. Calls expectations */
//     EXPECT_CALL(plb, _supplyPowerToStation(&mockStations.at(2))).Times(1);
//     EXPECT_CALL(mockBuilding, calculateSolarPower()).Times(1);
//     EXPECT_CALL(plb, _distributePower()).Times(1);
//     EXPECT_CALL(mockBuilding, send(mqtt_topic_calculateSolarPower, "Calculate Solar Power")).Times(1);
//     EXPECT_CALL(mockBuilding, receive()).Times(1);
    
//     for(int i=0, i<mockStations.size(), i++)
//     {
//         if(i==2) { EXPECT_CALL(mock_station, charge(11)).Times(1); }
//         else { EXPECT_CALL(mock_station, charge(0)).Times(0); }
//     }
//     EXPECT_CALL(plb, supplyPowerToBuidling()).Times(1);
//     EXPECT_CALL(mockBuilding, charge(power)).Times(1);
//     EXPECT_CALL(mockBuilding, send(mqtt_topic_charge_building, power)).Times(1);

//     /*4. Return values expectations*/
//     EXPECT_EQ(ST_NoDir, state);
// }

// TEST_F(testIdleState, test_supply4_event) {
//     /*1. Variables declaration */
//     int power = 80;

//     /*2. Method test */
//     PLBStates state =  plb.manageIdleState(EV_supply4);

//     /*3. Calls expectations */
//     EXPECT_CALL(plb, _supplyPowerToStation(&mockStations.at(3))).Times(1);
//     EXPECT_CALL(mockBuilding, calculateSolarPower()).Times(1);
//     EXPECT_CALL(plb, _distributePower()).Times(1);
//     EXPECT_CALL(mockBuilding, send(mqtt_topic_calculateSolarPower, "Calculate Solar Power")).Times(1);
//     EXPECT_CALL(mockBuilding, receive()).Times(1);
    
//     for(int i=0, i<mockStations.size(), i++)
//     {
//         if(i==3) { EXPECT_CALL(mock_station, charge(11)).Times(1); }
//         else { EXPECT_CALL(mock_station, charge(0)).Times(0); }
//     }
//     EXPECT_CALL(plb, supplyPowerToBuidling()).Times(1);
//     EXPECT_CALL(mockBuilding, charge(power)).Times(1);
//     EXPECT_CALL(mockBuilding, send(mqtt_topic_charge_building, power)).Times(1);

//     /*4. Return values expectations*/
//     EXPECT_EQ(ST_NoDir, state);
// }

// TEST_F(testIdleState, test_director1_event) {
//     /*1. Variables declaration */

//     int directorId = 1;

//     /*2. Method test */
//     PLBStates state =  plb.manageIdleState(EV_director1);

//     /*3. Calls expectations */
//     EXPECT_CALL(plb, checkDirector(directorId)).Times(1);

//     /*4. Return values expectations*/
//     EXPECT_EQ(ST_Dir1, state);
// }

// TEST_F(testIdleState, test_director2_event) {
//     /*1. Variables declaration */
//     int directorId = 1;

//     /*2. Method test */
//     PLBStates state =  plb.manageIdleState(EV_director2);

//     /*3. Calls expectations */
//     EXPECT_CALL(plb, checkDirector(directorId)).Times(1);

//     /*4. Return values expectations*/
//     EXPECT_EQ(ST_Dir1, state);
// }

// TEST_F(testIdleState, test_director3_event) {
//     /*1. Variables declaration */
//     int directorId = 1;

//     /*2. Method test */
//     PLBStates state =  plb.manageIdleState(EV_director3);

//     /*3. Calls expectations */
//     EXPECT_CALL(plb, checkDirector(directorId)).Times(1);

//     /*4. Return values expectations*/
//     EXPECT_EQ(ST_Dir1, state);
// }

// TEST_F(testIdleState, test_director4_event) {
//     /*1. Variables declaration */
//     int directorId = 1;

//     /*2. Method test */
//     PLBStates state =  plb.manageIdleState(EV_director4);

//     /*3. Calls expectations */
//     EXPECT_CALL(plb, checkDirector(directorId)).Times(1);

//     /*4. Return values expectations*/
//     EXPECT_EQ(ST_Dir1, state);
// }
