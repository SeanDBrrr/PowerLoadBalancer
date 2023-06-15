// #include "ChargingStation.h"
// #include <iostream>
// ChargingStation::ChargingStation(
//     int id,
//     IStart *Start,
//     IPlug *Plug,
//     IDirector *Director,
//     IDisplay *Display,
//     IPLB *PLB)
//     : _directorId(0),
//       _id(id),
//       _powerRecieved(0),
//       _isStartedFlag(0),
//       _isPluggedFlag(0),
//       _isRfidAvailable(1),
//       _currentState(State::STATE_IDLE),
//       _wifiTrials(0),
//       _mqttTrials(0),
//       _IPLB(PLB),
//       _IPlug(Plug),
//       _IStart(Start),
//       _IDirector(Director),
//       _display(Display)
// {
//     _display->display("IDLE");
// }

// State ChargingStation::HandleIdleState(Event ev)
// {

//     State result = State::STATE_IDLE;

//     switch (ev)
//     {
//     case Event::EV_PLUGGED:
//         _display->display("ev plugged","PLUGGED");
//         result = State::STATE_PLUGGED;
//         break;
//     case Event::EV_RFID_DIRECTOR_DETECTED:
//         _isRfidAvailable = false;
//         result = State::STATE_VERIFYING_DIRECTOR;
//         break;
//     case Event::EV_WIFI_TRIALS:
//         _wifiTrials++;
//         _display->display("Connecting WiFi:", "Attempts: " + static_cast<String>(_wifiTrials));
//         break;
//     case Event::EV_MQTT_TRIALS:
//         _mqttTrials++;
//         _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
//         break;
//     case Event::EV_WIFI_NOT_CONNECTED:
//         _display->display("ERROR:", "WiFi Connection");
//         break;
//     case Event::EV_MQTT_NOT_CONNECTED:
//         _display->display("ERROR:", "MQTT Connection");
//         break;
//     case Event::EV_WIFI_CONNECTED:
//         _wifiTrials = 0;
//         _mqttTrials = 0;
//         _display->display("WIFI:", "Connected!");
//         break;
//     case Event::EV_MQTT_CONNECTED:
//         _mqttTrials = 0;
//         _display->display("MQTT:", "Connected");
//         break;
//     case Event::noEvent:
//         // do nothing
//         break;
//     }

//     return result;
// }

// State ChargingStation::HandleVerifyingDirectorState(Event ev)
// {
//     State result = State::STATE_VERIFYING_DIRECTOR;

//     switch (ev)
//     {
//     case Event::EV_RFID_VALID:
//         if (_isPluggedFlag)
//         {
//             _display->display("RFID valid", "PLUGGED DIRECTOR");
//             result = State::STATE_PLUGGED_DIRECTOR;
//         }
//         else if (!_isPluggedFlag)
//         {
//             _display->display("RFID valid", "IDLE DIRECTOR");
//             result = State::STATE_IDLE_DIRECTOR;
//         }
//         break;
//     case Event::EV_RFID_INVALID:
//         _isRfidAvailable = true;
//         if (_isPluggedFlag)
//         {
//             _display->display("RFID invalid", "PLUGGED");
//             result = State::STATE_PLUGGED;
//         }
//         else if (!_isPluggedFlag)
//         {
//             _display->display("RFID invalid", "IDLE");
//             result = State::STATE_IDLE;
//         }
//         break;
//     case Event::EV_RFID_TIMED_OUT:
//         _isRfidAvailable = true;
//         //Serial.println("IN EV TIMED OUT");
//         //Serial.println(_isPluggedFlag);
//         if (_isPluggedFlag)
//         {
//             //Serial.println("TIMED OUT PLUGGED");
//             _display->display("RFID timed out", "PLUGGED");
//             result = State::STATE_PLUGGED;
//         }
//         else if (!_isPluggedFlag)
//         {
//             //Serial.println("TIMED OUT IDLE");
//             _display->display("RFID timed out", "IDLE");
//             result = State::STATE_IDLE;
//         }
//         break;
//     case Event::EV_RFID_ALREADY_CHECKED_IN:
//         _isRfidAvailable = true;
//         if (_isPluggedFlag)
//         {
//             _display->display("alrdy chekd in","PLUGGED");
//             result = State::STATE_PLUGGED;
//         }
//         else if (!_isPluggedFlag)
//         {
//             _display->display("alrdy chekd in","IDLE");
//             result = State::STATE_IDLE;
//         }
//         break;
//         _display->display("Connecting WiFi:", "Attempts: " + static_cast<String>(_wifiTrials));
//         break;
//     case Event::EV_MQTT_TRIALS:
//         _mqttTrials++;
//         _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
//         break;
//     case Event::EV_WIFI_NOT_CONNECTED:
//         _display->display("ERROR:", "WiFi Connection");
//         break;
//     case Event::EV_MQTT_NOT_CONNECTED:
//         _display->display("ERROR:", "MQTT Connection");
//         break;
//     case Event::EV_WIFI_CONNECTED:
//         _wifiTrials = 0;
//         _mqttTrials = 0;
//         _display->display("WIFI:", "Connected!");
//         break;
//     case Event::EV_MQTT_CONNECTED:
//         _mqttTrials = 0;
//         _display->display("MQTT:", "Connected");
//         break;
//     case Event::noEvent:
//         // do nothing
//         break;
//     }

//     return result;
// }

// State ChargingStation::HandleIdleDirectorState(Event ev)
// {
//     State result = State::STATE_IDLE_DIRECTOR;

//     switch (ev)
//     {
//     case Event::EV_PLUGGED:
//         _display->display("ev plugged", "PLUGGED DIRECTOR");
//         result = State::STATE_PLUGGED_DIRECTOR;
//         break;
//         _display->display("Connecting WiFi:", "Attempts: " + static_cast<String>(_wifiTrials));
//         break;
//     case Event::EV_MQTT_TRIALS:
//         _mqttTrials++;
//         _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
//         break;
//     case Event::EV_WIFI_NOT_CONNECTED:
//         _display->display("ERROR:", "WiFi Connection");
//         break;
//     case Event::EV_MQTT_NOT_CONNECTED:
//         _display->display("ERROR:", "MQTT Connection");
//         break;
//     case Event::EV_WIFI_CONNECTED:
//         _wifiTrials = 0;
//         _mqttTrials = 0;
//         _display->display("WIFI:", "Connected!");
//         break;
//     case Event::EV_MQTT_CONNECTED:
//         _mqttTrials = 0;
//         _display->display("MQTT:", "Connected");
//         break;
//     case Event::noEvent:
//         // do nothing
//         break;
//     }

//     return result;
// }

// State ChargingStation::HandlePluggedState(Event ev)
// {

//     State result = State::STATE_PLUGGED;

//     switch (ev)
//     {
//     case Event::EV_UNPLUGGED:
//         _display->display("ev unplugged", "IDLE");
//         result = State::STATE_IDLE;
//         break;
//     case Event::EV_RFID_DIRECTOR_DETECTED:
//         _isRfidAvailable = false;
//         result = State::STATE_VERIFYING_DIRECTOR;
//         break;
//     case Event::EV_START:
//         _isRfidAvailable = false;
//         _display->display("ev start", "WAITING ON POWER");
//         result = State::STATE_WAITING_FOR_POWER;
//         requestPower();
//         break;
//     case Event::EV_MQTT_TRIALS:
//         _mqttTrials++;
//         _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
//         break;
//     case Event::EV_WIFI_NOT_CONNECTED:
//         _display->display("ERROR:", "WiFi Connection");
//         break;
//     case Event::EV_MQTT_NOT_CONNECTED:
//         _display->display("ERROR:", "MQTT Connection");
//         break;
//     case Event::EV_WIFI_CONNECTED:
//         _wifiTrials = 0;
//         _mqttTrials = 0;
//         _display->display("WIFI:", "Connected!");
//         break;
//     case Event::EV_MQTT_CONNECTED:
//         _mqttTrials = 0;
//         _display->display("MQTT:", "Connected");
//         break;
//     case Event::noEvent:
//         // do nothing
//         break;
//     }

//     return result;
// }

// State ChargingStation::HandlePluggedDirectorState(Event ev)
// {
//     State result = State::STATE_PLUGGED_DIRECTOR;

//     switch (ev)
//     {
//     case Event::EV_UNPLUGGED:
//         _display->display("ev unplugged", "IDLE DIRECTOR");
//         result = State::STATE_IDLE_DIRECTOR;
//         break;
//     case Event::EV_RFID_INVALID:
//         _display->display("RFID invalid", "PLUGGED");
//         result = State::STATE_PLUGGED;
//         break;
//     case Event::EV_START:
//         _isRfidAvailable = false;
//         _display->display("ev start", "WAITING ON POWER");
//         result = State::STATE_WAITING_FOR_POWER;
//         requestPower();
//         break;
//     case Event::EV_MQTT_TRIALS:
//         _mqttTrials++;
//         _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
//         break;
//     case Event::EV_WIFI_NOT_CONNECTED:
//         _display->display("ERROR:", "WiFi Connection");
//         break;
//     case Event::EV_MQTT_NOT_CONNECTED:
//         _display->display("ERROR:", "MQTT Connection");
//         break;
//     case Event::EV_WIFI_CONNECTED:
//         _wifiTrials = 0;
//         _mqttTrials = 0;
//         _display->display("WIFI:", "Connected!");
//         break;
//     case Event::EV_MQTT_CONNECTED:
//         _mqttTrials = 0;
//         _display->display("MQTT:", "Connected");
//         break;
//     case Event::noEvent:
//         // do nothing
//         break;
//     }

//     return result;
// }

// State ChargingStation::HandleWaitingForPowerState(Event ev)
// {
//     State result = State::STATE_WAITING_FOR_POWER;

//     switch (ev)
//     {
//     case Event::EV_CHARGING:
//         result = State::STATE_CHARGING;
//         break;
//     case Event::EV_STOP:
//         //Serial.println("SHOULDA STOPPED CHARGING");
//         _IPLB->stopSupplyToStation(_id);
//         _display->display("ev stop", "STOPPED CHARGING");
//         result = State::STATE_STOPPED_CHARGING;
//         break;
//     case Event::EV_MQTT_TRIALS:
//         _mqttTrials++;
//         _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
//         break;
//     case Event::EV_WIFI_NOT_CONNECTED:
//         _display->display("ERROR:", "WiFi Connection");
//         break;
//     case Event::EV_MQTT_NOT_CONNECTED:
//         _display->display("ERROR:", "MQTT Connection");
//         break;
//     case Event::EV_WIFI_CONNECTED:
//         _wifiTrials = 0;
//         _mqttTrials = 0;
//         _display->display("WIFI:", "Connected!");
//         break;
//     case Event::EV_MQTT_CONNECTED:
//         _mqttTrials = 0;
//         _display->display("MQTT:", "Connected");
//         break;
//     case Event::noEvent:
//         // do nothing
//         break;
//     }

//     return result;
// }

// State ChargingStation::HandleChargingState(Event ev)
// {
//     State result = State::STATE_CHARGING;

//     switch (ev)
//     {
//     case Event::EV_STOP:
//         _IPLB->stopSupplyToStation(_id);
//         _display->display("ev stop", "STOPPED CHARGING");
//         result = State::STATE_STOPPED_CHARGING;
//         break;
//     case Event::EV_ERROR:
//         _display->display("ERROR");
//         result = State::STATE_ERROR;
//         break;
//     case Event::EV_MODE_CHANGED_DIRECTOR:
//         _display->display("Mode: Director");
//         break;
//     case Event::EV_MODE_CHANGED_FCFS:
//         _display->display("Mode: FCFS");
//         break;
//     case Event::EV_MODE_CHANGED_DYNAMIC:
//         _display->display("Mode: Dynamic");
//         break;
//     case Event::EV_MQTT_TRIALS:
//         _mqttTrials++;
//         _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
//         break;
//     case Event::EV_WIFI_NOT_CONNECTED:
//         _display->display("ERROR:", "WiFi Connection");
//         break;
//     case Event::EV_MQTT_NOT_CONNECTED:
//         _display->display("ERROR:", "MQTT Connection");
//         break;
//     case Event::EV_WIFI_CONNECTED:
//         _wifiTrials = 0;
//         _mqttTrials = 0;
//         _display->display("WIFI:", "Connected!");
//         break;
//     case Event::EV_MQTT_CONNECTED:
//         _mqttTrials = 0;
//         _display->display("MQTT:", "Connected");
//         break;
//     case Event::noEvent:
//         // do nothing
//         break;
//     }

//     return result;
// }

// State ChargingStation::HandleStoppedChargingState(Event ev)
// {
//     State result = State::STATE_STOPPED_CHARGING;

//     switch (ev)
//     {
//     case Event::EV_START:
//         _isRfidAvailable = false;
//         Serial.println("stp charging, requesting");
//         _display->display("ev start", "WAITING ON POWER");
//         result = State::STATE_WAITING_FOR_POWER;
//         requestPower();
//         break;
//     case Event::EV_UNPLUGGED:
//         _isRfidAvailable = true;
//         _display->display("ev unplugged", "IDLE");
//         result = State::STATE_IDLE;
//         break;
//     case Event::EV_MQTT_TRIALS:
//         _mqttTrials++;
//         _display->display("Connecting MQTT:", "Attempts: " + static_cast<String>(_mqttTrials));
//         break;
//     case Event::EV_WIFI_NOT_CONNECTED:
//         _display->display("ERROR:", "WiFi Connection");
//         break;
//     case Event::EV_MQTT_NOT_CONNECTED:
//         _display->display("ERROR:", "MQTT Connection");
//         break;
//     case Event::EV_WIFI_CONNECTED:
//         _wifiTrials = 0;
//         _mqttTrials = 0;
//         _display->display("WIFI:", "Connected!");
//         break;
//     case Event::EV_MQTT_CONNECTED:
//         _mqttTrials = 0;
//         _display->display("MQTT:", "Connected");
//         break;
//     case Event::noEvent:
//         // do nothing
//         break;
//     }

//     return result;
// }

// State ChargingStation::HandleErrorState(Event ev)
// {
//     _IPLB->stopSupplyToStation(_id);
//     return State::STATE_STOPPED_CHARGING;
// }

// void ChargingStation::HandleMainEvent(Event ev) // Technically might not be needed unless there are more states
// {
//     switch (_currentState)
//     {
//     case State::MAIN_STATE_WORKING:
//         _currentState = HandleMainWorkingState(ev);
//         break;
//     case State::MAIN_STATE_ERROR:
//         _currentState = HandleMainErrorState(ev);
//         break;
//     default:
//         throw std::runtime_error("ERROR: illegal/unhandled state with number");
//         break;
//     }
// }

// void ChargingStation::HandleEvent(Event ev) // can technically just call private variable _currentEvent, avoided for now in fear of issues.
// {
//     switch (_currentState)
//     {
//     case State::STATE_IDLE:
//         _currentState = HandleIdleState(ev);
//         break;
//     case State::STATE_VERIFYING_DIRECTOR:
//         _currentState = HandleVerifyingDirectorState(ev);
//         break;
//     case State::STATE_IDLE_DIRECTOR:
//         _currentState = HandleIdleDirectorState(ev);
//         break;
//     case State::STATE_PLUGGED:
//         _currentState = HandlePluggedState(ev);
//         break;
//     case State::STATE_PLUGGED_DIRECTOR:
//         _currentState = HandlePluggedDirectorState(ev);
//         break;
//     case State::STATE_WAITING_FOR_POWER:
//         _currentState = HandleWaitingForPowerState(ev);
//         break;
//     case State::STATE_CHARGING:
//         _currentState = HandleChargingState(ev);
//         break;
//     case State::STATE_STOPPED_CHARGING:
//         _currentState = HandleStoppedChargingState(ev);
//         break;
//     case State::STATE_ERROR:
//         _currentState = HandleErrorState(ev);
//         break;
//     default:
//         throw std::runtime_error("ERROR: illegal/unhandled state with number");
//         break;
//     }
// }

// void ChargingStation::requestPower() // MAKE VOID WHEN DONE
// {
//     _IPLB->supplyPowerToStation(_id);
//     unsigned long lastTime = millis();
//     while (!_IPLB->getPowerReceievedFlag()) // PLB
//     {
//         _IPLB->callClientLoop(); // PLB

//         if (_isStartedFlag)
//         {
//             if (_IStart->isStarted()) // STARTBUTTON
//             {
//                 _isStartedFlag = false;
//                 _display->display("ev stop", "STOPPED CHARGING");
//                 //_currentEvents.emplace_back(Event::EV_STOP);
//                 _currentState = State::STATE_STOPPED_CHARGING;
//                 Serial.println("STOPPED");
//                 return;
//             }
//         }

//         if (millis() - lastTime >= 5000)
//         {
//             _IPLB->supplyPowerToStation(_id); //_IPLB
//             Serial.println("Requesting Power");
//             lastTime = millis();
//         }
//     }
//     //Serial.println("EXIT WHILE");
//     _IPLB->SetPowerRecievedFlag(false); // PLB
//     _currentEvents.emplace_back(Event::EV_CHARGING);
//     return;
// }

// void ChargingStation::loop(Event ev)
// {
//  if (!_isStartedFlag)
//     {
//         if (_IStart->isStarted())
//         {
//             Serial.println("STARTED BUTTON");
//             _isStartedFlag = true;
//             _currentEvents.emplace_back(Event::EV_START);
//         }
//     }
//     else if (_isStartedFlag)
//     {
//         if (_IStart->isStarted())
//         {
//             Serial.println("STOPPED BUTTON");
//             _isStartedFlag = false;
//             _currentEvents.emplace_back(Event::EV_STOP);
//             //_currentEvent = Event::EV_STOP;
//         }
//     }

//     if (_currentState == State::STATE_WAITING_FOR_POWER && _isStartedFlag == false)
//     {
//         _currentEvents.emplace_back(Event::EV_STOP);
//     }
    

//     if (!_isPluggedFlag)
//     {
//         if (_IPlug->isPlugged())
//         {
//             _isPluggedFlag = true;
//             _currentEvents.emplace_back(Event::EV_PLUGGED);
//         }
//     }
//     else if (_isPluggedFlag)
//     {
//         if (_IPlug->isPlugged())
//         {
//             _isPluggedFlag = false;
//             _currentEvents.emplace_back(Event::EV_UNPLUGGED);
//         }
//     }



//     if (_currentState == State::STATE_IDLE || _currentState == State::STATE_PLUGGED)
//     {
//         _directorId = _IDirector->getID();
//     }
//     else
//     {
//         _directorId = 0;
//     }

//     if (_directorId != 0)
//     {
//         _IPLB->checkDirector(_directorId);
//         _display->display("VERIFYING","DIRECTOR");
//         //Serial.println("call dir timeout");
//         _IPLB->directorTimeout(3000);
//         //Serial.println("AFTER call dir timeout");
//         _isRfidAvailable = false;
//         _currentEvents.emplace_back(Event::EV_RFID_DIRECTOR_DETECTED);
//     }

//     static float lastPower = 0;
//     _powerRecieved = _IPLB->getPowerReceived();
//     if (_powerRecieved != lastPower)
//     {
//         _display->display("Charging: " + static_cast<String>(_powerRecieved));
//         lastPower = _powerRecieved;
//         _currentEvents.emplace_back(Event::EV_CHARGING);//technically shouldnt always call this event
//         //because if it receives 0 power when it stops charging, it will call EV_CHARGING for no reason.
//     }

//     _currentEvents.emplace_back(ev); // needed for the events recieved from the PLB

//     try
//     {
//         for (const auto &currentEvent : _currentEvents)
//         {
//             HandleEvent(currentEvent);
//         }
//         _currentEvents.clear();
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Exception occurred: " << e.what() << std::endl;
//     }
// }
