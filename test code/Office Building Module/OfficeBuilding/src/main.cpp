#include <Arduino.h>
#include <string.h>
#include "OfficeBuilding.h"
#include "BuildingDisplay.h"
#include "SolarPanel.h"
#include "MQTTClientPLB.h"

MQTTClientPLB* mqttPLB;
BuildingDisplay* buidlingDisplay;
SolarPanel* solarPanel1;
SolarPanel* solarPanel2;
SolarPanel* solarPanel3;
SolarPanel* solarPanel4;
OfficeBuilding* building;

void onConnectionEstablished() {
  mqttPLB->onConnectionSubscribe();
}

void setup() {
  Serial.begin(115200);
  mqttPLB = new MQTTClientPLB();
  building = new OfficeBuilding(mqttPLB, buidlingDisplay, solarPanel1, solarPanel2, solarPanel3, solarPanel4);
  mqttPLB->getClient().enableDebuggingMessages();
}

void loop() {
  building->handleEvent(mqttPLB->receive());
}