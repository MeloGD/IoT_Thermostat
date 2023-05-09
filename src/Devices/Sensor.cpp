#include "Devices/sensor.h"

SensorDS18B20::SensorDS18B20(const int gpio) {
  OneWire gpio_bus(gpio);
  gpio_wire_bus_ = gpio_bus;
  DallasTemperature sensor(&gpio_wire_bus_);
  sensor_ = sensor;  
  sensor_.begin();
};

const float SensorDS18B20::readTemp(const int index) {
  if (sensor_.getAddress(sensor_address_ , index)) {
    sensor_.requestTemperatures();
    temperature_ = sensor_.getTempCByIndex(index);
    return temperature_;
  } else {
    Serial.print("No se puede conectar con el sensor.");
    return 99.99;
  }
};
