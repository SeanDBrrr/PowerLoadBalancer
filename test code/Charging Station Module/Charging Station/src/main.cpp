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

int PlugPin = 0;
int PlugState = 0;

const int BUTTON_PIN = 4;
const int CONNECTION_LED = 2;
const int REQUEST_POWER_LED = 15;
const int DIRECTOR_LED = 26;
int counter = 0;

Button *button;
uint32_t id = 0;
DirectorScanner *director;
StationScreen *lcd;
bool check;

StartButton* startButton;
PlugButton* plugButton;
MQTTClientPLB* mqttStation;
ChargingStation* chargingStation;
// EspMQTTClient client1(name, password, broker_ip, mqtt_module, 1883);
// Director_RFID *director;

void onConnectionEstablished()
{
  mqttStation->onConnectionSubscribe();
}

void setup()
{
  button = new Button(BUTTON_PIN);
  lcd = new StationScreen(22, 21);
  director = new DirectorScanner(SS_PIN, RST_PIN);
  Serial.begin(115200);
  // button.begin();
  pinMode(CONNECTION_LED, OUTPUT);
  pinMode(REQUEST_POWER_LED, OUTPUT);
  pinMode(DIRECTOR_LED, OUTPUT);
  // button/*Plug*/ Plug(PlugState);
  // ChargingStation chargingStation(Plug);
  //LCD lcd(0x27,20,4);
  startButton = new StartButton();
  plugButton = new PlugButton();
  mqttStation = new MQTTClientPLB(1);
  chargingStation = new ChargingStation(
    startButton,
    plugButton,
    director,
    lcd,
    mqttStation
  );
}

  // put your main code here, to run repeatedly:
void loop() {
  chargingStation->HandleEvent(mqttStation->loop());
}