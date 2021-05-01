#pragma once
#include <DHT.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Adafruit_Sensor.h>

#define ONE_WIRE_BUS D4
#define DHT_SENSOR D5

class SENSOR_DATA{
private:
    float fOutsideTemp;
    float fInsideTemp;
    float fOutsideHumidity;
public:
    SENSOR_DATA(DallasTemperature&,DHT&);

    float getOutsideTemp();
    float getInsideTemp();
    float getOutsideHum();

    float measureOutsideTemperature(DHT&);
    float measureInsideTemperature(DallasTemperature&);
    float measureOutsideHumidity(DHT&);

    void measureAllValues(DallasTemperature&,DHT&);

};
