#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>


#define ONE_WIRE_BUS D4
#define SUBSCRIBE_LAMP_MANUAL "/sub/smoker_temp/lamp_manual"
#define SUBSCRIBE_TURN_OFF "/sub/smoker_temp/turn_off"
#define DHT_SENSOR D5
#define SWITCH_PIN D1

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DHT dht(DHT_SENSOR,DHT21);
const char* SSID = "xxxxxx";
const char* PASSWORD = "xxxxxxx";
const char* MQTT_BROKER = "xxxxxx";

unsigned long timestamp;
unsigned long period = 5000;

bool lightOn=false;
bool lightManual=false;
bool turnOFF=false;

WiFiClient WemosD1;
PubSubClient client(WemosD1);


char mqtt_pub_value_temperature_inside[10];
char mqtt_pub_value_temperature_outside[10];
char mqtt_pub_value_humidity_outside[10];
char mqtt_pub_state_light_on[10];

void sendToMQTTBroker();
void heatUP();
float measureTemperatureOutside();
float measureTemperatureInside();
float measureHumidityOutside();
void manualMode(char*, byte*, unsigned int);



void heatUp(){


    while(measureTemperatureInside()<13.0){
      digitalWrite(SWITCH_PIN,HIGH);
      sendToMQTTBroker();
      lightOn=true;
    }
    digitalWrite(SWITCH_PIN, LOW);
    lightOn=false;
}

void sendToMQTTBroker(){

    String tempInsideBuff;
    String tempOutsideBuff;
    String humOutsideBuff;
    String stateLightBuff;

  if (turnOFF==true){
    tempInsideBuff = "--,-";
    tempInsideBuff.toCharArray(mqtt_pub_value_temperature_inside,tempInsideBuff.length()+1);
    client.publish("/pub/smoker_temp/inside", mqtt_pub_value_temperature_inside,true);

    tempOutsideBuff = "--,-";
    tempOutsideBuff.toCharArray(mqtt_pub_value_temperature_outside,tempOutsideBuff.length()+1);
    client.publish("/pub/smoker_temp/outside",mqtt_pub_value_temperature_outside,true);

    Serial.println(" hier müsste die temp gesendet werden ");

    humOutsideBuff = "--,-";
    humOutsideBuff.toCharArray(mqtt_pub_value_humidity_outside,humOutsideBuff.length()+1);
    client.publish("/pub/smoker_temp/humidity",mqtt_pub_value_humidity_outside,true);

    stateLightBuff="0";
    stateLightBuff.toCharArray(mqtt_pub_state_light_on,stateLightBuff.length()+1);
    client.publish("/pub/smoker_temp/light",mqtt_pub_state_light_on,true);
  }
  else {
    tempInsideBuff = String(measureTemperatureInside());
    tempInsideBuff.toCharArray(mqtt_pub_value_temperature_inside,tempInsideBuff.length()+1);
    client.publish("/pub/smoker_temp/inside", mqtt_pub_value_temperature_inside,true);

    tempOutsideBuff = String(measureTemperatureOutside());
    tempOutsideBuff.toCharArray(mqtt_pub_value_temperature_outside,tempOutsideBuff.length()+1);
    client.publish("/pub/smoker_temp/outside",mqtt_pub_value_temperature_outside,true);

    Serial.println(" hier müsste die temp gesendet werden ");

    humOutsideBuff = String(measureHumidityOutside());
    humOutsideBuff.toCharArray(mqtt_pub_value_humidity_outside,humOutsideBuff.length()+1);
    client.publish("/pub/smoker_temp/humidity",mqtt_pub_value_humidity_outside,true);

    stateLightBuff=String(lightOn);
    stateLightBuff.toCharArray(mqtt_pub_state_light_on,stateLightBuff.length()+1);
    client.publish("/pub/smoker_temp/light",mqtt_pub_state_light_on,true);

  }
}

void setUpWLan(){
        
    WiFi.begin(SSID,PASSWORD);
    timestamp=millis();
        while ((millis() - timestamp <= period)&&WiFi.status()!=WL_CONNECTED){
        yield();
    }
}

void reconnect() {

  long timestamp2=millis();

  while (!client.connected()&&(millis() - timestamp2 <= period)) {

    if (client.connect("/dev/smoker_temp")) {
      client.subscribe(SUBSCRIBE_LAMP_MANUAL);
      client.subscribe(SUBSCRIBE_TURN_OFF);
    } else {
      delay(100); 
    }
    yield();
  }
}

void manualMode(char* topic, byte* message, unsigned int length){

    String messageBuff;

    for(unsigned int i=0;i<length;i++){
      messageBuff += ((char)message[i]);
    }  
      if(messageBuff=="false"){
        digitalWrite(SWITCH_PIN,LOW);
        lightOn=false;
        lightManual=false;
        delay(200);
      }
      else if(messageBuff=="true"){
        digitalWrite(SWITCH_PIN,HIGH);
        lightOn=true;
        lightManual=true;
        delay(200);
      }
}

void callback(char* topic,byte* message,unsigned int length){
    String messageBuff;

      for(unsigned int i=0;i<length;i++){
      Serial.print((char)message[i]);
      messageBuff += ((char)message[i]);
    } 

    if (String(topic)==SUBSCRIBE_LAMP_MANUAL){
      manualMode(topic,message,length);
    }
    else if (String(topic)==SUBSCRIBE_TURN_OFF){
      if (messageBuff=="1"){
      turnOFF=true;
      sendToMQTTBroker();
      }
    }
}

void setup(void)
{
    Serial.begin (9600);
    sensors.begin();

    dht.begin();

    pinMode(SWITCH_PIN,OUTPUT);
    
    setUpWLan();
    
    client.setServer(MQTT_BROKER, 1883);
    client.setCallback(callback);
}

void loop(void){

  float tempInside = measureTemperatureInside();
    
    if (!client.connected()) {
     reconnect();
    }

    client.loop();

    sendToMQTTBroker();

    if(tempInside<=10.0){
     heatUp();
    }
    else if (tempInside>10.0&&lightManual==false){
      digitalWrite(SWITCH_PIN,LOW);
    }
    else if (lightManual==true){
      delay(20);
    }

}


