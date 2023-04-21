#include <Arduino.h>
#include <string.h>
//#include <ESP32Servo.h>
#include "EspMQTTClient.h"
 
const char *name = "Kiwy";
const char *password = "aquamagic23";
const char *mqtt_module = "Group4-Charger2";
const char *mqtt_topic = "group4/esp2";
const char *mqtt_topic_receiver = "group4/esp";
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
  digitalWrite(CONNECTION_LED, HIGH);
  client1.subscribe(mqtt_topic_receiver, [](const String & topic, const String & payload) {
    Serial.println(payload);
  });
}

void loop()
{
  client1.loop();

  if(!client1.isConnected())
  {
    digitalWrite(CONNECTION_LED, LOW);
  }

  if(millis() - previousTime >= 5000)
  {
    client1.publish(mqtt_topic, "Hello", 0);
    previousTime = millis();
  }
}