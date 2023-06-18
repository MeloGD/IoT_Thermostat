#ifndef RTC_GUARD
#define RTC_GUARD

#include "RTClib.h"
#include "Wire.h"

#define RTC_SDA 11
#define RTC_SCL 17

/* Variables */
extern RTC_DS3231 rtc;
extern TwoWire i2c_rtc;

/* Functions */
void configRTC(void);
DateTime reportTime(void);

#endif