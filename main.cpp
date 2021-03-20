#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const char* SSID = "xxxxxxxxxxxxxxxxxxxxxxx";
const char* PASSWORD = "xxxxxxxxxxxxxxxxxxxxxxx";
const char* MQTT_BROKER = "xxxxxxxxxxxxxxxxxxxxxxxx";

const int SWITCH_PIN = D4;

bool rightTemperature = false;
unsigned long timestamp;
unsigned long period = 5000;

WiFiClient WemosD1;
PubSubClient client(WemosD1);

char mqtt_pub_value_temperature[50];

void setUpWLan(){
        
    WiFi.begin(SSID,PASSWORD); 

    timestamp=millis();

        while ((millis() - timestamp <= period)&&WiFi.status()!=WL_CONNECTED){
        yield();
    }
}

float measureTemperature(){

    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);

    return temp;
}

void reconnect() {

  long timestamp2=millis();

  while (!client.connected()&&(millis() - timestamp2 <= period)) {
       //Verbindungsversuch:
    if (client.connect("/dev/smoker_temp")) {
    } else {
      delay(100); 
    }
    yield();
  }
}

void sendToMQTTBroker(){

    String tempBuff;
    tempBuff = String(measureTemperature());
    tempBuff.toCharArray(mqtt_pub_value_temperature,tempBuff.length()+1);
    client.publish("/dev/smoker_temp", mqtt_pub_value_temperature,true);
}
void heatUp(){

    while(measureTemperature()<30.0){
      digitalWrite(SWITCH_PIN,LOW);
      sendToMQTTBroker();
    }
    digitalWrite(SWITCH_PIN, HIGH);
    rightTemperature=true;
}

void setup(void)
{
    Serial.begin (9600);
    sensors.begin();
    pinMode(SWITCH_PIN,OUTPUT);

    setUpWLan();
    client.setServer(MQTT_BROKER, 1883);
}

void loop(void){
    
    if (!client.connected()) {
     reconnect();
    }
    sendToMQTTBroker();

    if(measureTemperature()<=30.0){
      heatUp();
    }
    delay(1000);
   // ESP.deepSleep(4260000000);
   // delay(100);
}


