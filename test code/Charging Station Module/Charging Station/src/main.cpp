#include <Arduino.h>
#include "ChargingStation.h"

int PlugPin = 0;
int PlugState = 0;

void setup() {
  button/*Plug*/ Plug(PlugPin, PlugState);
  ChargingStation chargingStation(Plug)
}

void loop() {
  // put your main code here, to run repeatedly:
}