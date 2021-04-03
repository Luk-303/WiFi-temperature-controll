# WiFiThermometer-with-temperature-contoll

A simple IoT-Devices to controll the temperature in my smoker. I am using a Wemos D1 Mini, because with the ESP-8266 it´s possible to use a W-LAN connection to publish the data. In my personal case I measure the temperature inside of the Smoker with an DS18B20 temperature sensor and send it via WiFi and MQTT to a Raspberry with IOBroker and display it with Node-RED. If the temperature is too low, a Relais is switched and a 60 Watt light pulp is turned on to contoll the heat.
