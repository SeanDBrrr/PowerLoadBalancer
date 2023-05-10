#include <Arduino.h>
#include <string.h>
#include "PLB.h"
#include "MQTTServer.h"

MQTTServer mqttStation1;
MQTTServer mqttStation2;
MQTTServer mqttStation3;
MQTTServer mqttStation4;
MQTTServer mqttBuidling;
PLB plb(mqttBuidling, mqttStation1);
Events event;

void setup() {
  Serial.begin(115200);
}

void onConnectionEstablished() {
  mqtt.onConnectionSubscribe();
}

void loop() {
  plb.loop();
  if(plb.isSupplyRequest()) {
    switch (mqtt.getStationId()) {
    case 1:
      event = EV_supply1;
      break;
    case 2:
      event = EV_supply2;
      break;
    case 3:
      event = EV_supply3;
      break;
    case 4:
      event = EV_supply4;
      break;
    }
    mqtt.clearSupplyRequest();
  }
  plb.manageEvents(event);
  event = noEvent;
}