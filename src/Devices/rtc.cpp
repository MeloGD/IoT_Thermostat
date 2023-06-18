#include "Devices/rtc.h"

RTC_DS3231 rtc;
TwoWire i2c_rtc = TwoWire(1);

void configRTC(void) {
  i2c_rtc.begin(RTC_SDA , RTC_SCL, 100000);
  bool status = rtc.begin(&i2c_rtc);
  if (!status) {
    Serial.print("No se ha podido iniciar el reloj");
  } else {
    Serial.print("Se ha podido iniciar el reloj");
  }
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  } 
}

DateTime reportTime(void) {
  return rtc.now();
}