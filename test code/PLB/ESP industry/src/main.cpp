#include <Arduino.h>
#include <string.h>
#include "PLB.h"
#include "MQTTServer.h"

MQTTServer mqtt;
PLB plb(mqtt, mqtt);
Events event;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  mqtt.receive();
  plb.manageEvents(event);
  event = noEvent;
  if(mqtt.getSupplyRequest() == true)
  {
    switch (mqtt.getStationId())
    {
    case 1:
      event = supply1;
      break;
    case 2:
      event = supply2;
      break;
    case 3:
      event = supply3;
      break;
    case 4:
      event = supply4;
      break;
    }
    mqtt.setSupplyRequest();
  }
}