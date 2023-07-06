/**
 * @brief Fichero que define las funciones relativas a los sensores.
 * @file sensor.h
 * @author Jesús Carmelo González Domínguez
 * @date 14-07-2023
*/

#ifndef SENSOR_GUARD
#define SENSOR_GUARD

#include <DallasTemperature.h>
#include <DHT.h>

/**
 * @brief Pin que usará el bus de los sensores DS18B20
*/
#define DS18B20s_GPIO 8
/**
 * @brief Pin que usará el primer sensor DHT
*/
#define DHT_GPIO_1 9
/**
 * @brief Pin que usará el segundo sensor DHT
*/
#define DHT_GPIO_2 46
/**
 * @brief Definición del tipo de sensor DHT
*/
#define DHT_TYPE DHT21

/* Variables */
/**
 * @brief Bus OneWire que utilizarán los sensores DS18B20
*/
extern OneWire gpio_bus;
/**
 * @brief Sensores DS18B20
*/
extern DallasTemperature ds18b20_sensors;
/**
 * @brief Direcciones de los sensores DS18B20, para ser indentificados dentro
 * del bus.
*/
extern DeviceAddress ds18b20_address;
/**
 * @brief Primer sensor DHT, midiendo los valores del ambiente.
*/
extern DHT dht_environment;
/**
 * @brief Segundo sensor DHT, midiendo los valores de la llamada cueva húmeda.
*/
extern DHT dht_humid_hide;
/**
 * @brief Valor de la temperatura en la zona iluminada/calentada por el bombillo
 * UVA.
*/
static double warm_hide_temp;
/**
 * @brief Valor de la temperatura en la zona fría, no calentada por ningún 
 * bombillo.
*/
static double cold_hide_temp;
/**
 * @brief Valor de la temperatura ambiente
*/
static double environment_temp;
/**
 * @brief Valor de la humedad relativa del ambiente
*/
static double environment_hum;
/**
 * @brief Valor de la temperatura de la llamada cueva húmeda.
*/
static double humid_hide_temp;
/**
 * @brief Valor de la humedad relativa de la llamada cueva húmeda.
*/
static double humid_hide_hum;

/* Functions */
/**
 * @brief Configura los pines para funcionar correctamente con los sensores.
*/
void configSensorsGPIO(void);
/**
 * @brief Inicia los sensores.
*/
void prepareSensors(void);
/**
 * @brief Comprueba si un sensor DS18B20 está conectado.
 * @param index Identificador del sensor, puede ser 0 o 1.
 * @return true si está conectado, false si no.
*/
bool isTemperatureSensorConnected(const int index);
/**
 * @brief Lee la temperatura de un sensor DS18B20.
 * @param index Identificador del sensor, puede ser 0 o 1.
 * @return Valor de la temperatura.
*/
double readTemperatures(const int index);
/**
 * @brief Lee la temperatura del sensor DHT en la llamada cueva húmeda.
 * @return Valor de la temperatura.
*/
double readHumidHideTemp(void); 
/**
 * @brief Lee la humedad relativa del sensor DHT en la llamada cueva húmeda.
 * @return Valor de la humedad relativa.
*/
double readHumidHideHum(void); 
/**
 * @brief Lee la temperatura del sensor DHT ambiente.
 * @return Valor de la temperatura.
*/
double readEnvironmentTemp(void); 
/**
 * @brief Lee la humedad relativa del sensor DHT ambiente.
 * @return Valor de la humedad relativa.
*/
double readEnvironmentHum(void); 

#endif
