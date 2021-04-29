#include "SensorData.h"


SensorData::SensorData(DallasTemperature& sensors,DHT& dht){
    measureOutsideTemperature(dht);
    measureInsideTemperature(sensors);
    measureOutsideHumidity(dht);
}

float SensorData::getOutsideTemp(){return fOutsideTemp;}

float SensorData::getInsideTemp(){return fInsideTemp;};

float SensorData::getOutsideHum(){return fOutsideHumidity;};

float SensorData::measureInsideTemperature(DallasTemperature& sensors){
    sensors.requestTemperatures();
    float fInsideTempBuff = sensors.getTempCByIndex(0);
    return fInsideTempBuff;
    }

float SensorData::measureOutsideHumidity(DHT& dht){
    float fOutsideHumBuff=dht.readHumidity();
    delay(2000);
    return fOutsideHumBuff;
  }

float SensorData::measureOutsideTemperature(DHT& dht){  
    float fOutdoorTempBuff=dht.readTemperature();
    delay(2000);
    return fOutdoorTempBuff;
}

