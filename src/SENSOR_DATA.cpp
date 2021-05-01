#include "SENSOR_DATA.h"


SENSOR_DATA::SENSOR_DATA(DallasTemperature& sensors,DHT& dht){
    measureOutsideTemperature(dht);
    measureInsideTemperature(sensors);
    measureOutsideHumidity(dht);
}

float SENSOR_DATA::getOutsideTemp(){return fOutsideTemp;}

float SENSOR_DATA::getInsideTemp(){return fInsideTemp;};

float SENSOR_DATA::getOutsideHum(){return fOutsideHumidity;};

float SENSOR_DATA::measureInsideTemperature(DallasTemperature& sensors){
    sensors.requestTemperatures();
    float fInsideTempBuff = sensors.getTempCByIndex(0);
    return fInsideTempBuff;
    }

float SENSOR_DATA::measureOutsideHumidity(DHT& dht){
    float fOutsideHumBuff=dht.readHumidity();
    delay(2000);
    return fOutsideHumBuff;
  }

float SENSOR_DATA::measureOutsideTemperature(DHT& dht){  
    float fOutdoorTempBuff=dht.readTemperature();
    delay(2000);
    return fOutdoorTempBuff;
}

void SENSOR_DATA::measureAllValues(DallasTemperature& sensors, DHT& dht){
    sensors.requestTemperatures();
    float fInsideTempBuff = sensors.getTempCByIndex(0);
    float fOutsideHumBuff=dht.readHumidity();
    delay(2000);
    float fOutdoorTempBuff=dht.readTemperature();
    delay(2000);
}
