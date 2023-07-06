/**
 * @brief Fichero que define las funciones relacionadas con el reloj.
 * 
 * @file rtc.h
 * @author Jesús Carmelo González Domínguez
 * @date 14-07-2023
*/

#ifndef RTC_GUARD
#define RTC_GUARD

#include "RTClib.h"
#include "Wire.h"

/**
 * @brief Pin SDA del módulo DS3231 bajo el bus I2C
*/
#define RTC_SDA 11
/**
 * @brief Pin SCL del módulo DS3231 bajo el bus I2C
*/
#define RTC_SCL 17

/* Variables */
/**
 * @brief Instancia del reloj DS3231
*/
extern RTC_DS3231 rtc;
/**
 * @brief Instancia del bus I2C que utilizará el reloj. 
 * Tiene que ser el segundo bus que ofrece la placa porque el primero ya
 * está siendo utilizado por el panel táctil.
*/
extern TwoWire i2c_rtc;

/* Functions */
/**
 * @brief Configura el bus I2C para el reloj y lo inicia. En caso de que al
 * iniciar el dispositivo el reloj haya detectado que se perdió la energía
 * por parte de la batería, se asignará por defecto la hora de compilación
 * como la hora actual.
*/
void configRTC(void);
/**
 * @brief Informa sobre la hora.
 * @param DateTime Devuleve un objeto que contiene toda la información de un
 * determinado instante de tiempo, como la hora, minutos, etc.
*/
DateTime reportTime(void);

#endif