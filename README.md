# WiFi Thermometer-with-temperature-contoll

A selfmade IoT-Devices to controll and display the temperature in a smoker. I am using a Wemos D1 Mini, because with the ESP-8266 it´s possible to use a W-LAN connection to send and receive the data via MQTT. In my personal case I monitor and regulate the temperature inside and outside as well as the humidity of a smoker with an DS18B20 temperature sensor and a DHT21 sensor and send it via WiFi and MQTT to a RaspberryPi with IOBroker. The visualization takes place with VIS. If the temperature is too low, a Relais is switched and a 60 Watt light pulp is turned on to contoll the heat. It´s possible to turn the light pulp as the heat source manually on or off.



![image](https://user-images.githubusercontent.com/64526074/115380017-6aaf2780-a1d2-11eb-939c-5d40ce558f48.png)
