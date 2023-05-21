#ifndef SENSOR_GUARD
#define SENSOR_GUARD

#include <DallasTemperature.h>
#include <DHT.h>

#define DHT_PIN_1 9
#define DHT_PIN_2 46
#define DHT_TYPE DHT21

void prepareSensors();
float readTemperatures(const int index);
float readHumidHideTemp(void); 
float readHumidHideHum(void); 
float readEnvironmentTemp(void); 
float readEnvironmentHum(void); 


#endif
