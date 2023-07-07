/** 
 * @mainpage Documentación TFG: Dispositivo IoT a través del protocolo MQTT
 * @section intro_sec Introducción
 * En esta documentación se muestra como funciona el código de una aplicación \n
 * para un dispositivo IoT, concretamente, un termostato que controla las \n
 * condiciones climáticas en un terrario, usando una placa de desarrollo  \n
 * que integra el microcontrolador ESP32-S3. \n
 * Documentación de la placa de desarrllo: \n
 * https://github.com/Makerfabs/Makerfabs-ESP32-S3-SPI-TFT-with-Touch
 */

#include "UI/mcu_ui.h"
#include "RTOS/tasks.h"

/**
 * @brief Fichero principal
 * @file main.cpp
 * @author Jesús Carmelo González Domínguez
 * @date 14-07-2023
*/

/**
 * @brief Encargada de llamar a las funciones que realizan la
 * configuración inicial del microcontrolador.
 * @note Configura el serial, todos los pines GPIO utilizados, inicia la memoria flash,
 * inicia la GUI, crea las tareas del sistema e  inicia la comunicación WiFi-MQTT.
*/
void setup(void) {
  Serial.begin(115200);
  
  configButtonGPIO();
  configRelayGPIO();
  configDimmerGPIO();
  configSensorsGPIO();
  initFlashMemmory(); 
  configLVGLUI(); 
  initUITask();
  drawStartWiFiMenuScreen();
  createTasks();
  
  WiFi.onEvent(wifiEvent);
  configMQTT();
  connectWiFi();

  Serial.println( "Setup done" );
}

/**
 * @brief Encargada de llamar a las funciones que realizan la
 * configuración inicial del microcontrolador.
 * @note vTaskDelete(NULL), borra del task handler la función loop().
*/
void loop() {
  vTaskDelete(NULL);
}