#include <Arduino.h>
#include <string.h>
#include "PLB.h"
#include "MQTTClientStation.h"
#include "MQTTClientBuilding.h"

MQTTClientBuilding* mqttBuilding;
MQTTClientStation* mqttStation0;
MQTTClientStation* mqttStation1;
MQTTClientStation* mqttStation2;
MQTTClientStation* mqttStation3;
PLB* plb;

void onConnectionEstablished() {
  mqttBuilding->onConnectionSubscribe();
  mqttStation0->onConnectionSubscribe();
  mqttStation1->onConnectionSubscribe();
  mqttStation2->onConnectionSubscribe();
  mqttStation3->onConnectionSubscribe();
}

void setup() {
  Serial.begin(115200);
  mqttBuilding = new MQTTClientBuilding();
  mqttStation0 = new MQTTClientStation(0);
  mqttStation1 = new MQTTClientStation(1);
  mqttStation2 = new MQTTClientStation(2);
  mqttStation3 = new MQTTClientStation(3);
  plb = new PLB(mqttBuilding, mqttStation0, mqttStation1, mqttStation2, mqttStation3);
  mqttBuilding->getClient().enableDebuggingMessages();
  mqttStation0->getClient().enableDebuggingMessages();
  mqttStation1->getClient().enableDebuggingMessages();
  mqttStation2->getClient().enableDebuggingMessages();
  mqttStation3->getClient().enableDebuggingMessages();
}

void loop() {
  /* Check for any events */
  mqttBuilding->receive();
  mqttStation0->receive();
  mqttStation1->receive();
  mqttStation2->receive();
  mqttStation3->receive();

  /* Pass all events to the PLB */
  plb->setIdEvents(MQTTClientStation::idEvents);
                
  /* PLB handles events */
  plb->loop(MQTTClientStation::events);
}