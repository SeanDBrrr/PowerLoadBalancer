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


int stationId = 2;
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
  mqttPLB->getClient().enableLastWillMessage("group4/heartbeat", "ONLINE2");
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
  
  chargingStation->loop(mqttPLB->getEvent());
  mqttPLB->receive();
}