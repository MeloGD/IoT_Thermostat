# TFG: Dispositivo IoT a través del protocolo MQTT

## Español  
Trabajo final de universidad.

En este repositorio se muestra el código de un microncontrolador para un dispositivo IoT, 
concretamente, un termostato que recolecta información sobre temperatura/humedad y 
que además controla las condiciones climáticas en un terrario, usando una placa de 
desarrollo que integra el microcontrolador ESP32-S3.  

Dicho termostato hace uso del protocolo MQTT para conectarse a un servidor  
donde se encuentran herramientas como el proxy Traefik, el bróker MQTT Mosquitto,  
el gestor NodeRED, la BBDD InfluxDB y Grafana para la visualización de datos, 
todas desplegadas utilizando los contenedores de Docker.

En la carpeta "Docs" se encuentra la propia memoria de este proyecto que detalla su implementación
junto con la documentación del código realizada en Doxygen.  

Documentación de la placa de desarrllo usada:  
https://github.com/Makerfabs/Makerfabs-ESP32-S3-SPI-TFT-with-Touch


## English  
Last university project.

In this resository you will find the code for a microcontroller used for an IoT device, more 
specifically, a thermostat that gathers temperature/humidity data and manages the particular
environment requirments from a certain terrarium. All of this was possible thanks to a developer
board that integrates the ESP32-S3 microcontroller.

This thermostat uses the MQTT protocol in order to connect/communicate to a server which uses
Traefik as a proxy, Mosquitto as a broker, NodeRED as a data manager, InfluxDB as a BBDD and 
finally Grafana for the data visualization. All of this tools where deployed using Docker.

In the "Docs" folder you will find some docummentation about how this project was made, written
in spanish. 

Documentation about the developer board used:  
https://github.com/Makerfabs/Makerfabs-ESP32-S3-SPI-TFT-with-Touch
