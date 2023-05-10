#include <Arduino.h>
#include <string.h>
#include "EspMQTTClient.h"
#include "Button.h"
#include "LCD.h"
#include "Director_RFID.h"

#define BUTTON_PIN 4
#define CONNECTION_LED 2
#define REQUEST_POWER_LED 15
#define DIRECTOR_LED 26
int counter = 0;
long lastTime = 0;
const char *name = "Kiwy";
const char *password = "aquamagic23";
const char *mqtt_module = "Group4-Charger1";
const char *mqtt_topic = "group4/RequestPower";
const char *mqtt_topic_receiver = "group4/SentPower1";
const char *mqtt_topic_gate = "group4/gateUpdate";
const char *broker_ip = "192.168.193.23";

int dir = 1;

using namespace std;
long previousTime = millis();
Button button(BUTTON_PIN);

LCD *lcd;
EspMQTTClient client1(name, password, broker_ip, mqtt_module, 1883);
Director_RFID *director;
void setup()
{
  lcd = new LCD(22, 21);
  director = new Director_RFID(5, 27, 2);
  Serial.begin(115200);
  button.begin();
  pinMode(CONNECTION_LED, OUTPUT);
  pinMode(REQUEST_POWER_LED, OUTPUT);
  pinMode(DIRECTOR_LED, OUTPUT);
  client1.enableHTTPWebUpdater();
  client1.enableDebuggingMessages();
}

void onConnectionEstablished()
{
  client1.subscribe(mqtt_topic_receiver, [](const String &topic, const String &payload)
                    {
      if(payload == "Director:11kW")
      {
        digitalWrite(REQUEST_POWER_LED, HIGH);
        digitalWrite(DIRECTOR_LED, LOW);
        lcd->Display("DIRECTOR");
        delay(5000);
        digitalWrite(REQUEST_POWER_LED, LOW);
        digitalWrite(DIRECTOR_LED, HIGH);
        client1.publish(mqtt_topic_gate, "charged", 0);
        lcd->Display("");
      }
      else if(payload == "Director:5kW")
      {
        digitalWrite(REQUEST_POWER_LED, HIGH);
        digitalWrite(DIRECTOR_LED, LOW);
        lcd->Display("DIRECTOR");
        delay(10000);
        digitalWrite(REQUEST_POWER_LED, LOW);
        digitalWrite(DIRECTOR_LED, HIGH);
        client1.publish(mqtt_topic_gate, "charged", 0);
        lcd->Display("");
      }
      else if(payload == "User:11kW")
      {
        digitalWrite(REQUEST_POWER_LED, HIGH);
        digitalWrite(DIRECTOR_LED, LOW);
        lcd->Display("START");
        delay(5000);
        digitalWrite(REQUEST_POWER_LED, LOW);
        digitalWrite(DIRECTOR_LED, HIGH);
        client1.publish(mqtt_topic_gate, "charged", 0);
        lcd->Display("");
      }
      else if(payload == "User:5kW")
      {
        digitalWrite(REQUEST_POWER_LED, HIGH);
        digitalWrite(DIRECTOR_LED, LOW);
        lcd->Display("START");
        delay(10000);
        digitalWrite(REQUEST_POWER_LED, LOW);
        digitalWrite(DIRECTOR_LED, HIGH);
        client1.publish(mqtt_topic_gate, "charged", 0);
        lcd->Display("");
      } });
}

void loop()
{
  client1.loop();
  int directorr = director->GateState();
  if (directorr == 1)
  {
    client1.publish(mqtt_topic, "requestPower:Director:charger1", 0);
    digitalWrite(CONNECTION_LED, HIGH);
    lastTime = millis();
  }

  if(millis() - lastTime >= 2000)
  {
    digitalWrite(CONNECTION_LED, LOW);
  }

  if (button.singlePress())
  {
    client1.publish(mqtt_topic, "requestPower:User:charger1", 0);
  }
}