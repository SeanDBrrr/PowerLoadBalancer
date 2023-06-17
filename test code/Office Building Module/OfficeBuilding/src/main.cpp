#include <Arduino.h>
#include <string.h>
#include "OfficeBuilding.h"
#include "BuildingDisplay.h"
#include "SolarPanelLDR.h"
#include "MQTTClientPLB.h"
#include "Button.h"
#include "OfficeKey.h"

MQTTClientPLB *mqttPLB;
BuildingDisplay *buildingDisplay;
SolarPanelLDR *solarPanel1;
SolarPanelLDR *solarPanel2;
SolarPanelLDR *solarPanel3;
SolarPanelLDR *solarPanel4;
OfficeBuilding *building;
OfficeKey *officeKey;

const int  sdaPin = 21;
const int  sclPin = 22;
const int solarPanelPin1 = 32;
const int solarPanelPin2 = 33;
const int solarPanelPin3 = 34;
const int solarPanelPin4 = 35;
void onConnectionEstablished()
{
  mqttPLB->onConnectionSubscribe();
}

void setup()
{
  Serial.begin(115200);
  mqttPLB = new MQTTClientPLB();
  buildingDisplay = new BuildingDisplay(sclPin, sdaPin);
  officeKey = new OfficeKey();
  solarPanel1 = new SolarPanelLDR(solarPanelPin1);
  solarPanel2 = new SolarPanelLDR(solarPanelPin2);
  solarPanel3 = new SolarPanelLDR(solarPanelPin3);
  solarPanel4 = new SolarPanelLDR(solarPanelPin4);
  building = new OfficeBuilding(mqttPLB, buildingDisplay, officeKey, solarPanel1, solarPanel2, solarPanel3, solarPanel4);
  mqttPLB->getClient().enableDebuggingMessages();
}

void loop()
{
 building->loop(mqttPLB->receive());
 //buildingDisplay->display("Hello"); //hardware test

}