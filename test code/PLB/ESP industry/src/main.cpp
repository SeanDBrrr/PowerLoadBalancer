#include <Arduino.h>
#include <string.h>
#include "PLB.h"
#include "MQTTClientStation.h"
#include "MQTTClientBuilding.h"

MQTTClientStation *mqttStation1 = new MQTTClientStation(1);
MQTTClientStation *mqttStation2 = new MQTTClientStation(2);
MQTTClientStation *mqttStation3 = new MQTTClientStation(3);
MQTTClientStation *mqttStation4 = new MQTTClientStation(4);
MQTTClientBuilding *mqttBuilding = new MQTTClientBuilding();
PLB plb(mqttBuilding, mqttStation1, mqttStation2, mqttStation3, mqttStation4);

void onConnectionEstablished() {
  mqttStation1->onConnectionSubscribe();
  mqttStation2->onConnectionSubscribe();
  mqttStation3->onConnectionSubscribe();
  mqttStation4->onConnectionSubscribe();
  mqttBuilding->onConnectionSubscribe();
}

void setup() {
  Serial.begin(115200);
  // mqttStation1 = new MQTTClientStation(1);
  // mqttStation2 = new MQTTClientStation(2);
  // mqttStation3 = new MQTTClientStation(3);
  // mqttStation4 = new MQTTClientStation(4);
  // mqttBuilding = new MQTTClientBuilding();
}

void loop() {
  plb.loop();
  plb.setEvent(mqttStation1->loop());
  // plb.manageEvents();
  plb.setEvent(mqttStation2->loop());
  // plb.manageEvents();
  plb.setEvent(mqttStation3->loop());
  // plb.manageEvents();
  plb.setEvent(mqttStation4->loop());
  plb.manageEvents();
}