#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "SENSOR_DATA.h"
#include "MQTT_PUB.h"

#define SUBSCRIBE_LAMP_MANUAL "/sub/smoker_temp/lamp_manual"
#define SUBSCRIBE_TURN_OFF "/sub/smoker_temp/turn_off"
#define SWITCH_PIN D1

OneWire objOneWire(ONE_WIRE_BUS);
DallasTemperature objSensors(&objOneWire);
DHT objDHT(DHT_SENSOR,DHT21);
WiFiClient objWemosD1;
PubSubClient objClient(objWemosD1);
SENSOR_DATA objSensorData(objSensors,objDHT);
MQTT_PUB objMQTT(objSensorData);

const char* SSID = "xxxxxxxxxxxxx";
const char* PASSWORD = "xxxxxxxxxxxxxx";
const char* MQTT_BROKER = "xxxxxxxxxxxxxxxxxx";

unsigned long nTimestamp;
unsigned long nPeriod = 5000;

bool bLightOn=false;
bool bLightManual=false;
bool bTurnOFF=false;

void sendToMQTTBroker();
void heatUP();
float measureTemperatureOutside();
float measureTemperatureInside();
float measureHumidityOutside();
void manualMode(char*, byte*, unsigned int);

void heatUp(){

  while(objSensorData.measureInsideTemperature(objSensors)<13.0){
    digitalWrite(SWITCH_PIN,HIGH);
    objMQTT.sendValuesToBroker(objWemosD1,objClient);
    bLightOn=true;
  }
  digitalWrite(SWITCH_PIN, LOW);
  bLightOn=false;
}

void setUpWLan(){
        
    WiFi.begin(SSID,PASSWORD);
    nTimestamp=millis();
        while ((millis() - nTimestamp <= nPeriod)&&WiFi.status()!=WL_CONNECTED){
        yield();
    }
}

void reconnect() {

  long nTimestamp2=millis();

  while (!objClient.connected()&&(millis() - nTimestamp2 <= nPeriod)) {

    if (objClient.connect("/dev/smoker_temp")) {
      objClient.subscribe(SUBSCRIBE_LAMP_MANUAL);
      objClient.subscribe(SUBSCRIBE_TURN_OFF);
    } else {
      delay(100); 
    }
    yield();
  }
}

void manualMode(char* topic, byte* message, unsigned int length){

  String sMessageBuff;

  for(unsigned int i=0;i<length;i++){
    sMessageBuff += ((char)message[i]);
  }  

  if(sMessageBuff=="false"){
    digitalWrite(SWITCH_PIN,LOW);
    bLightOn=false;
    bLightManual=false;
    delay(200);
  }
  else if(sMessageBuff=="true"){
    digitalWrite(SWITCH_PIN,HIGH);
    bLightOn=true;
    bLightManual=true;
    delay(200);
  }
}

void callback(char* topic,byte* message,unsigned int length){

  String sMessageBuff;

    for(unsigned int i=0;i<length;i++){
    sMessageBuff += ((char)message[i]);
    } 

    if (String(topic)==SUBSCRIBE_LAMP_MANUAL){
      manualMode(topic,message,length);
    }
    else if (String(topic)==SUBSCRIBE_TURN_OFF&&sMessageBuff=="1"){
      objMQTT.setTurnOFF(true);
      objMQTT.sendIdleToBroker(objWemosD1,objClient);
    }
}

void setup(void)
{
    Serial.begin (9600);
    objSensors.begin();

    objDHT.begin();

    pinMode(SWITCH_PIN,OUTPUT);
    
    setUpWLan();
    
    objClient.setServer(MQTT_BROKER, 1883);
    objClient.setCallback(callback);
}

void loop(void){

  objSensorData.measureAllValues(objSensors, objDHT);

  float fTempInside = objSensorData.getInsideTemp();
  
  if (!objClient.connected()) {
    reconnect();
  }
  objClient.loop();
  objMQTT.sendValuesToBroker(objWemosD1, objClient);
  if(fTempInside<=10.0){
    heatUp();
  }
  else if (fTempInside>10.0&&bLightManual==false){
    digitalWrite(SWITCH_PIN,LOW);
  }
  else if (bLightManual==true){
    delay(20);
  }
}


