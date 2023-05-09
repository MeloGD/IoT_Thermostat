#ifndef SENSOR_GUARD
#define SENSOR_GUARD

#include <DallasTemperature.h>

// generalizar la clase para que funcione con sensores de temperatura y con el sensor de luz
class TempSensor {
  protected:
    float temperature_;
    float humidity_;
  public:
};

class SensorDS18B20: public TempSensor {
  private:
    OneWire gpio_wire_bus_;
    DallasTemperature sensor_;
    DeviceAddress sensor_address_;
  public:
    SensorDS18B20(const int gpio);
    const float readTemp(const int index);

};

class SensorAM2301: public TempSensor {
  private:
  public:
};

#endif
