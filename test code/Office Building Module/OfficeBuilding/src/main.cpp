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

int sdaPin = 1;
int sclPin = 2;

void onConnectionEstablished() {
  mqttPLB->onConnectionSubscribe();
}

void setup() {
  Serial.begin(115200);
  mqttPLB = new MQTTClientPLB();
  buidlingDisplay = new BuildingDisplay(sclPin, sdaPin);
  solarPanel1 = new SolarPanel(1);
  solarPanel2 = new SolarPanel(2);
  solarPanel3 = new SolarPanel(3);
  solarPanel4 = new SolarPanel(4);
  building = new OfficeBuilding(mqttPLB, buidlingDisplay, solarPanel1, solarPanel2, solarPanel3, solarPanel4);
  mqttPLB->getClient().enableDebuggingMessages();
}

void loop() {
  building->handleEvent(mqttPLB->receive());
}