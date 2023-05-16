#ifndef TOPICS_HPP
#define TOPICS_HPP

#include <string>

/* Building topics */
const char *mqtt_topic_solarPower = "group4/powerProduced";
const char *mqtt_topic_calculateSolarPower= "group4/calculateSolarPower";
const char *mqtt_topic_charge_building = "group4/chargeBuilding";

/* Station topics */
const char *mqtt_topic_mode = "group4/mode";
const char *mqtt_topic_StationId = "group4/StationId";
const char *mqtt_topic_directorId = "group4/directorId";
const char *mqtt_topic_requestSupply = "group4/requestSupply";
const char *mqtt_topic_charge_station = "group4/chargeStation";

#endif