#pragma once
#include <PubSubClient.h>


#define SUBSCRIBE_LAMP_MANUAL "/sub/smoker_temp/lamp_manual"
#define SUBSCRIBE_TURN_OFF "/sub/smoker_temp/turn_off"

class MQTT{

private:
    char cMqttPubValueTemperatureInside[10];
    char cMqttPubValueTemperatureOutside[10];
    char cMqttPubValueHumidityOutside[10];
    char cMqttPubStateLightOn[10];
public:
    MQTT();
    
    std::string getcMqttPubValueTemperatureInside[10];
    std::string getcMqttPubValueTemperatureOutside[10];
    std::string getcMqttPubValueHumidityOutside[10];
    std::string getcMqttPubStateLightOn[10];
    




};