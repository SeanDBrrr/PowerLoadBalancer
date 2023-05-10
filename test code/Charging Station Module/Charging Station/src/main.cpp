#include <Arduino.h>
#include "ChargingStation.h"

int PlugPin = 0;
int PlugState = 0;

void setup() {
  button/*Plug*/ Plug(PlugState);
  ChargingStation chargingStation(Plug);
  //LCD lcd(0x27,20,4);
}

void loop() {
  // put your main code here, to run repeatedly:
}