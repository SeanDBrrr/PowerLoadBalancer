#include <Arduino.h>
#include "ChargingStation.h"
#include "MQTTClientPLB.h"
#include "PlugButton.h"
#include "StartButton.h"
#include "DirectorScanner.h"
#include "StationScreen.h"
#include "Button.h"
#include "DirectorScanner.h"
#include "StationScreen.h"

int stationId = 3;
DirectorScanner *director;
StationScreen *lcd;
StartButton* startButton;
PlugButton* plugButton;
MQTTClientPLB* mqttPLB;
ChargingStation* chargingStation;

void onConnectionEstablished()
{
  mqttPLB->onConnectionSubscribe();
}

void setup()
{
  Serial.begin(115200);
  lcd = new StationScreen(22, 21);
  director = new DirectorScanner(SS_PIN, RST_PIN);
  startButton = new StartButton();
  plugButton = new PlugButton();
  mqttPLB = new MQTTClientPLB(stationId);
  chargingStation = new ChargingStation(
    stationId,
    startButton,
    plugButton,
    director,
    lcd,
    mqttPLB
  );
}

void loop() {
  // Event tempEV = mqttPLB->getEvent();
  // Event lastEV;
  // Event ReturnEvent;
  // if (tempEV != lastEV)
  // {
  //   Serial.println("PLB EVENT CHANGED");
  //   ReturnEvent = tempEV;
  //   lastEV = tempEV;
  // }
  // else
  // {
  //   ReturnEvent = Event::noEvent;
  // }
  
  chargingStation->loop(mqttPLB->getEvent());
  mqttPLB->receive();
}