#ifndef SENSOR_GUARD
#define SENSOR_GUARD

#include <DallasTemperature.h>
#include <DHT.h>

#define DS18B20s_GPIO 8
#define DHT_GPIO_1 9
#define DHT_GPIO_2 46
#define DHT_TYPE DHT21

/* Variables */
extern OneWire gpio_bus;
extern DallasTemperature ds18b20_sensors;
extern DeviceAddress ds18b20_address;
extern DHT dht_environment;
extern DHT dht_humid_hide;
static double warm_hide_temp;
static double cold_hide_temp;
static double environment_temp;
static double environment_hum;
static double humid_hide_temp;
static double humid_hide_hum;

/* Functions */
void configSensorsGPIO(void);
void prepareSensors();
double readTemperatures(const int index);
double readHumidHideTemp(void); 
double readHumidHideHum(void); 
double readEnvironmentTemp(void); 
double readEnvironmentHum(void); 

#endif
