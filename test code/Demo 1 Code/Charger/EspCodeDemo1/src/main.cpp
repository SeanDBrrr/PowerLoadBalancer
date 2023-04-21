#include <Arduino.h>
#include <string.h>
#include "EspMQTTClient.h"
 
const char *name = "Kiwy";
const char *password = "aquamagic23";
const char *mqtt_module = "Group4-Charger";
const char *mqtt_topic = "group4/RequestPower";
const char *mqtt_topic_receiver = "group4/SentPower";
const char *broker_ip = "192.168.61.23";

const int CONNECTION_LED = 2;
long previousTime = millis();

EspMQTTClient client1(name, password, broker_ip, mqtt_module, 1883);

void setup()
{
  Serial.begin(115200);
  pinMode(CONNECTION_LED,OUTPUT);
  client1.enableHTTPWebUpdater();
  client1.enableDebuggingMessages();
}

void onConnectionEstablished()
{
  client1.subscribe(mqtt_topic_receiver, [](const String & topic, const String & payload) {
      if(payload == "11kW")
      {
        digitalWrite(CONNECTION_LED, HIGH);
      }
  });
}

void loop()
{
  client1.loop();

  //IF BUTTON IS PRESSED
  client1.publish(mqtt_topic, "requestPower");
}