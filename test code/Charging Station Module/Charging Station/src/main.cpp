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
  mqttPLB = new MQTTClientPLB(1);
  mqttPLB->getClient().enableDebuggingMessages();
  chargingStation = new ChargingStation(
    1,
    startButton,
    plugButton,
    director,
    lcd,
    mqttPLB
  );
}

void loop() {
  chargingStation->loop();
  mqttPLB->receive();
  chargingStation->HandleEvent(mqttPLB->getEvent());
}