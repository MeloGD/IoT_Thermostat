#include "Devices/sensor.h"

/* Variables */
OneWire gpio_bus(DS18B20s_GPIO);
DallasTemperature ds18b20_sensors(&gpio_bus);
DeviceAddress ds18b20_address;
DHT dht_environment(DHT_GPIO_1, DHT_TYPE);
DHT dht_humid_hide(DHT_GPIO_2, DHT_TYPE);

/* Functions */
void configSensorsGPIO(void) {
  pinMode(DS18B20s_GPIO, INPUT_PULLUP);
  pinMode(DHT_GPIO_1, INPUT);
  pinMode(DHT_GPIO_2, INPUT);
}

void prepareSensors() {
  ds18b20_sensors.begin();
  dht_environment.begin();
  dht_humid_hide.begin();
}

double readTemperatures(const int index) {
  ds18b20_sensors.getAddress(ds18b20_address , index);
  if (ds18b20_sensors.isConnected(ds18b20_address)) {
    ds18b20_sensors.requestTemperatures();
    return ds18b20_sensors.getTempCByIndex(index);
  } else {
    Serial.print("No se puede conectar con el sensor.");
    return 99.99;
  }  
}

double readHumidHideTemp() {
  double value = dht_humid_hide.readTemperature();
  if (!isnan(value)) {
    return value;
  } else {
    return 99.99;
  }
}

double readHumidHideHum() {
  double value =  dht_humid_hide.readHumidity();
  if (!isnan(value)) {
    return value;
  } else {
    return 99.99;
  }
}

double readEnvironmentTemp() {
  double value = dht_environment.readTemperature();
  if (!isnan(value)) {
    return value;
  } else {
    return 99.99;
  }
}

double readEnvironmentHum() {
  double value = dht_environment.readHumidity();
  if (!isnan(value)) {
    return value;
  } else {
    return 99.99;
  }
}