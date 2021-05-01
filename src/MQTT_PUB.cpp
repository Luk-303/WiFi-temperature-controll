#include "MQTT_PUB.h"

MQTT_PUB::MQTT_PUB(SENSOR_DATA& objSensor_Data){
    sTempInsideBuff=objSensor_Data.getInsideTemp();
    sTempOutsideBuff=objSensor_Data.getOutsideTemp();
    sHumOutsideBuff=objSensor_Data.getOutsideHum();
    sStateLightBuff = "false";
}

void MQTT_PUB::sendValuesToBroker(WiFiClient& WemosD1, PubSubClient& client){

    sTempInsideBuff.toCharArray(cMqttPubValueTemperatureInside,sTempInsideBuff.length()+1);
    client.publish("/pub/smoker_temp/inside",cMqttPubValueTemperatureInside,true);

    sTempOutsideBuff.toCharArray(cMqttPubValueTemperatureOutside,sTempOutsideBuff.length()+1);
    client.publish("/pub/smoker_temp/outside",cMqttPubValueTemperatureOutside,true);

    sHumOutsideBuff.toCharArray(cMqttPubValueHumidityOutside,sHumOutsideBuff.length()+1);
    client.publish("/pub/smoker_temp/humidity",cMqttPubValueHumidityOutside,true);

    sStateLightBuff.toCharArray(cMqttPubStateLight,sStateLightBuff.length()+1);
    client.publish("/pub/smoker_temp/light",cMqttPubStateLight,true);
}

void MQTT_PUB::sendIdleToBroker(WiFiClient& WemosD1, PubSubClient& client){
    sTempInsideBuff = "--,-";
    sTempInsideBuff.toCharArray(cMqttPubValueTemperatureInside,sTempInsideBuff.length()+1);
    client.publish("/pub/smoker_temp/inside", cMqttPubValueTemperatureInside,true);

    sTempOutsideBuff = "--,-";
    sTempOutsideBuff.toCharArray(cMqttPubValueTemperatureOutside,sTempOutsideBuff.length()+1);
    client.publish("/pub/smoker_temp/outside",cMqttPubValueTemperatureOutside,true);

    sHumOutsideBuff = "--,-";
    sHumOutsideBuff.toCharArray(cMqttPubValueHumidityOutside,sHumOutsideBuff.length()+1);
    client.publish("/pub/smoker_temp/humidity",cMqttPubValueHumidityOutside,true);

    sStateLightBuff="0";
    sStateLightBuff.toCharArray(cMqttPubStateLight,sStateLightBuff.length()+1);
    client.publish("/pub/smoker_temp/light",cMqttPubStateLight,true);
}

void MQTT_PUB::setTempInsideBuff(SENSOR_DATA& objSensor_Data){
    sTempInsideBuff=objSensor_Data.getInsideTemp();
}
void MQTT_PUB::setTempOutsideBuff(SENSOR_DATA& objSensor_Data){ 
    sTempOutsideBuff=objSensor_Data.getOutsideTemp();
}
void MQTT_PUB::setHumOutsideBuff(SENSOR_DATA& objSensor_Data){   
    sHumOutsideBuff=objSensor_Data.getOutsideHum();
}
void MQTT_PUB::setTurnOFF(bool bTurnOFF){
    this->bTurnOFF=bTurnOFF;
}

String MQTT_PUB::getLightOnBuff(){return sStateLightBuff;}
String MQTT_PUB::getTempInsideBuff(){return sTempInsideBuff;}
String MQTT_PUB::getTempOutsideBuff(){return sTempOutsideBuff;}
String MQTT_PUB::getHumOutsideBuff(){return sHumOutsideBuff;}
bool MQTT_PUB::getTurnOFF(){return bTurnOFF;}