/**
 * @brief Fichero que contiene las definiciones de las funciones que controlan
 * los relés y el dimmer/regulador de potencia.
 * @file actuators.h
 * @author Jesús Carmelo González Domínguez
 * @date 14-07-2023
*/

#ifndef ACTUATORS_GUARD
#define ACTUATORS_GUARD

#include "Devices/rtc.h"
#include "Devices/sensor.h"
#include "UI/mcu_ui.h"

/**
 * @brief Pin del canal del relé que controla el bombillo UVB.
*/
#define UVB_RELAY_GPIO 4
/**
 * @brief Pin del canal del relé que controla el bombillo Plants.
*/
#define PLANTS_RELAY_GPIO 6
/**
 * @brief Pin del regulador de potencia que controla el bombillo UVA.
*/
#define DIMMER_GPIO 7
/**
 * @brief Pin del botón que apaga/enciende el brillo de la pantalla.
*/
#define BUTTON_GPIO 16

/* Functions */
// BUTTON
/**
 * @brief Función que configura el GPIO del botón en modo INPUT_PULLUP,
 * con lo que se ahorra tener que poner una resistencia en el circuito
 * electrónico.
*/
void configButtonGPIO(void);

// Relays
/**
 * @brief Función que configura los GPIO de los relés como salidas.
*/
void configRelayGPIO(void);
/**
 * @brief Función que activa el relé del bombillo UVB.
 * @param value Si value = true, se apaga. value= false, se enciende.
*/
void switchUVBRelay(const bool value);
/**
 * @brief Función que activa el relé del bombillo Plants.
 * @param value Si value = true, se apaga. value= false, se enciende.
*/
void switchPlantsRelay(const bool value);
/**
 * @brief Función que controla el comportamiento del bombillo UVA, en
 * base a una hora de encendido y apagado.
 * @param on_hour Hora de encendido
 * @param on_minute Minuto de encendido
 * @param off_hour Hora de apagado
 * @param off_minite Minuto de apagado
 * 
*/
void controlUVALight(const char* on_hour, const char* on_minute,
                     const char* off_hour, const char* off_minute);
                     /**
 * @brief Función que controla el comportamiento del bombillo UVB, en
 * base a una hora de encendido y apagado.
 * @param on_hour Hora de encendido
 * @param on_minute Minuto de encendido
 * @param off_hour Hora de apagado
 * @param off_minite Minuto de apagado
 * 
*/
void controlUVBLight(const char* on_hour, const char* on_minute,
                     const char* off_hour, const char* off_minute);
                     /**
 * @brief Función que controla el comportamiento del bombillo Plants, en
 * base a una hora de encendido y apagado.
 * @param on_hour Hora de encendido
 * @param on_minute Minuto de encendido
 * @param off_hour Hora de apagado
 * @param off_minite Minuto de apagado
 * 
*/
void controlPlantsLight(const char* on_hour, const char* on_minute,
                     const char* off_hour, const char* off_minute);
                     
// Dimmer
/**
 * @brief Función que configura el GPIO del módulo dimmer/regulador de potencia.
 *  - Se realiza a través de ledc la función creada por Espressif.
 *  - Se ha establecido una frecuencia de 60Hz, al ser esta la frecuencia operativa del
 *    bombillo.
 *  - Para el ESP32-S3, hay 8 canales PWM, de 0 a 7.
 *  - Se ha escogido el canal 1. No se puede escoger el canal 7, está ocupado por el panel
 *    LCD.
 *  - Para una frecuencia de 60Hz, es necesario tener una resolución mayor que 8, en este
 *    caso, 10. Véase la explicación en: 
 *    https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/ledc.html#ledc-api-configure-timer
 * 
 * 
*/
void configDimmerGPIO(void);
/**
 * @brief Función que fija el valor de la señal PWM del bombillo UVA. Al tener una
 * resolución de 10 bits, los valores pueden ir desde 0 a 1023.
 * @param pwmvalue Valor PWM de 0 a 1023.
*/
void setPWM(const int pwm_value);

#endif