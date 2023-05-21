#include "Devices/sensor.h"

OneWire gpio_bus(8);
DallasTemperature ds18b20_sensors(&gpio_bus);
DeviceAddress ds18b20_address;
DHT dht_environment(DHT_PIN_1, DHT_TYPE);
DHT dht_humid_hide(DHT_PIN_2, DHT_TYPE);

void prepareSensors() {
  ds18b20_sensors.begin();
  dht_environment.begin();
  dht_humid_hide.begin();
}

float readTemperatures(const int index) {
  ds18b20_sensors.getAddress(ds18b20_address , index);
  if (ds18b20_sensors.isConnected(ds18b20_address)) {
    ds18b20_sensors.requestTemperatures();
    return ds18b20_sensors.getTempCByIndex(index);
  } else {
    Serial.print("No se puede conectar con el sensor.");
    return 99.99;
  }  
}

float readHumidHideTemp() {
  float value = dht_humid_hide.readTemperature();
  if (!isnan(value)) {
    return value;
  } else {
    return 99.99;
  }
}

float readHumidHideHum() {
  float value =  dht_humid_hide.readHumidity();
  if (!isnan(value)) {
    return value;
  } else {
    return 99.99;
  }
}

float readEnvironmentTemp() {
  float value = dht_environment.readTemperature();
  if (!isnan(value)) {
    return value;
  } else {
    return 99.99;
  }
}

float readEnvironmentHum() {
  float value = dht_environment.readHumidity();
  if (!isnan(value)) {
    return value;
  } else {
    return 99.99;
  }
}