#include <Arduino.h>
#include <string.h>
#include "OfficeBuilding.h"
#include "BuildingDisplay.h"
#include "SolarPanel.h"
#include "MQTTClientPLB.h"

MQTTClientPLB *mqttPLB;
BuildingDisplay *buildingDisplay;
SolarPanel *solarPanel1;
SolarPanel *solarPanel2;
SolarPanel *solarPanel3;
SolarPanel *solarPanel4;
OfficeBuilding *building;

int sdaPin = 21;
int sclPin = 22;
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
  solarPanel1 = new SolarPanel(solarPanelPin1);
  solarPanel2 = new SolarPanel(solarPanelPin2);
  solarPanel3 = new SolarPanel(solarPanelPin3);
  solarPanel4 = new SolarPanel(solarPanelPin4);
  building = new OfficeBuilding(mqttPLB, buildingDisplay, solarPanel1, solarPanel2, solarPanel3, solarPanel4);
  mqttPLB->getClient().enableDebuggingMessages();
}

void loop()
{
  // mqttPLB->checkConnection()
  // buildingDisplay->display("Wifi connection", "Is success !");
  // delay(1000);

  building->handleEvent(mqttPLB->receive());

}