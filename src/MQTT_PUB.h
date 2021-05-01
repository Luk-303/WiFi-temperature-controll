#pragma once
#include "SENSOR_DATA.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>


class MQTT_PUB{

private:
    String sTempInsideBuff;
    String sTempOutsideBuff;
    String sHumOutsideBuff;
    String sStateLightBuff;

    char cMqttPubValueTemperatureInside[10];
    char cMqttPubValueTemperatureOutside[10];
    char cMqttPubValueHumidityOutside[10];
    char cMqttPubStateLight[10];

    bool bTurnOFF;
public:
    MQTT_PUB(SENSOR_DATA&);

    void setLightOnBuff();
    void setTempInsideBuff(SENSOR_DATA&);
    void setTempOutsideBuff(SENSOR_DATA&);
    void setHumOutsideBuff(SENSOR_DATA&);
    void setTurnOFF(bool);

    String getTempInsideBuff();
    String getTempOutsideBuff();
    String getHumOutsideBuff();
    String getLightOnBuff();
    bool getTurnOFF();

    void sendValuesToBroker(WiFiClient&, PubSubClient&);
    void sendIdleToBroker(WiFiClient&, PubSubClient&);
};